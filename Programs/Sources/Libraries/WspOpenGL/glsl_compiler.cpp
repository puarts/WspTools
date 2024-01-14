
#include "glsl_compiler.h"

#include <string.h>
#include <memory>
#include <regex>
#include <sstream>

using namespace wsp::opengl;

namespace
{
    //----
    std::string LoadTextFile(const char* filename)
    {
        // ファイルを開く
        FILE* fp = fopen(filename, "rb");
        if (fp == NULL)
        {
            return std::string();
        }

        // ファイルサイズ取得
        fseek(fp, 0, SEEK_END);
        size_t size = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        // ファイル読み込み
        std::unique_ptr<char[]> mem(WSP_NEW char[size + 1]);
        memset(mem.get(), 0, sizeof(char) * (size + 1));
        fread(mem.get(), size, 1, fp);

        // ファイルを閉じる
        fclose(fp);

        return std::string(mem.get());
    }

    //----
    std::string FindFile(const std::vector<std::string>& paths, const std::string& filename)
    {
        std::vector<std::string>::const_iterator it = paths.begin();
        for (; it != paths.end(); ++it)
        {
            std::string filepath = (*it) + "/" + filename;

            // ファイルを開いてみる
            FILE* fp = fopen(filepath.c_str(), "r");
            bool isOpened = fp != NULL;
            fclose(fp);
            if (isOpened)
            {
                return filepath;
            }
        }

        return std::string();
    }

    //----
    std::string ResolveInclude(const std::string& source, const std::vector<std::string>& paths, int level = 0)
    {
        if (level > 16)
        {
            printf("警告！\n");
            printf("    インクルードファイルの深度がリミットに到達しました。\n");
            printf("    循環参照の可能性があるため、インクルードファイルの解決を打ち切ります。\n");
            return source;
        }

        using namespace std;

        static const regex kIncludePattern("^[ ]*#[ ]*include[ ]+[\"<](.*)[\">].*");
        stringstream inputStream;
        stringstream outputStream;
        inputStream << source;

        smatch matches;
        string line;
        while (getline(inputStream, line))
        {
            if (regex_search(line, matches, kIncludePattern))
            {
                string include_filename = matches[1];
                string include_filepath = FindFile(paths, include_filename);
                if (include_filepath.empty())
                {
                    printf("GLSLのコンパイルに失敗しました\n");
                    printf("    インクルードファイル(%s)が指定パスに見つかりませんでした。\n", include_filename.c_str());
                }
                string include_string = LoadTextFile(include_filepath.c_str());
                if (include_string.empty())
                {
                    printf("GLSLのコンパイルに失敗しました\n");
                    printf("    インクルードファイル(%s)の読み込みに失敗しました。\n", include_filepath.c_str());
                }

                outputStream << ResolveInclude(include_string, paths, level + 1) << endl;
            }
            else
            {
                outputStream << line << endl;
            }
        }

        return outputStream.str();
    }

    //----
    // シェーダオブジェクトのコンパイル結果を表示する
    bool PrintShaderInfoLog(GLuint shader)
    {
        // コンパイル結果を取得する
        GLint status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE)
        {
            printf("GLSLのコンパイルに失敗しました\n");
        }

        // シェーダのコンパイル時のログの長さを取得する
        GLsizei bufSize;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufSize);

        if (bufSize > 1)
        {
            // シェーダのコンパイル時のログの内容を取得する
            std::unique_ptr<GLchar[]> infoLog(WSP_NEW GLchar[bufSize]);
            GLsizei length;
            glGetShaderInfoLog(shader, bufSize, &length, infoLog.get());
            printf("    %s\n", infoLog.get());
        }

        return status == GL_TRUE;
    }

    bool HasCodeText(const char* source)
    {
        for (const char* char_ptr = source; *char_ptr != '\0'; ++char_ptr)
        {
            if (*char_ptr == '/')
            {
                // スラッシュがあるものは即コメントアウトとみなす
                // コンパイルが通るソースなら問題ないはず
                return false;
            }
            else if (*char_ptr == '\n' || *char_ptr == '\r')
            {
                // 改行は無視
            }
            else if (*char_ptr != ' ')
            {
                // 空白か改行以外の文字が見つかればコードがあるとみなす
                return true;
            }
        }

        // 空白しかない
        return false;
    }
}    // namespace

class GlslCompiler::Impl
{
public:
	std::vector<std::string>    m_includePaths;                // インクルードファイルを検索するパス
	std::vector<DefineValue>    m_defines;                    // 標準的に使用する定義
};

GlslCompiler::GlslCompiler()
	: m_impl(WSP_NEW Impl())
{
}

GlslCompiler::~GlslCompiler()
{
	delete m_impl;
}

int GlslCompiler::AddIncludePath(const std::string& path)
{
	m_impl->m_includePaths.push_back(path);
	return (int)m_impl->m_includePaths.size();
}

void GlslCompiler::ClearIncludePaths()
{
	m_impl->m_includePaths.clear();
}

int GlslCompiler::AddDefine(const std::string& def, const std::string& val)
{
	m_impl->m_defines.push_back(DefineValue(def, val));
	return (int)m_impl->m_defines.size();
}

void GlslCompiler::ClearDefines()
{
	m_impl->m_defines.clear();
}

//----
bool GlslCompiler::HasIncludeARB()
{
    const GLubyte* exts = glGetString(GL_EXTENSIONS);
    return strstr(reinterpret_cast<const char*>(exts), "GL_ARB_shading_language_include") != NULL;
}

//----
bool GlslCompiler::CompileFromFile(GLuint shader, const char* filename)
{
    std::string source = LoadTextFile(filename);
    if (source.empty())
    {
        printf("GLSLのコンパイルに失敗しました\n");
        printf("    シェーダファイル(%s)が見つかりません。\n", filename);
        return false;
    }

    return CompileFromString(shader, source);
}

//----
bool GlslCompiler::CompileFromString(GLuint shader, const std::string& source)
{
    std::string resolve_source;
    bool result = GetResolvedSource(&resolve_source, source);
    if (!result)
    {
        return result;
    }

    // コンパイルする
    const char* ps = resolve_source.c_str();
    glShaderSource(shader, 1, reinterpret_cast<const GLchar**>(&ps), NULL);
    glCompileShader(shader);
    if (!PrintShaderInfoLog(shader))
    {
        return false;
    }

    return true;
}

bool GlslCompiler::GetResolvedSource(std::string* o_resolved_source, const char* filename)
{
    std::string source = LoadTextFile(filename);
    if (source.empty())
    {
        printf("GLSLのコンパイルに失敗しました\n");
        printf("    シェーダファイル(%s)が見つかりません。\n", filename);
        return false;
    }

    return GetResolvedSource(o_resolved_source, source);
}

bool GlslCompiler::GetResolvedSource(std::string* o_resolved_source, const std::string& source)
{
    std::string resolve_source;
    std::string macro_definitions;

    // 定義を書き込む
    std::vector<DefineValue>::iterator it = m_impl->m_defines.begin();
    for (; it != m_impl->m_defines.end(); ++it)
    {
        macro_definitions += "#define " + it->first + " " + it->second + "\n";
    }

    // #version マクロの対処
    {
        using namespace std;

        static const regex kVersionPattern("^[ ]*#[ ]*version[ ]+(.*)");
        stringstream inputStream;
        inputStream << source;

        smatch matches;
        string first_line;
        while (getline(inputStream, first_line))
        {
            if (HasCodeText(first_line.c_str()))
            {
                break;
            }
        }

        if (regex_search(first_line, matches, kVersionPattern))
        {
            resolve_source += first_line + "\n";
            resolve_source += macro_definitions;
        }
        else
        {
            resolve_source += macro_definitions;
            resolve_source += first_line + "\n";
        }

        string line;
        while (getline(inputStream, line))
        {
            resolve_source += line + "\n";
        }
    }

    // #includeディレクティブを解決する
    resolve_source = ResolveInclude(resolve_source, m_impl->m_includePaths);

    if (resolve_source.empty())
    {
        return false;
    }

    *o_resolved_source = resolve_source;

    return true;
}

//    EOF

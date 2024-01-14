#pragma once

#include <wsp/common/_define_commoncore.h>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <wsp/common/fn-debug.h>

namespace wsp{
    namespace opengl{

        // GLSLコンパイラ
        class WSP_DLL_EXPORT GlslCompiler
        {
        public:
            typedef std::pair<std::string, std::string>    DefineValue;

        public:
			GlslCompiler();
			~GlslCompiler();

            // インクルードパスを追加する
			int AddIncludePath(const std::string& path);

            int AddIncludePath(const char* path)
            {
                return AddIncludePath(std::string(path));
            }

            // インクルードパスをクリアする
			void ClearIncludePaths();

            // 定義を追加する
			int AddDefine(const std::string& def, const std::string& val);
            int AddDefine(const char* def, const char* val)
            {
                WSP_REQUIRES_NOT_NULL(def);
                WSP_REQUIRES_NOT_NULL(val);
                return AddDefine(std::string(def), std::string(val));
            }

            // 定義をクリアする
			void ClearDefines();

            // ファイルからGLSLをコンパイルする
            bool CompileFromFile(GLuint shader, const char* filename);

            // 文字列からGLSLをコンパイルする
            bool CompileFromString(GLuint shader, const std::string& source);

            bool GetResolvedSource(std::string* o_resolved_source, const char* filename);
            bool GetResolvedSource(std::string* o_resolved_source, const std::string& source);

        public:
            // GLSLで#includeディレクティブが使用できるかどうか調べる
            static bool HasIncludeARB();

        private:
			class Impl;
			Impl* m_impl;
        };    // class GlslCompiler
    }
}

//    EOF

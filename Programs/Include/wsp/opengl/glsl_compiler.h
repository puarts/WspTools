#pragma once

#include <wsp/common/_define_commoncore.h>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <wsp/common/fn-debug.h>

namespace wsp{
    namespace opengl{

        // GLSL�R���p�C��
        class WSP_DLL_EXPORT GlslCompiler
        {
        public:
            typedef std::pair<std::string, std::string>    DefineValue;

        public:
			GlslCompiler();
			~GlslCompiler();

            // �C���N���[�h�p�X��ǉ�����
			int AddIncludePath(const std::string& path);

            int AddIncludePath(const char* path)
            {
                return AddIncludePath(std::string(path));
            }

            // �C���N���[�h�p�X���N���A����
			void ClearIncludePaths();

            // ��`��ǉ�����
			int AddDefine(const std::string& def, const std::string& val);
            int AddDefine(const char* def, const char* val)
            {
                WSP_REQUIRES_NOT_NULL(def);
                WSP_REQUIRES_NOT_NULL(val);
                return AddDefine(std::string(def), std::string(val));
            }

            // ��`���N���A����
			void ClearDefines();

            // �t�@�C������GLSL���R���p�C������
            bool CompileFromFile(GLuint shader, const char* filename);

            // �����񂩂�GLSL���R���p�C������
            bool CompileFromString(GLuint shader, const std::string& source);

            bool GetResolvedSource(std::string* o_resolved_source, const char* filename);
            bool GetResolvedSource(std::string* o_resolved_source, const std::string& source);

        public:
            // GLSL��#include�f�B���N�e�B�u���g�p�ł��邩�ǂ������ׂ�
            static bool HasIncludeARB();

        private:
			class Impl;
			Impl* m_impl;
        };    // class GlslCompiler
    }
}

//    EOF

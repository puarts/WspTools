#pragma once

#include <wsp/image/tpl_cl-image.h>

namespace wsp {
    namespace graphic {
        class Material
        {
        public:
            Material()
                : diffuse_color_r_(0.0f),
                  diffuse_color_g_(0.0f),
                  diffuse_color_b_(0.0f),
                  ambient_color_r_(0.0f),
                  ambient_color_g_(0.0f),
                  ambient_color_b_(0.0f),
                  specular_color_r_(0.0f),
                  specular_color_g_(0.0f),
                  specular_color_b_(0.0f)
            {
                strcpy(name_, "");
                strcpy(diffuse_tex_file_name_, "");
            }

            Material(const Material& rhs)
            {
                strncpy(name_, rhs.name_, MAX_LENGTH_OF_NAME);
                strncpy(diffuse_tex_file_name_, rhs.diffuse_tex_file_name_, MAX_LENGTH_OF_NAME);
                diffuse_color_r_  = rhs.diffuse_color_r_;
                diffuse_color_g_ = rhs.diffuse_color_g_;
                diffuse_color_b_ = rhs.diffuse_color_b_;
                ambient_color_r_ = rhs.ambient_color_r_;
                ambient_color_g_ = rhs.ambient_color_g_;
                ambient_color_b_ = rhs.ambient_color_b_;
                specular_color_r_ = rhs.specular_color_r_;
                specular_color_g_ = rhs.specular_color_g_;
                specular_color_b_ = rhs.specular_color_b_;
            }

            ~Material()
            {
            }

            const char* GetName() const
            {
                return name_;
            }

            void SetName(const char* name)
            {
                strncpy(name_, name, MAX_LENGTH_OF_NAME);
            }

            const char* GetDiffuseTextureFileName() const
            {
                return diffuse_tex_file_name_;
            }

            void SetDiffuseTextuerFileName(const char* file_name)
            {
                strncpy(diffuse_tex_file_name_, file_name, MAX_LENGTH_OF_NAME);
            }

            float GetDiffuseColorR() const { return diffuse_color_r_; }
            float GetDiffuseColorG() const { return diffuse_color_g_; }
            float GetDiffuseColorB() const { return diffuse_color_b_; }
            float GetAmbientColorR() const { return ambient_color_r_; }
            float GetAmbientColorG() const { return ambient_color_g_; }
            float GetAmbientColorB() const { return ambient_color_b_; }
            float GetSpecularColorR() const { return specular_color_r_; }
            float GetSpecularColorG() const { return specular_color_g_; }
            float GetSpecularColorB() const { return specular_color_b_; }

            void SetDiffuseColorR(float value) { diffuse_color_r_ = value; }
            void SetDiffuseColorG(float value) { diffuse_color_g_ = value; }
            void SetDiffuseColorB(float value) { diffuse_color_b_ = value; }
            void SetAmbientColorR(float value) { ambient_color_r_ = value; }
            void SetAmbientColorG(float value) { ambient_color_g_ = value; }
            void SetAmbientColorB(float value) { ambient_color_b_ = value; }
            void SetSpecularColorR(float value) { specular_color_r_ = value; }
            void SetSpecularColorG(float value) { specular_color_g_ = value; }
            void SetSpecularColorB(float value) { specular_color_b_ = value; }

        private:
            char name_[MAX_LENGTH_OF_NAME];
            char diffuse_tex_file_name_[MAX_LENGTH_OF_NAME];

            float diffuse_color_r_;
            float diffuse_color_g_;
            float diffuse_color_b_;
            float ambient_color_r_;
            float ambient_color_g_;
            float ambient_color_b_;
            float specular_color_r_;
            float specular_color_g_;
            float specular_color_b_;
        };
    }
}
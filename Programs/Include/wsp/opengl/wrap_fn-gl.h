/**
 * @file wrap_fn-gl.h
 * 
 * @brief GL wrapper functions.
 */

#ifndef __WSP_GRAPHIC_FUNCTION_GL_H__
#define __WSP_GRAPHIC_FUNCTION_GL_H__

#ifndef __WSP_COMMONCORE_CTYPE_H__
#include <wsp/common/_ctypes.h>
#endif


#ifdef __cplusplus
extern "C"{
#endif
    
    #include <GL/glew.h>
    
#ifdef _WIN32
    #include <GL/glut.h>
#else
    #include <GLUT/GLUT.h>
#endif

#ifdef __cplusplus
}
#endif

namespace wsp{ namespace opengl{
    WSP_DLL_EXPORT void GlInit(int *argc, char **argv);

    /**
     *   Load shader file.
     */
    WSP_DLL_EXPORT GLuint GlCompileShaderFile(GLenum shader_type, const char *filename);
    WSP_DLL_EXPORT GLuint GlLoadShaderFile(const char *vsh_file, const char *fsh_file);

    #ifdef USE_QT
    /** @brief  Load GLSL shader file using QDialog. */
    WSP_DLL_EXPORT GLuint GlLoadShaderFileDialog();
    #endif

    // GL -----------------
    // drawing string function
    WSP_DLL_EXPORT void glDrawString(
            const char *str, 
            f32 x = 32,
            f32 y = 32,
            f32 font_color_r = 1.0f,
            f32 font_color_g = 1.0f,
            f32 font_color_b = 1.0f,
            f32 font_color_a = 1.0f,
            void *font = GLUT_BITMAP_TIMES_ROMAN_10,
            int font_height = 13
            );

    ///** @brief Draw circle. */
    //WSP_DLL_EXPORT void glDrawCircle(float x, float y, float radius);

    ///** @brief Draw rectangle. */
    //WSP_DLL_EXPORT void glDrawRectangle(float topLeftX, float topLeftY, float bottomRightX, float bottomRightY);

    /** @return texture ID */
    WSP_DLL_EXPORT GLuint glInitTexture( 
            u8 **tex_rgba8_buffer, 
            u32 *o_tex_width, u32 *o_tex_height,
            u32 width, u32 height
        );

    WSP_DLL_EXPORT void glDestroyTexture(u8 **tex_rgba8_buffer);

    /** @brief Draw rectangle texture. */
    //WSP_DLL_EXPORT void glDrawTexture(float topLeftX, float topLeftY, float bottomRightX, float bottomRightY);

}}

#endif


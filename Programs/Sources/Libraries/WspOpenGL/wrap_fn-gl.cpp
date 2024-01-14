/**
 * @file fn-gl.cpp
 * 
 * @brief GL wrapper functions.
 */

#ifndef __WSP_COMMONUTIL_FN_UTIL_H__
#include <wsp/common/fn-util.h>
#endif
#include "wrap_fn-gl.h"
#include <wsp/common/fn-debug.h>
#include <wsp/math/_fn-math_core.h>
#include <wsp/opengl/GlProxy.h>

#include <math.h>


void wsp::opengl::GlInit(int *argc, char **argv) {
    // //////////////////////////////////////////////
    //                  GL INIT
    // //////////////////////////////////////////////

    // GLUT Initialization
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
}

GLuint wsp::opengl::GlCompileShaderFile(GLenum shader_type, const char *filename)
{
    FILE *fp;
    size_t read_size;
    GLchar *shader_src;
    GLuint shader;


    if ((fp = fopen(filename, "rb")) == NULL) {
        printf("can not open %s\n", filename);
        return 0;
    }

    fseek(fp, 0, SEEK_END);
    size_t len = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if (!(shader_src = (GLchar *)calloc(len + 1, sizeof(char)))) {
        printf("can not allocate memory.\n");
        fclose(fp);
        return 0;
    }

    if ((read_size = fread((void *)shader_src, sizeof(char), len, fp)) != len)
    {
        printf("can not read file. %zu %zu\n", read_size, len);
        fclose(fp);
        free((void *)shader_src);
        return 0;
    }
    fclose(fp);


    /*
     *  Compile shader
     */
    shader = glCreateShader(shader_type);

    if (shader) {
        GLint compiled;
        const GLchar *src = shader_src;


        glShaderSource(shader, 1, &src, NULL);

        glCompileShader(shader);

        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

        if (!compiled) {
            GLint len;
            GLchar *log;

            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
            log = (GLchar *)malloc(len);

            glGetShaderInfoLog(shader, len, &len, log);

            printf(
                "\n*****\n"
                "%s Compile\n"
                "*****\n"
                "%s\n"
                , shader_type == GL_VERTEX_SHADER ? "VS" : "FS"
                , log);

            free(log);
        }
    }
    free((void *)shader_src);

    return shader;
}


GLuint wsp::opengl::GlLoadShaderFile(const char*vsh_file, const char *fsh_file)
{
    /* program object */
    GLuint program = 0;

    /* shader objects */
    GLuint vs = 0;
    GLuint fs = 0;

    if ((vs = wsp::opengl::GlCompileShaderFile(GL_VERTEX_SHADER, vsh_file)) == 0) {
        printf("can not read shader. : %s\n", vsh_file);
        return 0;
    }

    if ((fs = wsp::opengl::GlCompileShaderFile(GL_FRAGMENT_SHADER, fsh_file)) == 0) {
        printf("can not read shader. : %s\n", fsh_file);
        return 0;
    }

    program = glCreateProgram();

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    /*
     *  Linking
     */
    GLint linked;
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &linked);

    if (!linked)
    {
        GLint len;
        GLchar *log;

        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
        log = (GLchar *)malloc(len);

        glGetProgramInfoLog(program, len, &len, log);
        printf(
            "\n*****\n"
            "Link\n"
            "*****\n"
            "%s\n", log);

        free(log);
        return 0;
    }
    wsp::opengl::glproxy::UseProgram(program);

    return program;
}

// GL Functions -------------------------------------------
void wsp::opengl::glDrawString(
    const char *str,
    f32 x,
    f32 y,
    f32 font_color_r,
    f32 font_color_g,
    f32 font_color_b,
    f32 font_color_a,
    void *font,
    int font_height
)
{
    int wy;
    int width, height;

    width = glutGet((GLenum)GLUT_WINDOW_WIDTH);
    height = glutGet((GLenum)GLUT_WINDOW_HEIGHT);

    /* 固定パイプラインに戻す */
    (wsp::opengl::glproxy::UseProgram(0));

    (wsp::opengl::glproxy::Disable(GL_DEPTH_TEST));
    (wsp::opengl::glproxy::Disable(GL_LIGHTING));
    (wsp::opengl::glproxy::ActiveTexture(GL_TEXTURE0));
    (wsp::opengl::glproxy::Disable(GL_TEXTURE_2D));
    (wsp::opengl::glproxy::Disable(GL_TEXTURE_CUBE_MAP));
    (wsp::opengl::glproxy::ActiveTexture(GL_TEXTURE1));
    (wsp::opengl::glproxy::Disable(GL_TEXTURE_2D));
    (wsp::opengl::glproxy::Disable(GL_TEXTURE_CUBE_MAP));
    (wsp::opengl::glproxy::MatrixMode(GL_PROJECTION));
    (wsp::opengl::glproxy::PushMatrix());
    (wsp::opengl::glproxy::LoadIdentity());
    (wsp::opengl::glproxy::Ortho2D(0.0f, (float)width, 0.0f, (float)height));
    (wsp::opengl::glproxy::MatrixMode(GL_MODELVIEW));
    (wsp::opengl::glproxy::PushMatrix());
    (wsp::opengl::glproxy::LoadIdentity());
    (wsp::opengl::glproxy::Color4f(font_color_r, font_color_g, font_color_b, font_color_a));

#if OPENGL_IMAGE_COORDINATE
    wy = y;
#else
    wy = height - y - font_height;
#endif
    (wsp::opengl::glproxy::RasterPos2i(x, wy));

    for (; *str != 0; str++)
    {
        if (*str == '\n')
        {
            wy -= font_height;
            (wsp::opengl::glproxy::RasterPos2i(x, wy));
            continue;
        }

        (wsp::opengl::glproxy::BitmapCharacter(font, *str));
    }

    (wsp::opengl::glproxy::MatrixMode(GL_PROJECTION));
    (wsp::opengl::glproxy::PopMatrix());
    (wsp::opengl::glproxy::MatrixMode(GL_MODELVIEW));
    (wsp::opengl::glproxy::PopMatrix());
}

static const float DEG2RAD = 3.14159 / 180;
//void wsp::opengl::glDrawCircle(float x, float y, float radius)
//{
//   wsp::opengl::glproxy::Begin(GL_TRIANGLE_FAN);
// 
//   for (int i=0; i < 360; i++)
//   {
//      float degInRad = i*DEG2RAD;
//      wsp::opengl::glproxy::Vertex2f(x + cos(degInRad)*radius, y + sin(degInRad)*radius);
//   }
// 
//   wsp::opengl::glproxy::End();
//}
//
//void wsp::opengl::glDrawRectangle(float topLeftX, float topLeftY, float bottomRightX, float bottomRightY)
//{
//    GLfloat verts[8] = {
//        topLeftX, topLeftY,
//        topLeftX, bottomRightY,
//        bottomRightX, bottomRightY,
//        bottomRightX, topLeftY
//    };
//
//    wsp::opengl::glproxy::Begin(GL_TRIANGLE_FAN);
//
//    for (int i = 0; i < 4; ++i)
//    {
//        wsp::opengl::glproxy::Vertex2f(verts[i * 2], verts[i * 2 + 1]);
//    }
//
//    wsp::opengl::glproxy::End();
//
//    //glVertexPointer(2, GL_FLOAT, 0, verts);
//    //glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
//
//    //TODO: Maybe glFinish needed here instead - if there's some bad graphics crap
//    //glFlush();
//}

namespace {
    GLfloat texcoords[8];
}
GLuint wsp::opengl::glInitTexture(
    u8 **tex_rgba8_buffer,
    u32 *o_tex_width, u32 *o_tex_height,
    u32 width, u32 height
)
{
    WSP_ASSERT(tex_rgba8_buffer != NULL, "tex_rgba8_buffer must not be NULL");
    WSP_ASSERT(o_tex_width != NULL, "o_tex_width must not be NULL");
    WSP_ASSERT(o_tex_height != NULL, "o_tex_height must not be NULL");

    GLuint tex_id = 0;
    glGenTextures(1, &tex_id);

    *o_tex_width = WSP_GetClosestPowerOfTwo(width);
    *o_tex_height = WSP_GetClosestPowerOfTwo(height);
    size_t tex_buffer_size = (*o_tex_width) * (*o_tex_height) * 4;
    *tex_rgba8_buffer = WSP_NEW u8[tex_buffer_size];
    WSP_ASSERT(*tex_rgba8_buffer != NULL, "memory alloction failed");

    wsp::opengl::glproxy::BindTexture(GL_TEXTURE_2D, tex_id);

    wsp::opengl::glproxy::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    wsp::opengl::glproxy::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    f32 texXpos = (f32)width / (*o_tex_width);
    f32 texYpos = (f32)height / (*o_tex_height);

    memset(texcoords, 0, 8 * sizeof(float));
    texcoords[0] = texXpos, texcoords[1] = texYpos, texcoords[2] = texXpos, texcoords[7] = texYpos;

    return tex_id;
}

void wsp::opengl::glDestroyTexture(u8 **tex_rgba8_buffer)
{
    WSP_ASSERT(tex_rgba8_buffer != NULL, "tex_rgba8_buffer must not be NULL");
    delete[](*tex_rgba8_buffer);
    (*tex_rgba8_buffer) = NULL;
}
//
//void wsp::opengl::glDrawTexture(float topLeftX, float topLeftY, float bottomRightX, float bottomRightY)
//{
//    wsp::opengl::glproxy::EnableClientState(GL_TEXTURE_COORD_ARRAY);
//    wsp::opengl::glproxy::TexCoordPointer(2, GL_FLOAT, 0, texcoords);
//
//    wsp::opengl::glDrawRectangle(topLeftX, topLeftY, bottomRightX, bottomRightY);
//
//    wsp::opengl::glproxy::DisableClientState(GL_TEXTURE_COORD_ARRAY);
//}

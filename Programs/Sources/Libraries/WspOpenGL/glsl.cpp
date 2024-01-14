/********************************************************************
glsl.cpp
Version: 1.0.0_rc5
Last update: 2006/11/12 (Geometry Shader Support)

(c) 2003-2006 by Martin Christen. All Rights reserved.
*********************************************************************/

#include "glsl.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <math.h>

#include <wsp/common/fn-debug.h>
#include <wsp/opengl/GlProxy.h>

using namespace std;
using namespace wsp::opengl;

namespace{
    bool g_use_glsl = false;
    bool g_gl_extensions_initialized = false;
    bool g_is_geometry_shader_supported = false;
    bool g_has_gpu_shader_model4 = false;

    void SplitString(std::vector<std::string>* o_str, const std::string &str, char delim)
    {
        size_t current = 0, found;
        while ((found = str.find_first_of(delim, current)) != std::string::npos)
        {
            o_str->push_back(std::string(str, current, found - current));
            current = found + 1;
        }
        o_str->push_back(std::string(str, current, str.size() - current));
    }

    void AppendLineNumbersToText(std::string* o_result, const std::string& shader_code)
    {
        std::vector<std::string> splited;
        SplitString(&splited, shader_code, '\n');
        int line_number = 1;
        for (std::vector<std::string>::iterator iter = splited.begin(), end = splited.end(); iter != end; ++iter, ++line_number)
        {
            (*o_result) += std::to_string(line_number) + ":" + (*iter) + "\n";
        }
    }
}

//-----------------------------------------------------------------------------
/*! \mainpage
\section s_intro Introduction
This is libglsl - a collection of helper classes to Load, Compile, Link and activate shaders
written in the OpenGL Shading language. Vertex Shaders, Geometry Shaders and Fragment shaders
are supported (if the hardware is capable of supporting them, of course).

Version info: \ref s_libglslWSP_NEWs

\section s_examples Examples

\subsection Loading Vertex and Fragment Shader using Shader Manager.

\verbatim
Initialization:
GlShaderManager SM;
GlShader *shader = SM.LoadFromFile("test.vert","test.frag");
if (shader==0)
cout << "Error Loading, compiling or Linking shader\n";

Render:
shader->begin();
shader->SetUniform1f("MyFloat", 1.123);
glutDrawSolidSphere(1.0);
shader->end();
\endverbatim

\subsection geom_shader Geometry Shader
The easiest way to use Geometry Shaders is through the
Shadermanager.
Initialization:
\verbatim
SM.SetInputPrimitiveType(GL_TRIANGLES);
SM.SetOutputPrimitiveType(GL_TRIANGLE_STRIP);
SM.SetVerticesOut(3);
GlShader *shader = SM.LoadFromFile("test.vert","test.geom","test.frag");
\endverbatim

*/

#define CHECK_GL_ERROR() CheckGLError(__FILE__, __LINE__)

namespace wsp{
    namespace opengl{
        //-----------------------------------------------------------------------------
        // Error, Warning and Info Strings
        char* aGLSLStrings[] = {
            "[e00] GLSL is not available!",
            "[e01] Not a valid program object!",
            "[e02] Not a valid object!",
            "[e03] Out of memory!",
            "[e04] Unknown Compiler error!",
            "[e05] Linker log is not available!",
            "[e06] Compiler log is not available!",
            "[Empty]"
        };
        //-----------------------------------------------------------------------------      

        // GL ERROR CHECK
        int CheckGLError(char *file, int line)
        {
            GLenum error_code;
            int    retCode = 0;

            error_code = glGetError();
            while (error_code != GL_NO_ERROR)
            {
                const GLubyte* sError = gluErrorString(error_code);

                if (sError)
                {
                    cout << "GL Error #" << error_code << "(" << gluErrorString(error_code) << ") " << " in File " << file << " at line: " << line << endl;
                }
                else
                {
                    cout << "GL Error #" << error_code << " (no message available)" << " in File " << file << " at line: " << line << endl;
                }

                WSP_PrintCallStack();
                retCode = 1;
                error_code = glGetError();
            }

            return retCode;
        }

        //----------------------------------------------------------------------------- 
        bool InitOpenGLExtensions(void)
        {
            if (g_gl_extensions_initialized)
            {
                return true;
            }
            g_gl_extensions_initialized = true;

            GLenum err = glewInit();

            if (GLEW_OK != err)
            {
                cout << "Error:" << glewGetErrorString(err) << endl;
                g_gl_extensions_initialized = false;
                return false;
            }

            cout << "OpenGL Vendor: " << (char*)glGetString(GL_VENDOR) << "\n";
            cout << "OpenGL Renderer: " << (char*)glGetString(GL_RENDERER) << "\n";
            cout << "OpenGL Version: " << (char*)glGetString(GL_VERSION) << "\n\n";
            //cout << "OpenGL Extensions:\n" << (char*) glGetString(GL_EXTENSIONS) << "\n\n";

            HasGLSLSupport();

            return true;
        }


        bool HasGLSLSupport(void)
        {
            g_is_geometry_shader_supported = HasGeometryShaderSupport();
            g_has_gpu_shader_model4 = HasShaderModel4();

            if (g_use_glsl)
            {
                return true;  // already initialized and GLSL is available
            }

            g_use_glsl = true;

            if (!g_gl_extensions_initialized) InitOpenGLExtensions();  // extensions were not yet initialized!!


            if (GLEW_VERSION_2_0)
            {
                cout << "OpenGL 2.0 (or higher) is available!" << endl;
            }
            else if (GLEW_VERSION_1_5)
            {
                cout << "OpenGL 1.5 core functions are available" << endl;
            }
            else if (GLEW_VERSION_1_4)
            {
                cout << "OpenGL 1.4 core functions are available" << endl;
            }
            else if (GLEW_VERSION_1_3)
            {
                cout << "OpenGL 1.3 core functions are available" << endl;
            }
            else if (GLEW_VERSION_1_2)
            {
                cout << "OpenGL 1.2 core functions are available" << endl;
            }

            if (GL_TRUE != glewGetExtension("GL_ARB_fragment_shader"))
            {
                cout << "[WARNING] GL_ARB_fragment_shader extension is not available!\n";
                g_use_glsl = false;
            }

            if (GL_TRUE != glewGetExtension("GL_ARB_vertex_shader"))
            {
                cout << "[WARNING] GL_ARB_vertex_shader extension is not available!\n";
                g_use_glsl = false;
            }

            if (GL_TRUE != glewGetExtension("GL_ARB_shader_objects"))
            {
                cout << "[WARNING] GL_ARB_shader_objects extension is not available!\n";
                g_use_glsl = false;
            }

            if (g_use_glsl)
            {
                cout << "[OK] OpenGL Shading Language is available!\n\n";
            }
            else
            {
                cout << "[FAILED] OpenGL Shading Language is not available...\n\n";
            }

            return g_use_glsl;
        }


        bool HasOpenGL2Support(void)
        {
            if (!g_gl_extensions_initialized) InitOpenGLExtensions();

            return (GLEW_VERSION_2_0 == GL_TRUE);
        }


        bool HasGeometryShaderSupport(void)
        {
            if (GL_TRUE != glewGetExtension("GL_EXT_geometry_shader4"))
            {
                return false;
            }

            return true;
        }

        bool HasShaderModel4(void)
        {
            if (GL_TRUE != glewGetExtension("GL_EXT_gpu_shader4"))
            {
                return false;
            }

            return true;
        }
    }
}
//----------------------------------------------------------------------------- 

// ************************************************************************
// Implementation of GlShader class
// ************************************************************************

class GlShader::Impl
{
public:
	std::vector<GlShaderObject*> shader_list_;       // List of all Shader Programs
};

GlShader::GlShader()
	: impl_(WSP_NEW Impl())
{
    InitOpenGLExtensions();
    program_object_ = 0;
    linker_log = 0;
    is_linked_ = false;
    _noshader = true;

    if (!g_use_glsl)
    {
        cout << "**ERROR: OpenGL Shading Language is NOT available!" << endl;
    }
    else
    {
        program_object_ = glCreateProgram();
    }
}

//----------------------------------------------------------------------------- 

GlShader::~GlShader()
{
    if (linker_log != 0)
    {
        free(linker_log);
    }

    for (unsigned int i = 0; i < impl_->shader_list_.size(); i++)
    {
        glDetachShader(program_object_, impl_->shader_list_[i]->shader_object_);
        CHECK_GL_ERROR(); // if you get an error here, you deleted the Program object first and then
        delete impl_->shader_list_[i];
    }

    if (program_object_ != 0)
    {
        (glDeleteProgram(program_object_));
        CHECK_GL_ERROR();
    }

	delete impl_;
}

//----------------------------------------------------------------------------- 

void GlShader::AddShader(GlShaderObject* shader_program)
{
    if (!g_use_glsl)
    {
        return;
    }

    if (shader_program == 0)
    {
        return;
    }

    if (!shader_program->is_compiled_)
    {
        cout << "**warning** please Compile program before adding object! trying to Compile now...\n";
        if (!shader_program->Compile())
        {
            cout << "...Compile ERROR!\n";
            return;
        }
        else
        {
            cout << "...ok!\n";
        }
    }

	impl_->shader_list_.push_back(shader_program);

}

//----------------------------------------------------------------------------- 

void GlShader::SetInputPrimitiveType(int nInputPrimitiveType)
{
    input_primitive_type_ = nInputPrimitiveType;
}

void GlShader::SetOutputPrimitiveType(int nOutputPrimitiveType)
{
    output_primitive_type_ = nOutputPrimitiveType;
}

void GlShader::SetVerticesOut(int nVerticesOut)
{
    max_number_of_output_vertices_ = nVerticesOut;
}
//----------------------------------------------------------------------------- 

bool GlShader::Link(void)
{
    if (!g_use_glsl)
    {
        return false;
    }

    unsigned int i;

    //if (uses_geometry_shader_)
    //{
    //    (glProgramParameteriEXT(program_object_, GL_GEOMETRY_INPUT_TYPE_EXT, input_primitive_type_));
    //    (glProgramParameteriEXT(program_object_, GL_GEOMETRY_OUTPUT_TYPE_EXT, output_primitive_type_));
    //    (glProgramParameteriEXT(program_object_, GL_GEOMETRY_VERTICES_OUT_EXT, max_number_of_output_vertices_));
    //}

    if (is_linked_)  // already Linked, detach everything first
    {
        cout << "**warning** Object is already Linked, trying to Link again" << endl;
        for (i = 0; i < impl_->shader_list_.size(); i++)
        {
            (glDetachShader(program_object_, impl_->shader_list_[i]->shader_object_));
            CHECK_GL_ERROR();
        }
    }

    for (i = 0; i < impl_->shader_list_.size(); ++i)
    {
        (glAttachShader(program_object_, impl_->shader_list_[i]->shader_object_));
        CHECK_GL_ERROR();
        //cout << "attaching ProgramObj [" << i << "] @ 0x" << hex << shader_list_[i]->program_object_ << " in ShaderObj @ 0x"  << shader_object_ << endl;
    }

    glProgramParameteri(program_object_, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE);
    CHECK_GL_ERROR();

    GLint link_status; // bugfix Oct-06-2006
    glLinkProgram(program_object_);
    CHECK_GL_ERROR();

    glGetProgramiv(program_object_, GL_LINK_STATUS, &link_status);
    CHECK_GL_ERROR();

    if (link_status)
    {
        is_linked_ = true;
        return true;
    }
    else
    {
        cout << "**Linker error**\n";
    }

    return false;
}

//----------------------------------------------------------------------------- 
// Compiler Log: Ausgabe der Compiler Meldungen in String

char* GlShader::GetLinkerLog(void)
{
    if (!g_use_glsl) return aGLSLStrings[0];

    GLint blen = 0;    // bugfix Oct-06-2006    
    GLsizei slen = 0;  // bugfix Oct-06-2006

    if (program_object_ == 0) return aGLSLStrings[2];
    (glGetProgramiv(program_object_, GL_INFO_LOG_LENGTH, &blen));
    CHECK_GL_ERROR();

    if (blen == 0)
    {
        return "";
    }

    if (linker_log != 0)
    {
        free(linker_log);
        linker_log = 0;
    }
    if ((linker_log = (GLcharARB*)malloc(blen)) == NULL)
    {
        printf("ERROR: Could not allocate compiler_log_ buffer\n");
        return aGLSLStrings[3];
    }

    (glGetProgramInfoLog(program_object_, blen, &slen, linker_log));
    CHECK_GL_ERROR();

    if (linker_log != 0)
    {
        return (char*)linker_log;
    }
    else
    {
        return aGLSLStrings[5];
    }

    return aGLSLStrings[4];
}

void GlShader::Begin(void)
{
    if (!g_use_glsl)
    {
        return;
    }

    if (program_object_ == 0)
    {
        return;
    }
    if (!_noshader)
    {
        return;
    }

    if (is_linked_)
    {
        (wsp::opengl::glproxy::UseProgram(program_object_));
    }
}

//----------------------------------------------------------------------------- 

void GlShader::End(void)
{
    if (!g_use_glsl)
    {
        return;
    }

    if (!_noshader)
    {
        return;
    }

    (wsp::opengl::glproxy::UseProgram(0));
}

//----------------------------------------------------------------------------- 

GLint GlShader::GetAttribLocation(GLchar* name)
{
    return glGetAttribLocationARB(program_object_, name);
}

//----------------------------------------------------------------------------- 
void GlShader::BindFragDataLocation(GLuint color_number, GLchar* name)
{
    (wsp::opengl::glproxy::BindFragDataLocation(program_object_, color_number, name));
    CHECK_GL_ERROR();
}

//----------------------------------------------------------------------------- 

bool GlShader::SetUniform1f(GLcharARB* varname, GLfloat v0, GLint index)
{
    if (!g_use_glsl)
    {
        return false; // GLSL not available
    }

    if (!_noshader)
    {
        return true;
    }

    GLint loc;
    if (varname)
    {
        loc = GetUniformLocation(varname);
    }
    else
    {
        loc = index;
    }

    if (loc == -1)
    {
        return false;  // can't find variable / invalid index
    }


    wsp::opengl::glproxy::Uniform1f(loc, v0);

    return true;
}

//----------------------------------------------------------------------------- 

bool GlShader::SetUniform2f(GLcharARB* varname, GLfloat v0, GLfloat v1, GLint index)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!_noshader) return true;

    GLint loc;
    if (varname)
    {
        loc = GetUniformLocation(varname);
    }
    else
    {
        loc = index;
    }

    if (loc == -1)
    {
        return false;  // can't find variable / invalid index
    }

    (wsp::opengl::glproxy::Uniform2f(loc, v0, v1));

    return true;
}

//----------------------------------------------------------------------------- 

bool GlShader::SetUniform3f(GLcharARB* varname, GLfloat v0, GLfloat v1, GLfloat v2, GLint index)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!_noshader) return true;

    GLint loc;
    if (varname)
    {
        loc = GetUniformLocation(varname);
    }
    else
    {
        loc = index;
    }

    if (loc == -1)
    {
        return false;  // can't find variable / invalid index
    }

    (wsp::opengl::glproxy::Uniform3f(loc, v0, v1, v2));

    return true;
}

//----------------------------------------------------------------------------- 

bool GlShader::SetUniform4f(GLcharARB* varname, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3, GLint index)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!_noshader) return true;

    GLint loc;
    if (varname)
    {
        loc = GetUniformLocation(varname);
    }
    else
    {
        loc = index;
    }

    if (loc == -1)
    {
        return false;  // can't find variable / invalid index
    }

    (wsp::opengl::glproxy::Uniform4f(loc, v0, v1, v2, v3));

    return true;
}

//----------------------------------------------------------------------------- 

bool GlShader::SetUniform1i(GLcharARB* varname, GLint v0, GLint index)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!_noshader) return true;

    GLint loc;
    if (varname)
    {
        loc = GetUniformLocation(varname);
    }
    else
    {
        loc = index;
    }

    if (loc == -1)
    {
        return false;  // can't find variable / invalid index
    }

    (wsp::opengl::glproxy::Uniform1i(loc, v0));

    return true;
}

//-----------------------------------------------------------------------------

bool GlShader::SetUniform2i(GLcharARB* varname, GLint v0, GLint v1, GLint index)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!_noshader) return true;

    GLint loc;
    if (varname)
        loc = GetUniformLocation(varname);
    else
        loc = index;

    if (loc == -1)
        return false;  // can't find variable / invalid index

    wsp::opengl::glproxy::Uniform2i(loc, v0, v1);


    return true;
}

//----------------------------------------------------------------------------- 

bool GlShader::SetUniform3i(GLcharARB* varname, GLint v0, GLint v1, GLint v2, GLint index)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!_noshader) return true;

    GLint loc;
    if (varname)
        loc = GetUniformLocation(varname);
    else
        loc = index;

    if (loc == -1)
        return false;  // can't find variable / invalid index

    wsp::opengl::glproxy::Uniform3i(loc, v0, v1, v2);

    return true;
}

//-----------------------------------------------------------------------------

bool GlShader::SetUniform4i(GLcharARB* varname, GLint v0, GLint v1, GLint v2, GLint v3, GLint index)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!_noshader) return true;

    GLint loc;
    if (varname)
        loc = GetUniformLocation(varname);
    else
        loc = index;

    if (loc == -1)
        return false;  // can't find variable / invalid index

    wsp::opengl::glproxy::Uniform4i(loc, v0, v1, v2, v3);

    return true;
}
//-----------------------------------------------------------------------------
//----------------------------------------------------------------------------- 

bool GlShader::SetUniform1ui(GLcharARB* varname, GLuint v0, GLint index)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!g_has_gpu_shader_model4) return false;
    if (!_noshader) return true;

    GLint loc;
    if (varname)
        loc = GetUniformLocation(varname);
    else
        loc = index;

    if (loc == -1)
        return false;  // can't find variable / invalid index

    wsp::opengl::glproxy::Uniform1ui(loc, v0);

    return true;
}

//-----------------------------------------------------------------------------

bool GlShader::SetUniform2ui(GLcharARB* varname, GLuint v0, GLuint v1, GLint index)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!g_has_gpu_shader_model4) return false;
    if (!_noshader) return true;

    GLint loc;
    if (varname)
        loc = GetUniformLocation(varname);
    else
        loc = index;

    if (loc == -1)
        return false;  // can't find variable / invalid index

    wsp::opengl::glproxy::Uniform2ui(loc, v0, v1);


    return true;
}

//----------------------------------------------------------------------------- 

bool GlShader::SetUniform3ui(GLcharARB* varname, GLuint v0, GLuint v1, GLuint v2, GLint index)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!g_has_gpu_shader_model4) return false;
    if (!_noshader) return true;

    GLint loc;
    if (varname)
        loc = GetUniformLocation(varname);
    else
        loc = index;

    if (loc == -1)
        return false;  // can't find variable / invalid index

    wsp::opengl::glproxy::Uniform3ui(loc, v0, v1, v2);

    return true;
}

//-----------------------------------------------------------------------------

bool GlShader::SetUniform4ui(GLcharARB* varname, GLuint v0, GLuint v1, GLuint v2, GLuint v3, GLint index)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!g_has_gpu_shader_model4) return false;
    if (!_noshader) return true;

    GLint loc;
    if (varname)
        loc = GetUniformLocation(varname);
    else
        loc = index;

    if (loc == -1)
        return false;  // can't find variable / invalid index

    wsp::opengl::glproxy::Uniform4ui(loc, v0, v1, v2, v3);

    return true;
}
//-----------------------------------------------------------------------------

bool GlShader::SetUniform1fv(GLcharARB* varname, GLsizei count, GLfloat *value, GLint index)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!_noshader) return true;

    GLint loc;
    if (varname)
        loc = GetUniformLocation(varname);
    else
        loc = index;

    if (loc == -1)
        return false;  // can't find variable / invalid index

    wsp::opengl::glproxy::Uniform1fv(loc, count, value);

    return true;
}
bool GlShader::SetUniform2fv(GLcharARB* varname, GLsizei count, GLfloat *value, GLint index)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!_noshader) return true;

    GLint loc;
    if (varname)
        loc = GetUniformLocation(varname);
    else
        loc = index;

    if (loc == -1)
        return false;  // can't find variable / invalid index

    wsp::opengl::glproxy::Uniform2fv(loc, count, value);

    return true;
}

//----------------------------------------------------------------------------- 

bool GlShader::SetUniform3fv(GLcharARB* varname, GLsizei count, GLfloat *value, GLint index)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!_noshader) return true;

    GLint loc;
    if (varname)
        loc = GetUniformLocation(varname);
    else
        loc = index;

    if (loc == -1)
        return false;  // can't find variable / invalid index

    wsp::opengl::glproxy::Uniform3fv(loc, count, value);

    return true;
}

//----------------------------------------------------------------------------- 

bool GlShader::SetUniform4fv(GLcharARB* varname, GLsizei count, GLfloat *value, GLint index)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!_noshader) return true;

    GLint loc;
    if (varname)
        loc = GetUniformLocation(varname);
    else
        loc = index;

    if (loc == -1)
        return false;  // can't find variable / invalid index

    wsp::opengl::glproxy::Uniform4fv(loc, count, value);

    return true;
}

//----------------------------------------------------------------------------- 

bool GlShader::SetUniform1iv(GLcharARB* varname, GLsizei count, GLint *value, GLint index)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!_noshader) return true;

    GLint loc;
    if (varname)
        loc = GetUniformLocation(varname);
    else
        loc = index;

    if (loc == -1)
        return false;  // can't find variable / invalid index

    wsp::opengl::glproxy::Uniform1iv(loc, count, value);

    return true;
}

//----------------------------------------------------------------------------- 

bool GlShader::SetUniform2iv(GLcharARB* varname, GLsizei count, GLint *value, GLint index)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!_noshader) return true;

    GLint loc;
    if (varname)
        loc = GetUniformLocation(varname);
    else
        loc = index;

    if (loc == -1)
        return false;  // can't find variable / invalid index

    wsp::opengl::glproxy::Uniform2iv(loc, count, value);

    return true;
}

//----------------------------------------------------------------------------- 

bool GlShader::SetUniform3iv(GLcharARB* varname, GLsizei count, GLint *value, GLint index)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!_noshader) return true;

    GLint loc;
    if (varname)
        loc = GetUniformLocation(varname);
    else
        loc = index;

    if (loc == -1)
        return false;  // can't find variable / invalid index

    wsp::opengl::glproxy::Uniform3iv(loc, count, value);

    return true;
}

//----------------------------------------------------------------------------- 

bool GlShader::SetUniform4iv(GLcharARB* varname, GLsizei count, GLint *value, GLint index)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!_noshader) return true;

    GLint loc;
    if (varname)
        loc = GetUniformLocation(varname);
    else
        loc = index;

    if (loc == -1)
        return false;  // can't find variable / invalid index

    wsp::opengl::glproxy::Uniform4iv(loc, count, value);

    return true;
}

//----------------------------------------------------------------------------- 

bool GlShader::SetUniform1uiv(GLcharARB* varname, GLsizei count, GLuint *value, GLint index)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!g_has_gpu_shader_model4) return false;
    if (!_noshader) return true;

    GLint loc;
    if (varname)
        loc = GetUniformLocation(varname);
    else
        loc = index;

    if (loc == -1)
        return false;  // can't find variable / invalid index

    wsp::opengl::glproxy::Uniform1uiv(loc, count, value);

    return true;
}

//----------------------------------------------------------------------------- 

bool GlShader::SetUniform2uiv(GLcharARB* varname, GLsizei count, GLuint *value, GLint index)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!g_has_gpu_shader_model4) return false;
    if (!_noshader) return true;

    GLint loc;
    if (varname)
        loc = GetUniformLocation(varname);
    else
        loc = index;

    if (loc == -1)
        return false;  // can't find variable / invalid index

    wsp::opengl::glproxy::Uniform2uiv(loc, count, value);

    return true;
}

//----------------------------------------------------------------------------- 

bool GlShader::SetUniform3uiv(GLcharARB* varname, GLsizei count, GLuint *value, GLint index)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!g_has_gpu_shader_model4) return false;
    if (!_noshader) return true;

    GLint loc;
    if (varname)
        loc = GetUniformLocation(varname);
    else
        loc = index;

    if (loc == -1)
        return false;  // can't find variable / invalid index

    wsp::opengl::glproxy::Uniform3uiv(loc, count, value);

    return true;
}

//----------------------------------------------------------------------------- 

bool GlShader::SetUniform4uiv(GLcharARB* varname, GLsizei count, GLuint *value, GLint index)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!g_has_gpu_shader_model4) return false;
    if (!_noshader) return true;

    GLint loc;
    if (varname)
        loc = GetUniformLocation(varname);
    else
        loc = index;

    if (loc == -1)
        return false;  // can't find variable / invalid index

    wsp::opengl::glproxy::Uniform4uiv(loc, count, value);

    return true;
}

//----------------------------------------------------------------------------- 

bool GlShader::SetUniformMatrix2fv(GLcharARB* varname, GLsizei count, GLboolean transpose, GLfloat *value, GLint index)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!_noshader) return true;

    GLint loc;
    if (varname)
        loc = GetUniformLocation(varname);
    else
        loc = index;

    if (loc == -1)
        return false;  // can't find variable / invalid index

    wsp::opengl::glproxy::UniformMatrix2fv(loc, count, transpose, value);

    return true;
}

//----------------------------------------------------------------------------- 

bool GlShader::SetUniformMatrix3f(GLcharARB* varname, GLboolean transpose, GLfloat *value, GLint index)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!_noshader) return true;

    GLint loc;
    if (varname)
        loc = GetUniformLocation(varname);
    else
        loc = index;

    if (loc == -1)
        return false;  // can't find variable / invalid index

    wsp::opengl::glproxy::UniformMatrix3f(loc, transpose, value);

    return true;
}

bool GlShader::SetUniformMatrix3fv(GLcharARB* varname, GLsizei count, GLboolean transpose, GLfloat *value, GLint index)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!_noshader) return true;

    GLint loc;
    if (varname)
        loc = GetUniformLocation(varname);
    else
        loc = index;

    if (loc == -1)
        return false;  // can't find variable / invalid index

    wsp::opengl::glproxy::UniformMatrix3fv(loc, count, transpose, value);

    return true;
}

//----------------------------------------------------------------------------- 


bool GlShader::SetUniformMatrix4f(GLcharARB* varname, GLboolean transpose, GLfloat *value, GLint index)
{
    if (!g_use_glsl)
    {
        return false; // GLSL not available
    }
    if (!_noshader)
    {
        return true;
    }

    GLint loc;
    if (varname)
    {
        loc = GetUniformLocation(varname);
    }
    else
    {
        loc = index;
    }

    if (loc == -1)
    {
        return false;  // can't find variable / invalid index
    }

    wsp::opengl::glproxy::UniformMatrix4f(loc, transpose, value);

    return true;
}

bool GlShader::SetUniformMatrix4fv(GLcharARB* varname, GLsizei count, GLboolean transpose, GLfloat *value, GLint index)
{
    if (!g_use_glsl)
    {
        return false; // GLSL not available
    }
    if (!_noshader)
    {
        return true;
    }

    GLint loc;
    if (varname)
    {
        loc = GetUniformLocation(varname);
    }
    else
    {
        loc = index;
    }

    if (loc == -1)
    {
        return false;  // can't find variable / invalid index
    }

    wsp::opengl::glproxy::UniformMatrix4fv(loc, count, transpose, value);

    return true;
}

//----------------------------------------------------------------------------- 

GLint GlShader::GetUniformLocation(const GLcharARB *name)
{
    GLint loc;
    loc = glGetUniformLocation(program_object_, name);
    CHECK_GL_ERROR();
    return loc;
}

//----------------------------------------------------------------------------- 

void GlShader::GetUniformfv(GLcharARB* varname, GLfloat* values, GLint index)
{
    if (!g_use_glsl) return;

    GLint loc;
    if (varname)
        loc = GetUniformLocation(varname);
    else
        loc = index;

    if (loc == -1)
        return;  // can't find variable / invalid index

    glGetUniformfv(program_object_, loc, values);

}

//----------------------------------------------------------------------------- 

void GlShader::GetUniformiv(GLcharARB* varname, GLint* values, GLint index)
{
    if (!g_use_glsl) return;

    GLint loc;
    if (varname)
        loc = GetUniformLocation(varname);
    else
        loc = index;

    if (loc == -1)
        return;  // can't find variable / invalid index

    glGetUniformiv(program_object_, loc, values);

}

//----------------------------------------------------------------------------- 

void GlShader::GetUniformuiv(GLcharARB* varname, GLuint* values, GLint index)
{
    if (!g_use_glsl) return;

    GLint loc;
    if (varname)
        loc = GetUniformLocation(varname);
    else
        loc = index;

    if (loc == -1)
        return;  // can't find variable / invalid index

    glGetUniformuivEXT(program_object_, loc, values);

}

//-----------------------------------------------------------------------------
void  GlShader::BindAttribLocation(GLint index, GLchar* name)
{
    wsp::opengl::glproxy::BindAttribLocation(program_object_, index, name);
    CHECK_GL_ERROR();
}

//-----------------------------------------------------------------------------


bool GlShader::SetVertexAttrib1f(GLuint index, GLfloat v0)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!_noshader) return true;

    wsp::opengl::glproxy::VertexAttrib1f(index, v0);

    return true;
}

bool GlShader::SetVertexAttrib2f(GLuint index, GLfloat v0, GLfloat v1)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!_noshader) return true;

    wsp::opengl::glproxy::VertexAttrib2f(index, v0, v1);

    return true;
}

bool GlShader::SetVertexAttrib3f(GLuint index, GLfloat v0, GLfloat v1, GLfloat v2)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!_noshader) return true;

    wsp::opengl::glproxy::VertexAttrib3f(index, v0, v1, v2);

    return true;
}

bool GlShader::SetVertexAttrib4f(GLuint index, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!_noshader) return true;

    wsp::opengl::glproxy::VertexAttrib4f(index, v0, v1, v2, v3);

    return true;
}

//-----------------------------------------------------------------------------

bool GlShader::SetVertexAttrib1d(GLuint index, GLdouble v0)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!_noshader) return true;

    wsp::opengl::glproxy::VertexAttrib1d(index, v0);

    return true;
}

//-----------------------------------------------------------------------------

bool GlShader::SetVertexAttrib2d(GLuint index, GLdouble v0, GLdouble v1)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!_noshader) return true;

    wsp::opengl::glproxy::VertexAttrib2d(index, v0, v1);

    return true;
}

//-----------------------------------------------------------------------------
bool GlShader::SetVertexAttrib3d(GLuint index, GLdouble v0, GLdouble v1, GLdouble v2)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!_noshader) return true;

    wsp::opengl::glproxy::VertexAttrib3d(index, v0, v1, v2);

    return true;
}
//-----------------------------------------------------------------------------
bool GlShader::SetVertexAttrib4d(GLuint index, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!_noshader) return true;

    wsp::opengl::glproxy::VertexAttrib4d(index, v0, v1, v2, v3);

    return true;
}

//-----------------------------------------------------------------------------

bool GlShader::SetVertexAttrib1s(GLuint index, GLshort v0)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!_noshader) return true;

    wsp::opengl::glproxy::VertexAttrib1s(index, v0);

    return true;
}

//-----------------------------------------------------------------------------

bool GlShader::SetVertexAttrib2s(GLuint index, GLshort v0, GLshort v1)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!_noshader) return true;

    wsp::opengl::glproxy::VertexAttrib2s(index, v0, v1);

    return true;
}

//-----------------------------------------------------------------------------
bool GlShader::SetVertexAttrib3s(GLuint index, GLshort v0, GLshort v1, GLshort v2)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!_noshader) return true;

    wsp::opengl::glproxy::VertexAttrib3s(index, v0, v1, v2);

    return true;
}
//-----------------------------------------------------------------------------
bool GlShader::SetVertexAttrib4s(GLuint index, GLshort v0, GLshort v1, GLshort v2, GLshort v3)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!_noshader) return true;

    wsp::opengl::glproxy::VertexAttrib4s(index, v0, v1, v2, v3);

    return true;
}
//----------------------------------------------------------------------------
bool GlShader::SetVertexAttribNormalizedByte(GLuint index, GLbyte v0, GLbyte v1, GLbyte v2, GLbyte v3)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!_noshader) return true;

    wsp::opengl::glproxy::VertexAttrib4Nub(index, v0, v1, v2, v3);

    return true;
}
//-----------------------------------------------------------------------------

bool GlShader::SetVertexAttrib1i(GLuint index, GLint v0)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!g_has_gpu_shader_model4) return false;
    if (!_noshader) return true;

    wsp::opengl::glproxy::VertexAttribI1i(index, v0);

    return true;
}

//-----------------------------------------------------------------------------

bool GlShader::SetVertexAttrib2i(GLuint index, GLint v0, GLint v1)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!g_has_gpu_shader_model4) return false;
    if (!_noshader) return true;

    wsp::opengl::glproxy::VertexAttribI2i(index, v0, v1);

    return true;
}

//-----------------------------------------------------------------------------
bool GlShader::SetVertexAttrib3i(GLuint index, GLint v0, GLint v1, GLint v2)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!g_has_gpu_shader_model4) return false;
    if (!_noshader) return true;

    wsp::opengl::glproxy::VertexAttribI3i(index, v0, v1, v2);

    return true;
}
//-----------------------------------------------------------------------------
bool GlShader::SetVertexAttrib4i(GLuint index, GLint v0, GLint v1, GLint v2, GLint v3)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!g_has_gpu_shader_model4) return false;
    if (!_noshader) return true;

    wsp::opengl::glproxy::VertexAttribI4i(index, v0, v1, v2, v3);

    return true;
}
//-----------------------------------------------------------------------------
bool GlShader::SetVertexAttrib1ui(GLuint index, GLuint v0)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!g_has_gpu_shader_model4) return false;
    if (!_noshader) return true;

    wsp::opengl::glproxy::VertexAttribI1ui(index, v0);

    return true;
}

//-----------------------------------------------------------------------------

bool GlShader::SetVertexAttrib2ui(GLuint index, GLuint v0, GLuint v1)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!g_has_gpu_shader_model4) return false;
    if (!_noshader) return true;

    wsp::opengl::glproxy::VertexAttribI2ui(index, v0, v1);

    return true;
}

//-----------------------------------------------------------------------------
bool GlShader::SetVertexAttrib3ui(GLuint index, GLuint v0, GLuint v1, GLuint v2)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!g_has_gpu_shader_model4) return false;
    if (!_noshader) return true;

    wsp::opengl::glproxy::VertexAttribI3ui(index, v0, v1, v2);

    return true;
}
//-----------------------------------------------------------------------------
bool GlShader::SetVertexAttrib4ui(GLuint index, GLuint v0, GLuint v1, GLuint v2, GLuint v3)
{
    if (!g_use_glsl) return false; // GLSL not available
    if (!g_has_gpu_shader_model4) return false;
    if (!_noshader) return true;

    wsp::opengl::glproxy::VertexAttribI4ui(index, v0, v1, v2, v3);

    return true;
}

GLuint GlShader::GetUniformBlockIndex(GLcharARB* uniform_block_name)
{
    return glGetUniformBlockIndex(program_object_, uniform_block_name);
}

void GlShader::UniformBlockBinding(GLuint uniform_block_index, GLuint uniform_block_binding)
{
    wsp::opengl::glproxy::UniformBlockBinding(program_object_, uniform_block_index, uniform_block_binding);
}

//-----------------------------------------------------------------------------
// ************************************************************************
// Shader Program : Manage Shader Programs (Vertex/Fragment)
// ************************************************************************
GlShaderObject::GlShaderObject()
    : shader_object_(0)
    , shader_source_(0)
    , compiler_log_(0)
    , is_compiled_(false)
    , program_type_(0)
    , memalloc_(false)
{
    InitOpenGLExtensions();
}

//----------------------------------------------------------------------------- 
GlShaderObject::~GlShaderObject()
{
    if (compiler_log_ != 0)
    {
        free(compiler_log_);
    }

    if (shader_source_ != 0)
    {
        if (memalloc_)
        {
            delete[] shader_source_;  // free ASCII Source
        }
    }

    if (shader_object_ != 0)
    {
        glDeleteShader(shader_object_);
        CHECK_GL_ERROR();
    }
}

//----------------------------------------------------------------------------- 
unsigned long getFileLength(ifstream& file)
{
    if (!file.good())
    {
        return 0;
    }

    unsigned long pos = file.tellg();
    WSP_UNUSED(pos);
    file.seekg(0, ios::end);
    unsigned long len = file.tellg();
    file.seekg(ios::beg);

    return len;
}

//----------------------------------------------------------------------------- 
void GlShaderObject::LoadFromMemory(const char* program)
{
    if (shader_source_ != 0)    // there is already a source Loaded, free it!
    {
        if (memalloc_)
        {
            delete[] shader_source_;
        }
    }

    memalloc_ = false;
    shader_source_ = (GLubyte*)program;
}


// ----------------------------------------------------------------------------
// Compiler Log: Ausgabe der Compiler Meldungen in String
char* GlShaderObject::GetCompilerLog(void)
{
    if (!g_use_glsl)
    {
        return aGLSLStrings[0];
    }

    if (shader_object_ == 0)
    {
        return aGLSLStrings[1]; // not a valid program object
    }

    GLint blen = 0;
    GLsizei slen = 0;
    glGetShaderiv(shader_object_, GL_INFO_LOG_LENGTH, &blen);
    CHECK_GL_ERROR();

    if (blen == 0)
    {
        return "";
    }

    if (compiler_log_ != 0)
    {
        free(compiler_log_);
        compiler_log_ = 0;
    }
    if ((compiler_log_ = (GLcharARB*)malloc(blen)) == NULL)
    {
        printf("ERROR: Could not allocate compiler_log_ buffer\n");
        return aGLSLStrings[3];
    }

    glGetInfoLogARB(shader_object_, blen, &slen, compiler_log_);
    CHECK_GL_ERROR();

    if (compiler_log_ != 0)
    {
        return (char*)compiler_log_;
    }
    else
    {
        return aGLSLStrings[6];
    }

    return aGLSLStrings[4];
}

// ----------------------------------------------------------------------------
bool GlShaderObject::Compile(void)
{
    if (!g_use_glsl)
    {
        return false;
    }

    is_compiled_ = false;

    GLint Compiled = 0;

    if (shader_source_ == 0) return false;

    GLint    length = (GLint)strlen((const char*)shader_source_);
    glShaderSourceARB(shader_object_, 1, (const GLcharARB **)&shader_source_, &length);
    CHECK_GL_ERROR();

    glCompileShaderARB(shader_object_);
    CHECK_GL_ERROR();
    glGetObjectParameterivARB(shader_object_, GL_COMPILE_STATUS, &Compiled);
    CHECK_GL_ERROR();

    if (Compiled)
    {
        is_compiled_ = true;
    }

    return is_compiled_;
}

// ----------------------------------------------------------------------------
GLint GlShaderObject::GetAttribLocation(char* attribName)
{
    return glGetAttribLocationARB(shader_object_, attribName);
}

// ----------------------------------------------------------------------------
VertexShader::VertexShader()
{
    program_type_ = 1;
    if (g_use_glsl)
    {
        shader_object_ = glCreateShader(GL_VERTEX_SHADER_ARB);
        CHECK_GL_ERROR();
    }
}

// ----------------------------------------------------
VertexShader::~VertexShader()
{
}
// ----------------------------------------------------

FragmentShader::FragmentShader()
{
    program_type_ = 2;
    if (g_use_glsl)
    {
        shader_object_ = glCreateShader(GL_FRAGMENT_SHADER_ARB);
        CHECK_GL_ERROR();
    }
}

// ----------------------------------------------------

FragmentShader::~FragmentShader()
{
}


// ----------------------------------------------------

GeometryShader::GeometryShader()
{
    program_type_ = 3;
    if (g_use_glsl && g_is_geometry_shader_supported)
    {
        shader_object_ = glCreateShader(GL_GEOMETRY_SHADER_EXT);
        CHECK_GL_ERROR();
    }
}

// ----------------------------------------------------

GeometryShader::~GeometryShader()
{
}

// ----------------------------------------------------------------------------
// ShaderManager: Easy use of (multiple) Shaders

class GlShaderManager::Impl
{
public:
	std::vector<GlShader*>  shader_obj_list_;
};

GlShaderManager::GlShaderManager()
	: impl_(WSP_NEW Impl())
{
    InitOpenGLExtensions();
    input_primitive_type_ = GL_TRIANGLES;
    output_primitive_type_ = GL_TRIANGLE_STRIP;
    max_number_of_output_vertices_ = 3;

}

GlShaderManager::~GlShaderManager()
{
    // free objects
    for (GlShader* shader : impl_->shader_obj_list_)
    {
        delete shader;
    }
	impl_->shader_obj_list_.clear();

	delete impl_;
}

std::vector<GlShader*>& GlShaderManager::shader_obj_list()
{
	return impl_->shader_obj_list_;
}

const std::vector<GlShader*>& GlShaderManager::shader_obj_list() const
{
	return impl_->shader_obj_list_;
}

// ----------------------------------------------------------------------------

void GlShaderManager::SetInputPrimitiveType(int nInputPrimitiveType)
{
    input_primitive_type_ = nInputPrimitiveType;

}

void GlShaderManager::SetOutputPrimitiveType(int nOutputPrimitiveType)
{
    output_primitive_type_ = nOutputPrimitiveType;

}

void GlShaderManager::SetVerticesOut(int nVerticesOut)
{
    max_number_of_output_vertices_ = nVerticesOut;
}

// ----------------------------------------------------------------------------

void PrintCompileLog(GlShaderObject* shader)
{
    const char* log = shader->GetCompilerLog();
    if (strlen(log) > 0)
    {
        cout << "GLSL Compiler Log (Vertex Shader):\n";
        cout << log << "\n";
    }
}

void PrintLinkerLog(GlShader* shader)
{
    const char* log = shader->GetLinkerLog();
    if (strlen(log) > 0)
    {
        cout << "GLSL Linker Log:\n";
        cout << log << "\n";
    }
}

// ----------------------------------------------------------------------------

GlShader* GlShaderManager::LoadFromMemory(const char* vertex_memory, const char* fragment_memory)
{
    return LoadFromMemory(vertex_memory, nullptr, fragment_memory);
}

GlShader* LoadFromMemoryImpl(
    const char* vertex_memory,
    const char* geometry_memory,
    const char* fragment_memory)
{
    GlShader* shader = WSP_NEW GlShader();
    if (geometry_memory != nullptr)
    {
        shader->UsesGeometryShader(true);
        //shader->SetInputPrimitiveType(input_primitive_type_);
        //shader->SetOutputPrimitiveType(output_primitive_type_);
        //shader->SetVerticesOut(max_number_of_output_vertices_);
    }
    else
    {
        shader->UsesGeometryShader(false);
    }

    // get vertex program
    if (vertex_memory != 0)
    {
        VertexShader* vertex_shader = WSP_NEW VertexShader;
        vertex_shader->LoadFromMemory(vertex_memory);

        if (!vertex_shader->Compile())
        {
            cout << "***COMPILER ERROR (Vertex Shader):\n";
            cout << vertex_shader->GetCompilerLog() << endl;
            {
                std::string shader_code_log;
                AppendLineNumbersToText(&shader_code_log, vertex_memory);
                cout << shader_code_log.c_str() << endl;
            }

            delete shader;
            delete vertex_shader;
            return 0;
        }

        PrintCompileLog(vertex_shader);
        shader->AddShader(vertex_shader);
    }

    // get geometry program
    if (geometry_memory != 0)
    {
        GeometryShader* geometry_shader = WSP_NEW GeometryShader;
        geometry_shader->LoadFromMemory(geometry_memory);

        if (!geometry_shader->Compile())
        {
            cout << "***COMPILER ERROR (Geometry Shader):\n";
            cout << geometry_shader->GetCompilerLog() << endl;

            {
                std::string shader_code_log;
                AppendLineNumbersToText(&shader_code_log, geometry_memory);
                cout << shader_code_log.c_str() << endl;
            }

            delete shader;
            delete geometry_shader;
            return 0;
        }

        PrintCompileLog(geometry_shader);
        shader->AddShader(geometry_shader);
    }

    // get fragment program
    if (fragment_memory != 0)
    {
        FragmentShader* fragment_shader = WSP_NEW FragmentShader;
        fragment_shader->LoadFromMemory(fragment_memory);

        if (!fragment_shader->Compile())
        {
            cout << "***COMPILER ERROR (Fragment Shader):\n";
            cout << fragment_shader->GetCompilerLog() << endl;
            {
                std::string shader_code_log;
                AppendLineNumbersToText(&shader_code_log, fragment_memory);
                cout << shader_code_log.c_str() << endl;
            }

            delete shader;
            delete fragment_shader;
            return 0;
        }

        PrintCompileLog(fragment_shader);
        shader->AddShader(fragment_shader);
    }

    // Link 
    if (!shader->Link())
    {
        cout << "**LINKER ERROR\n";
        cout << shader->GetLinkerLog() << endl;

        {
            std::string shader_code_log;
            AppendLineNumbersToText(&shader_code_log, vertex_memory);
            cout << "========= VERTEX =========\n" << shader_code_log.c_str() << "\n\n";
        }

        {
            std::string shader_code_log;
            AppendLineNumbersToText(&shader_code_log, geometry_memory);
            cout << "========= GEOMETRY =========\n" << shader_code_log.c_str() << "\n\n";
        }

        {
            std::string shader_code_log;
            AppendLineNumbersToText(&shader_code_log, fragment_memory);
            cout << "========= FRAGMENT =========\n" << shader_code_log.c_str() << "\n\n";
        }
        delete shader;
        return 0;
    }

    PrintLinkerLog(shader);

    return shader;
}

GlShader* GlShaderManager::LoadFromMemory(
    const char* vertex_memory,
    const char* geometry_memory,
    const char* fragment_memory)
{
    GlShader* shader = LoadFromMemoryImpl(
        vertex_memory,
        geometry_memory,
        fragment_memory);
    if (shader != nullptr)
    {
        impl_->shader_obj_list_.push_back(shader);
    }
    return shader;
}

GlShader* GlShaderManager::LoadFromBinary(const GlShaderBinaryFile& file)
{
    GlShader* shader = WSP_NEW GlShader();
    GLuint shader_program = shader->GetProgramObject();

    GLint binary_format_count;
    glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &binary_format_count);
    std::vector<int> formats;
    formats.resize(binary_format_count);
    glGetIntegerv(GL_PROGRAM_BINARY_FORMATS, &formats[0]);
    printf("supported binary formats:\n");
    for (int i = 0; i < binary_format_count; ++i)
    {
        printf("  %d\n", formats[i]);
    }

    glProgramParameteri(shader_program, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE);
    CHECK_GL_ERROR();

    printf("input binary format is %d\n", file.header.binary_format);
    glProgramBinary(shader_program, file.header.binary_format, file.shader_binary, file.header.binary_size);
    CHECK_GL_ERROR();

    GLint link_status;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &link_status);
    CHECK_GL_ERROR();
    if (link_status == GL_FALSE)
    {
        cout << "**LINKER ERROR\n";
        cout << shader->GetLinkerLog() << endl;
        shader->is_linked_ = false;
        return nullptr;
    }

    shader->is_linked_ = true;
	impl_->shader_obj_list_.push_back(shader);
    return shader;
}

// ----------------------------------------------------------------------------

bool GlShaderManager::Free(GlShader* shader)
{
    for (vector<GlShader*>::iterator iter = impl_->shader_obj_list_.begin();
        iter != impl_->shader_obj_list_.end(); ++iter)
    {
        if ((*iter) == shader)
        {
			impl_->shader_obj_list_.erase(iter);
            delete shader;
            return true;
        }
    }

    return false;
}



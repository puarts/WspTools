#pragma once

#include "glsl_compiler.h"

//! \defgroup GLSL libglsl
//#include "glslSettings.h"
#include <vector>
#include <iostream>
#include <wsp/common/_define_commoncore.h>
//#define GLEW_STATIC 

#include <GL/glew.h>

#define GLSLAPI    // static build

// GL ERROR CHECK
namespace wsp {
    namespace opengl {
        WSP_DLL_EXPORT int CheckGLError(char *file, int line);
    }
}

#define WSP_CHECK_GL_ERROR() wsp::opengl::CheckGLError(__FILE__, __LINE__)

#define WSP_ASSERT_GL_ERROR() WSP_ASSERT(wsp::opengl::CheckGLError(__FILE__, __LINE__) == 0, "GL error")

namespace wsp {
    namespace opengl {

        class GlShaderManager;

        //! Shader Object holds the Shader Source, the OpenGL "Shader Object" and provides some methods to load shaders.
        /*!
              \author Martin Christen
              \ingroup GLSL
              \bug This class should be an interface. (pure virtual)
              */
        class WSP_DLL_EXPORT GlShaderObject
        {
            friend class GlShader;

        public:
            GlShaderObject();
            virtual ~GlShaderObject();

            void        LoadFromMemory(const char* source);      //!< \brief Load program from null-terminated char array. \param program Address of the memory containing the shader program.

            bool        Compile(void);                            //!< compile program
            char*       GetCompilerLog(void);                     //!< get compiler messages
            GLint       GetAttribLocation(char* attribName);      //!< \brief Retrieve attribute location. \return Returns attribute location. \param attribName Specify attribute name.  

        protected:
            int        program_type_;  //!< The program type. 1=Vertex Program, 2=Fragment Program, 3=Geometry Progam, 0=none

            GLuint     shader_object_;  //!< Shader Object
            GLubyte*   shader_source_;  //!< ASCII Source-Code

            GLcharARB* compiler_log_;

            bool       is_compiled_;   //!< true if compiled
            bool       memalloc_;     //!< true if memory for shader source was allocated
        };

        //-----------------------------------------------------------------------------

        //! \brief Class holding Vertex Shader \ingroup GLSL \author Martin Christen
        class WSP_DLL_EXPORT VertexShader : public GlShaderObject
        {
        public:
            VertexShader();  //!< Constructor for Vertex Shader
            virtual     ~VertexShader() override;
        };

        //-----------------------------------------------------------------------------

        //! \brief Class holding Fragment Shader \ingroup GLSL \author Martin Christen
        class WSP_DLL_EXPORT FragmentShader : public GlShaderObject
        {
        public:
            FragmentShader(); //!< Constructor for Fragment Shader
            virtual     ~FragmentShader() override;

        };

        //-----------------------------------------------------------------------------

        //! \brief Class holding Geometry Shader \ingroup GLSL \author Martin Christen
        class WSP_DLL_EXPORT GeometryShader : public GlShaderObject
        {
        public:
            GeometryShader(); //!< Constructor for Geometry Shader
            virtual     ~GeometryShader() override;
        };

        //-----------------------------------------------------------------------------

        //! \brief Controlling compiled and linked GLSL program. \ingroup GLSL \author Martin Christen
        class WSP_DLL_EXPORT GlShader
        {
            friend class GlShaderManager;

        public:
            GlShader();
            virtual    ~GlShader();
            void       AddShader(GlShaderObject* ShaderProgram); //!< add a Vertex or Fragment Program \param ShaderProgram The shader object.

            //!< Returns the OpenGL Program Object (only needed if you want to control everything yourself) \return The OpenGL Program Object
            GLuint     GetProgramObject() const { return program_object_; }

            bool       Link(void);                        //!< Link all Shaders
            char*      GetLinkerLog(void);                //!< Get Linker Messages \return char pointer to linker messages. Memory of this string is available until you link again or destroy this class.

            void       Begin();                           //!< use Shader. OpenGL calls will go through vertex, geometry and/or fragment shaders.
            void       End();                             //!< Stop using this shader. OpenGL calls will go through regular pipeline.

            // Geometry Shader: Input Type, Output and Number of Vertices out
            void       SetInputPrimitiveType(int nInputPrimitiveType);   //!< Set the input primitive type for the geometry shader
            void       SetOutputPrimitiveType(int nOutputPrimitiveType); //!< Set the output primitive type for the geometry shader
            void       SetVerticesOut(int nVerticesOut);                 //!< Set the maximal number of vertices the geometry shader can output

            GLint       GetUniformLocation(const GLcharARB *name);  //!< Retrieve Location (index) of a Uniform Variable

            // Submitting Uniform Variables. You can set varname to 0 and specifiy index retrieved with GetUniformLocation (best performance)
            bool       SetUniform1f(GLcharARB* varname, GLfloat v0, GLint index = -1);  //!< Specify value of uniform variable. \param varname The name of the uniform variable.
            bool       SetUniform2f(GLcharARB* varname, GLfloat v0, GLfloat v1, GLint index = -1);  //!< Specify value of uniform variable. \param varname The name of the uniform variable.
            bool       SetUniform3f(GLcharARB* varname, GLfloat v0, GLfloat v1, GLfloat v2, GLint index = -1);  //!< Specify value of uniform variable. \param varname The name of the uniform variable.
            bool       SetUniform4f(GLcharARB* varname, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3, GLint index = -1);  //!< Specify value of uniform variable. \param varname The name of the uniform variable.

            bool       SetUniform1i(GLcharARB* varname, GLint v0, GLint index = -1);  //!< Specify value of uniform integer variable. \param varname The name of the uniform variable.
            bool       SetUniform2i(GLcharARB* varname, GLint v0, GLint v1, GLint index = -1); //!< Specify value of uniform integer variable. \param varname The name of the uniform variable.
            bool       SetUniform3i(GLcharARB* varname, GLint v0, GLint v1, GLint v2, GLint index = -1); //!< Specify value of uniform integer variable. \param varname The name of the uniform variable.
            bool       SetUniform4i(GLcharARB* varname, GLint v0, GLint v1, GLint v2, GLint v3, GLint index = -1); //!< Specify value of uniform integer variable. \param varname The name of the uniform variable.

            // Note: unsigned integers require GL_EXT_gpu_shader4 (for example GeForce 8800)
            bool       SetUniform1ui(GLcharARB* varname, GLuint v0, GLint index = -1); //!< Specify value of uniform unsigned integer variable. \warning Requires GL_EXT_gpu_shader4. \param varname The name of the uniform variable.
            bool       SetUniform2ui(GLcharARB* varname, GLuint v0, GLuint v1, GLint index = -1); //!< Specify value of uniform unsigned integer variable. \warning Requires GL_EXT_gpu_shader4. \param varname The name of the uniform variable.
            bool       SetUniform3ui(GLcharARB* varname, GLuint v0, GLuint v1, GLuint v2, GLint index = -1); //!< Specify value of uniform unsigned integer variable. \warning Requires GL_EXT_gpu_shader4. \param varname The name of the uniform variable.
            bool       SetUniform4ui(GLcharARB* varname, GLuint v0, GLuint v1, GLuint v2, GLuint v3, GLint index = -1); //!< Specify value of uniform unsigned integer variable. \warning Requires GL_EXT_gpu_shader4. \param varname The name of the uniform variable.

            // Arrays
            bool       SetUniform1fv(GLcharARB* varname, GLsizei count, GLfloat *value, GLint index = -1); //!< Specify values of uniform array. \param varname The name of the uniform variable.
            bool       SetUniform2fv(GLcharARB* varname, GLsizei count, GLfloat *value, GLint index = -1); //!< Specify values of uniform array. \param varname The name of the uniform variable.
            bool       SetUniform3fv(GLcharARB* varname, GLsizei count, GLfloat *value, GLint index = -1); //!< Specify values of uniform array. \param varname The name of the uniform variable.
            bool       SetUniform4fv(GLcharARB* varname, GLsizei count, GLfloat *value, GLint index = -1); //!< Specify values of uniform array. \param varname The name of the uniform variable.

            bool       SetUniform1iv(GLcharARB* varname, GLsizei count, GLint *value, GLint index = -1); //!< Specify values of uniform array. \param varname The name of the uniform variable.
            bool       SetUniform2iv(GLcharARB* varname, GLsizei count, GLint *value, GLint index = -1); //!< Specify values of uniform array. \param varname The name of the uniform variable.
            bool       SetUniform3iv(GLcharARB* varname, GLsizei count, GLint *value, GLint index = -1); //!< Specify values of uniform array. \param varname The name of the uniform variable.
            bool       SetUniform4iv(GLcharARB* varname, GLsizei count, GLint *value, GLint index = -1); //!< Specify values of uniform array. \param varname The name of the uniform variable.

            bool       SetUniform1uiv(GLcharARB* varname, GLsizei count, GLuint *value, GLint index = -1); //!< Specify values of uniform array. \warning Requires GL_EXT_gpu_shader4. \param varname The name of the uniform variable.
            bool       SetUniform2uiv(GLcharARB* varname, GLsizei count, GLuint *value, GLint index = -1); //!< Specify values of uniform array. \warning Requires GL_EXT_gpu_shader4. \param varname The name of the uniform variable.
            bool       SetUniform3uiv(GLcharARB* varname, GLsizei count, GLuint *value, GLint index = -1); //!< Specify values of uniform array. \warning Requires GL_EXT_gpu_shader4. \param varname The name of the uniform variable.
            bool       SetUniform4uiv(GLcharARB* varname, GLsizei count, GLuint *value, GLint index = -1); //!< Specify values of uniform array. \warning Requires GL_EXT_gpu_shader4. \param varname The name of the uniform variable.

            bool       SetUniformMatrix2fv(GLcharARB* varname, GLsizei count, GLboolean transpose, GLfloat *value, GLint index = -1); //!< Specify values of uniform 2x2 matrix. \param varname The name of the uniform variable.
            bool       SetUniformMatrix3f(GLcharARB* varname, GLboolean transpose, GLfloat *value, GLint index = -1); //!< Specify values of uniform 3x3 matrix. \param varname The name of the uniform variable.
            bool       SetUniformMatrix3fv(GLcharARB* varname, GLsizei count, GLboolean transpose, GLfloat *value, GLint index = -1); //!< Specify values of uniform 3x3 matrix. \param varname The name of the uniform variable.
            bool       SetUniformMatrix4f(GLcharARB* varname, GLboolean transpose, GLfloat *value, GLint index = -1); //!< Specify values of uniform 4x4 matrix. \param varname The name of the uniform variable.
            bool       SetUniformMatrix4fv(GLcharARB* varname, GLsizei count, GLboolean transpose, GLfloat *value, GLint index = -1); //!< Specify values of uniform 4x4 matrix. \param varname The name of the uniform variable.

            // Receive Uniform variables:
            void       GetUniformfv(GLcharARB* varname, GLfloat* values, GLint index = -1); //!< Receive value of uniform variable. \param varname The name of the uniform variable.
            void       GetUniformiv(GLcharARB* varname, GLint* values, GLint index = -1); //!< Receive value of uniform variable. \param varname The name of the uniform variable.
            void       GetUniformuiv(GLcharARB* varname, GLuint* values, GLint index = -1); //!< Receive value of uniform variable. \warning Requires GL_EXT_gpu_shader4 \param varname The name of the uniform variable.

            GLuint GetUniformBlockIndex(GLcharARB* uniform_block_name);
            void UniformBlockBinding(GLuint uniform_block_index, GLuint uniform_block_binding);

            /*! This method simply calls glBindAttribLocation for the current program_object_
            \warning NVidia implementation is different than the GLSL standard: GLSL attempts to eliminate aliasing of vertex attributes but this is integral to NVIDIA�s hardware approach and necessary for maintaining compatibility with existing OpenGL applications that NVIDIA customers rely on. NVIDIA�s GLSL implementation therefore does not allow built-in vertex attributes to collide with a generic vertex attributes that is assigned to a particular vertex  attribute index with glBindAttribLocation. For example, you should not use gl_Normal (a built-in vertex attribute) and also use glBindAttribLocation to bind a generic vertex attribute named "whatever" to vertex attribute index 2 because gl_Normal aliases to index 2.
            \verbatim
            gl_Vertex                0
            gl_Normal                2
            gl_Color                 3
            gl_SecondaryColor        4
            gl_FogCoord              5
            gl_MultiTexCoord0        8
            gl_MultiTexCoord1        9
            gl_MultiTexCoord2       10
            gl_MultiTexCoord3       11
            gl_MultiTexCoord4       12
            gl_MultiTexCoord5       13
            gl_MultiTexCoord6       14
            gl_MultiTexCoord7       15
            \endverbatim

            \param index Index of the variable
            \param name Name of the attribute.
            */
            void        BindAttribLocation(GLint index, GLchar* name);
            GLint GetAttribLocation(GLchar* name);

            void BindFragDataLocation(GLuint color_number, GLchar* name);

            //GLfloat
            bool        SetVertexAttrib1f(GLuint index, GLfloat v0);                                          //!< Specify value of attribute. \param index The index of the vertex attribute. \param v0 value of the attribute component
            bool        SetVertexAttrib2f(GLuint index, GLfloat v0, GLfloat v1);                                  //!< Specify value of attribute. \param index The index of the vertex attribute. \param v0 value of the attribute component \param v1 value of the attribute component
            bool        SetVertexAttrib3f(GLuint index, GLfloat v0, GLfloat v1, GLfloat v2);                     //!< Specify value of attribute. \param index The index of the vertex attribute. \param v0 value of the attribute component \param v1 value of the attribute component \param v2 value of the attribute component
            bool        SetVertexAttrib4f(GLuint index, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);  //!< Specify value of attribute. \param index The index of the vertex attribute. \param v0 value of the attribute component \param v1 value of the attribute component \param v2 value of the attribute component \param v3 value of the attribute component

            //GLdouble
            bool        SetVertexAttrib1d(GLuint index, GLdouble v0);                                         //!< Specify value of attribute. \param index The index of the vertex attribute. \param v0 value of the attribute component
            bool        SetVertexAttrib2d(GLuint index, GLdouble v0, GLdouble v1);                            //!< Specify value of attribute. \param index The index of the vertex attribute. \param v0 value of the attribute component \param v1 value of the attribute component
            bool        SetVertexAttrib3d(GLuint index, GLdouble v0, GLdouble v1, GLdouble v2);               //!< Specify value of attribute. \param index The index of the vertex attribute. \param v0 value of the attribute component \param v1 value of the attribute component \param v2 value of the attribute component
            bool        SetVertexAttrib4d(GLuint index, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3);  //!< Specify value of attribute. \param index The index of the vertex attribute. \param v0 value of the attribute component \param v1 value of the attribute component \param v2 value of the attribute component \param v3 value of the attribute component

            //GLshort
            bool        SetVertexAttrib1s(GLuint index, GLshort v0);                                      //!< Specify value of attribute. \param index The index of the vertex attribute. \param v0 value of the attribute component
            bool        SetVertexAttrib2s(GLuint index, GLshort v0, GLshort v1);                          //!< Specify value of attribute. \param index The index of the vertex attribute. \param v0 value of the attribute component \param v1 value of the attribute component
            bool        SetVertexAttrib3s(GLuint index, GLshort v0, GLshort v1, GLshort v2);              //!< Specify value of attribute. \param index The index of the vertex attribute. \param v0 value of the attribute component \param v1 value of the attribute component \param v2 value of the attribute component
            bool        SetVertexAttrib4s(GLuint index, GLshort v0, GLshort v1, GLshort v2, GLshort v3);  //!< Specify value of attribute. \param index The index of the vertex attribute. \param v0 value of the attribute component \param v1 value of the attribute component \param v2 value of the attribute component \param v3 value of the attribute component

            // Normalized Byte (for example for RGBA colors)
            bool        SetVertexAttribNormalizedByte(GLuint index, GLbyte v0, GLbyte v1, GLbyte v2, GLbyte v3); //!< Specify value of attribute. Values will be normalized.

            //GLint (Requires GL_EXT_gpu_shader4)
            bool        SetVertexAttrib1i(GLuint index, GLint v0); //!< Specify value of attribute. Requires GL_EXT_gpu_shader4.
            bool        SetVertexAttrib2i(GLuint index, GLint v0, GLint v1); //!< Specify value of attribute. Requires GL_EXT_gpu_shader4.
            bool        SetVertexAttrib3i(GLuint index, GLint v0, GLint v1, GLint v2); //!< Specify value of attribute. Requires GL_EXT_gpu_shader4.
            bool        SetVertexAttrib4i(GLuint index, GLint v0, GLint v1, GLint v2, GLint v3); //!< Specify value of attribute. Requires GL_EXT_gpu_shader4.

            //GLuint (Requires GL_EXT_gpu_shader4)
            bool        SetVertexAttrib1ui(GLuint index, GLuint v0); //!< Specify value of attribute. \warning Requires GL_EXT_gpu_shader4. \param v0 value of the first component
            bool        SetVertexAttrib2ui(GLuint index, GLuint v0, GLuint v1); //!< Specify value of attribute. \warning Requires GL_EXT_gpu_shader4.
            bool        SetVertexAttrib3ui(GLuint index, GLuint v0, GLuint v1, GLuint v2); //!< Specify value of attribute. \warning Requires GL_EXT_gpu_shader4.
            bool        SetVertexAttrib4ui(GLuint index, GLuint v0, GLuint v1, GLuint v2, GLuint v3); //!< Specify value of attribute. \warning Requires GL_EXT_gpu_shader4.

            //! Enable this Shader:
            void        Enable(void) //!< Enables Shader (Shader is enabled by default)
            {
                _noshader = true;
            }

            //! Disable this Shader:
            void        Disable(void) //!< Disables Shader.
            {
                _noshader = false;
            }

            void        UsesGeometryShader(bool bYesNo) { uses_geometry_shader_ = bYesNo; }

        private:
			class Impl;
			Impl* impl_;
            GLuint      program_object_;                      // GLProgramObject


            GLcharARB*  linker_log;
            bool        is_linked_;

            bool        _noshader;

            bool        uses_geometry_shader_;

            int         input_primitive_type_;
            int         output_primitive_type_;
            int         max_number_of_output_vertices_;
        };

        //-----------------------------------------------------------------------------

        struct GlShaderBinaryFile
        {
            struct Header
            {
                GLenum binary_format;
                size_t binary_size;
            };
            Header header;
            char shader_binary[4]; // �ϒ�

            size_t GetFileSize() const
            {
                return sizeof(Header) + header.binary_size;
            }
        };

        //! To simplify the process loading/compiling/linking shaders this high level interface to simplify setup of a vertex/fragment shader was created. \ingroup GLSL \author Martin Christen
        class WSP_DLL_EXPORT GlShaderManager
        {
        public:


            GlShaderManager();
            ~GlShaderManager();

            // Regular GLSL (Vertex+Fragment Shader)
            GlShader* LoadFromMemory(const char* vertexMem, const char* fragmentMem); //!< load vertex/fragment shader from memory. If you specify 0 for one of the shaders, the fixed function pipeline is used for that part.

            // With Geometry Shader (Vertex+Geomentry+Fragment Shader)
            GlShader* LoadFromMemory(
                const char* vertexMem,
                const char* geometryMem,
                const char* fragmentMem); //!< load vertex/geometry/fragment shader from memory. If you specify 0 for one of the shaders, the fixed function pipeline is used for that part.

            GlShader* LoadFromBinary(const GlShaderBinaryFile& info);

            void      SetInputPrimitiveType(int nInputPrimitiveType);    //!< Set the input primitive type for the geometry shader \param nInputPrimitiveType Input Primitive Type, for example GL_TRIANGLES
            void      SetOutputPrimitiveType(int nOutputPrimitiveType);  //!< Set the output primitive type for the geometry shader \param nOutputPrimitiveType Output Primitive Type, for example GL_TRIANGLE_STRIP
            void      SetVerticesOut(int nVerticesOut);                  //!< Set the maximal number of vertices the geometry shader can output \param nVerticesOut Maximal number of output vertices. It is possible to output less vertices!

            bool      Free(GlShader* shader); //!< Remove the shader and free the memory occupied by this shader.

			std::vector<GlShader*>& shader_obj_list();

			const std::vector<GlShader*>& shader_obj_list() const;

        private:
			class Impl;
			Impl* impl_;
            int                     input_primitive_type_;
            int                     output_primitive_type_;
            int                     max_number_of_output_vertices_;
        };

        //-----------------------------------------------------------------------------
        // Global functions to initialize OpenGL Extensions and check for GLSL and 
        // OpenGL2. Also functions to check if Shader Model 4 is available and if
        // Geometry Shaders are supported.
        WSP_DLL_EXPORT bool InitOpenGLExtensions(void); //!< Initialize OpenGL Extensions (using glew) \ingroup GLSL
        WSP_DLL_EXPORT bool HasGLSLSupport(void);       //!< Returns true if OpenGL Shading Language is supported. (This function will return a GLSL version number in a future release) \ingroup GLSL  
        WSP_DLL_EXPORT bool HasOpenGL2Support(void);    //!< Returns true if OpenGL 2.0 is supported. This function is deprecated and shouldn't be used anymore. \ingroup GLSL \deprecated
        WSP_DLL_EXPORT bool HasGeometryShaderSupport(void); //!< Returns true if Geometry Shaders are supported. \ingroup GLSL
        WSP_DLL_EXPORT bool HasShaderModel4(void); //!< Returns true if Shader Model 4 is supported. \ingroup GLSL

        // these function names are deprecated, just here for backwards
        // compatibility. It is very likely they will be removed in a future version
#define initGLExtensions InitOpenGLExtensions
#define checkGLSL HasGLSLSupport
#define checkGL2  HasOpenGL2Support
        //----------------------------------------------------------------------------

    }
}

#pragma once

#include <gl/glew.h>
#include <wsp/common/_define_commoncore.h>

namespace wsp {
    namespace opengl {
        namespace glproxy {
            namespace detail {

                WSP_DLL_EXPORT void BindFramebuffer(GLenum target, GLuint framebuffer);

                WSP_DLL_EXPORT void DrawBuffer(GLenum buf);

                WSP_DLL_EXPORT void DrawBuffers(GLsizei n, const GLenum *bufs);

                WSP_DLL_EXPORT void DepthMask(GLboolean flag);

                WSP_DLL_EXPORT void Enable(GLenum cap);

                WSP_DLL_EXPORT void Disable(GLenum cap);

                WSP_DLL_EXPORT void Hint(GLenum target, GLenum mode);

                WSP_DLL_EXPORT void EnableClientState(GLenum cap);

                WSP_DLL_EXPORT void DisableClientState(GLenum cap);

                WSP_DLL_EXPORT void DrawElements(GLenum mode,
                    GLsizei count,
                    GLenum type,
                    const GLvoid * indices);

                WSP_DLL_EXPORT void Clear(GLbitfield mask);

                WSP_DLL_EXPORT void ClearColor(
                    GLclampf red,
                    GLclampf green,
                    GLclampf blue,
                    GLclampf alpha);

                WSP_DLL_EXPORT void ClearDepth(GLdouble depth);

                WSP_DLL_EXPORT void ClearBufferfv(
                    GLenum buffer,
                    GLint drawbuffer,
                    const GLfloat * value);

                WSP_DLL_EXPORT void BindTexture(GLenum target,
                    GLuint texture);

                WSP_DLL_EXPORT void TexImage2D(GLenum target,
                    GLint level,
                    GLint internalFormat,
                    GLsizei width,
                    GLsizei height,
                    GLint border,
                    GLenum format,
                    GLenum type,
                    const GLvoid * data);

                WSP_DLL_EXPORT void TexParameteri(GLenum target,
                    GLenum pname,
                    GLint param);

                WSP_DLL_EXPORT void ReadBuffer(GLenum mode);

                WSP_DLL_EXPORT void BlitFramebuffer(
                    GLint srcX0,
                    GLint srcY0,
                    GLint srcX1,
                    GLint srcY1,
                    GLint dstX0,
                    GLint dstY0,
                    GLint dstX1,
                    GLint dstY1,
                    GLbitfield mask,
                    GLenum filter);

                WSP_DLL_EXPORT void ReadPixels(
                    GLint  	x,
                    GLint  	y,
                    GLsizei  	width,
                    GLsizei  	height,
                    GLenum  	format,
                    GLenum  	type,
                    GLvoid *  	data);

                WSP_DLL_EXPORT void BindBuffer(GLenum target,
                    GLuint buffer);

                WSP_DLL_EXPORT void VertexAttribPointer(GLuint index,
                    GLint size,
                    GLenum type,
                    GLboolean normalized,
                    GLsizei stride,
                    const GLvoid * pointer);

                WSP_DLL_EXPORT void VertexAttribIPointer(GLuint index,
                    GLint size,
                    GLenum type,
                    GLsizei stride,
                    const GLvoid * pointer);

                WSP_DLL_EXPORT void BufferData(GLenum target,
                    GLsizeiptr size,
                    const GLvoid * data,
                    GLenum usage);

                WSP_DLL_EXPORT void BindBufferBase(GLenum target,
                    GLuint index,
                    GLuint buffer);

                WSP_DLL_EXPORT void EnableVertexAttribArray(GLuint index);

                WSP_DLL_EXPORT void DisableVertexAttribArray(GLuint index);

                WSP_DLL_EXPORT void BindVertexArray(GLuint array);

                WSP_DLL_EXPORT void VertexPointer(GLint  	size,
                    GLenum  	type,
                    GLsizei  	stride,
                    const GLvoid *  	pointer);

                WSP_DLL_EXPORT void ActiveTexture(GLenum texture);

                WSP_DLL_EXPORT void FramebufferTexture2D(GLenum target,
                    GLenum attachment,
                    GLenum textarget,
                    GLuint texture,
                    GLint level);

                WSP_DLL_EXPORT void Viewport(GLint x,
                    GLint y,
                    GLsizei width,
                    GLsizei height);
                WSP_DLL_EXPORT void DrawArrays(GLenum mode,
                    GLint first,
                    GLsizei count);
                WSP_DLL_EXPORT void Flush(void);
                WSP_DLL_EXPORT void Finish(void);
                WSP_DLL_EXPORT void BlendEquation(GLenum mode);
                WSP_DLL_EXPORT void BlendFunc(GLenum sfactor,
                    GLenum dfactor);
                WSP_DLL_EXPORT void DepthFunc(GLenum func);

                WSP_DLL_EXPORT void ColorMask(GLboolean red,
                    GLboolean green,
                    GLboolean blue,
                    GLboolean alpha);

                WSP_DLL_EXPORT void PolygonOffset(GLfloat factor,
                    GLfloat units);

                WSP_DLL_EXPORT void CullFace(GLenum mode);

                WSP_DLL_EXPORT void PixelStorei(GLenum pname,
                    GLint param);

                WSP_DLL_EXPORT void LineWidth(GLfloat width);

                WSP_DLL_EXPORT void BindRenderbuffer(GLenum target,
                    GLuint renderbuffer);

                WSP_DLL_EXPORT void RenderbufferStorage(GLenum target,
                    GLenum internalformat,
                    GLsizei width,
                    GLsizei height);

                WSP_DLL_EXPORT void FramebufferRenderbuffer(GLenum target,
                    GLenum attachment,
                    GLenum renderbuffertarget,
                    GLuint renderbuffer);

                WSP_DLL_EXPORT void PushMatrix(void);

                //WSP_DLL_EXPORT void Translatef(GLfloat  	x,
                //    GLfloat  	y,
                //    GLfloat  	z);

                //WSP_DLL_EXPORT void Scalef(GLfloat  	x,
                //    GLfloat  	y,
                //    GLfloat  	z);

                //WSP_DLL_EXPORT void Rotatef(GLfloat angle,
                //    GLfloat x,
                //    GLfloat y,
                //    GLfloat z);

                WSP_DLL_EXPORT void MatrixMode(GLenum mode);

                //WSP_DLL_EXPORT void NormalPointer(GLenum  	type,
                //    GLsizei  	stride,
                //    const GLvoid *  	pointer);

                //WSP_DLL_EXPORT void ColorPointer(GLint size,
                //    GLenum type,
                //    GLsizei stride,
                //    const GLvoid * pointer);

                WSP_DLL_EXPORT void TexCoordPointer(GLint size,
                    GLenum type,
                    GLsizei stride,
                    const GLvoid * pointer);

                WSP_DLL_EXPORT void PopMatrix(void);

                //WSP_DLL_EXPORT void TexEnvf(GLenum target,
                //    GLenum pname,
                //    GLfloat param);

                //WSP_DLL_EXPORT void TexEnvi(GLenum target,
                //    GLenum pname,
                //    GLint param);

                //WSP_DLL_EXPORT void Begin(GLenum  	mode);
                //
                //WSP_DLL_EXPORT void Normal3d(GLdouble nx,
                //    GLdouble ny,
                //    GLdouble nz);

                //WSP_DLL_EXPORT void Normal3f(GLfloat nx,
                //    GLfloat ny,
                //    GLfloat nz);

                //WSP_DLL_EXPORT void Vertex2f(GLfloat x,
                //    GLfloat y);

                //WSP_DLL_EXPORT void Vertex3fv(
                //    const GLfloat *v);

                //WSP_DLL_EXPORT void End();

                //WSP_DLL_EXPORT void Materialf(GLenum  	face,
                //    GLenum  	pname,
                //    GLfloat  	param);

                //WSP_DLL_EXPORT void Materialfv(GLenum  	face,
                //    GLenum  	pname,
                //    const GLfloat *  	params);


                //WSP_DLL_EXPORT void Color3fv(
                //    const GLfloat *v);

                //WSP_DLL_EXPORT void Lightfv(GLenum light,
                //    GLenum pname,
                //    const GLfloat * params);

                //WSP_DLL_EXPORT void Vertex3d(
                //    GLdouble x,
                //    GLdouble y,
                //    GLdouble z);

                WSP_DLL_EXPORT void TexParameterf(GLenum target,
                    GLenum pname,
                    GLfloat param);

                WSP_DLL_EXPORT void TexParameterfv(GLenum target,
                    GLenum pname,
                    const GLfloat* params);

                WSP_DLL_EXPORT void LoadIdentity(void);

                WSP_DLL_EXPORT void Color4f(GLfloat  	red,
                    GLfloat  	green,
                    GLfloat  	blue,
                    GLfloat  	alpha);

                WSP_DLL_EXPORT void RasterPos2i(
                    GLint x,
                    GLint y);

                WSP_DLL_EXPORT void PolygonMode(GLenum face,
                    GLenum mode);
                WSP_DLL_EXPORT void FramebufferTexture(GLenum target,
                    GLenum attachment,
                    GLuint texture,
                    GLint level);
                WSP_DLL_EXPORT void AlphaFunc(GLenum  	func,
                    GLclampf  	ref);

                WSP_DLL_EXPORT void UseProgram(GLuint program);

                WSP_DLL_EXPORT void Uniform1f(GLint location,
                    GLfloat v0);


                WSP_DLL_EXPORT void Uniform2f(GLint location,
                    GLfloat v0,
                    GLfloat v1);

                WSP_DLL_EXPORT void Uniform3f(GLint location,
                    GLfloat v0,
                    GLfloat v1,
                    GLfloat v2);

                WSP_DLL_EXPORT void Uniform4f(GLint location,
                    GLfloat v0,
                    GLfloat v1,
                    GLfloat v2,
                    GLfloat v3);

                WSP_DLL_EXPORT void Uniform1i(GLint location,
                    GLint v0);

                WSP_DLL_EXPORT void Uniform2i(GLint location,
                    GLint v0,
                    GLint v1);

                WSP_DLL_EXPORT void Uniform3i(GLint location,
                    GLint v0,
                    GLint v1,
                    GLint v2);

                WSP_DLL_EXPORT void Uniform4i(GLint location,
                    GLint v0,
                    GLint v1,
                    GLint v2,
                    GLint v3);

                WSP_DLL_EXPORT void Uniform1ui(GLint location,
                    GLuint v0);

                WSP_DLL_EXPORT void Uniform2ui(GLint location,
                    GLuint v0,
                    GLuint v1);

                WSP_DLL_EXPORT void Uniform3ui(GLint location,
                    GLuint v0,
                    GLuint v1,
                    GLuint v2);

                WSP_DLL_EXPORT void Uniform4ui(GLint location,
                    GLuint v0,
                    GLuint v1,
                    GLuint v2,
                    GLuint v3);

                WSP_DLL_EXPORT void Uniform1fv(GLint location,
                    GLsizei count,
                    const GLfloat *value);

                WSP_DLL_EXPORT void Uniform2fv(GLint location,
                    GLsizei count,
                    const GLfloat *value);

                WSP_DLL_EXPORT void Uniform3fv(GLint location,
                    GLsizei count,
                    const GLfloat *value);

                WSP_DLL_EXPORT void Uniform4fv(GLint location,
                    GLsizei count,
                    const GLfloat *value);

                WSP_DLL_EXPORT void Uniform1iv(GLint location,
                    GLsizei count,
                    const GLint *value);

                WSP_DLL_EXPORT void Uniform2iv(GLint location,
                    GLsizei count,
                    const GLint *value);

                WSP_DLL_EXPORT void Uniform3iv(GLint location,
                    GLsizei count,
                    const GLint *value);

                WSP_DLL_EXPORT void Uniform4iv(GLint location,
                    GLsizei count,
                    const GLint *value);

                WSP_DLL_EXPORT void Uniform1uiv(GLint location,
                    GLsizei count,
                    const GLuint *value);

                WSP_DLL_EXPORT void Uniform2uiv(GLint location,
                    GLsizei count,
                    const GLuint *value);

                WSP_DLL_EXPORT void Uniform3uiv(GLint location,
                    GLsizei count,
                    const GLuint *value);

                WSP_DLL_EXPORT void Uniform4uiv(GLint location,
                    GLsizei count,
                    const GLuint *value);

                WSP_DLL_EXPORT void UniformMatrix2fv(GLint location,
                    GLsizei count,
                    GLboolean transpose,
                    const GLfloat *value);

                WSP_DLL_EXPORT void UniformMatrix3fv(GLint location,
                    GLsizei count,
                    GLboolean transpose,
                    const GLfloat *value);

                WSP_DLL_EXPORT void UniformMatrix4fv(GLint location,
                    GLsizei count,
                    GLboolean transpose,
                    const GLfloat *value);

                WSP_DLL_EXPORT void UniformBlockBinding(GLuint program,
                    GLuint uniformBlockIndex,
                    GLuint uniformBlockBinding);

                WSP_DLL_EXPORT void VertexAttrib1f(GLuint index,
                    GLfloat v0);

                WSP_DLL_EXPORT void VertexAttrib1s(GLuint index,
                    GLshort v0);

                WSP_DLL_EXPORT void VertexAttrib1d(GLuint index,
                    GLdouble v0);

                WSP_DLL_EXPORT void VertexAttribI1i(GLuint index,
                    GLint v0);

                WSP_DLL_EXPORT void VertexAttribI1ui(GLuint index,
                    GLuint v0);

                WSP_DLL_EXPORT void VertexAttrib2f(GLuint index,
                    GLfloat v0,
                    GLfloat v1);

                WSP_DLL_EXPORT void VertexAttrib2s(GLuint index,
                    GLshort v0,
                    GLshort v1);

                WSP_DLL_EXPORT void VertexAttrib2d(GLuint index,
                    GLdouble v0,
                    GLdouble v1);

                WSP_DLL_EXPORT void VertexAttribI2i(GLuint index,
                    GLint v0,
                    GLint v1);

                WSP_DLL_EXPORT void VertexAttribI2ui(GLuint index,
                    GLuint v0,
                    GLuint v1);

                WSP_DLL_EXPORT void VertexAttrib3f(GLuint index,
                    GLfloat v0,
                    GLfloat v1,
                    GLfloat v2);

                WSP_DLL_EXPORT void VertexAttrib3s(GLuint index,
                    GLshort v0,
                    GLshort v1,
                    GLshort v2);

                WSP_DLL_EXPORT void VertexAttrib3d(GLuint index,
                    GLdouble v0,
                    GLdouble v1,
                    GLdouble v2);

                WSP_DLL_EXPORT void VertexAttribI3i(GLuint index,
                    GLint v0,
                    GLint v1,
                    GLint v2);

                WSP_DLL_EXPORT void VertexAttribI3ui(GLuint index,
                    GLuint v0,
                    GLuint v1,
                    GLuint v2);

                WSP_DLL_EXPORT void VertexAttrib4f(GLuint index,
                    GLfloat v0,
                    GLfloat v1,
                    GLfloat v2,
                    GLfloat v3);

                WSP_DLL_EXPORT void VertexAttrib4s(GLuint index,
                    GLshort v0,
                    GLshort v1,
                    GLshort v2,
                    GLshort v3);

                WSP_DLL_EXPORT void VertexAttrib4d(GLuint index,
                    GLdouble v0,
                    GLdouble v1,
                    GLdouble v2,
                    GLdouble v3);

                WSP_DLL_EXPORT void VertexAttrib4Nub(GLuint index,
                    GLubyte v0,
                    GLubyte v1,
                    GLubyte v2,
                    GLubyte v3);

                WSP_DLL_EXPORT void VertexAttribI4i(GLuint index,
                    GLint v0,
                    GLint v1,
                    GLint v2,
                    GLint v3);

                WSP_DLL_EXPORT void VertexAttribI4ui(GLuint index,
                    GLuint v0,
                    GLuint v1,
                    GLuint v2,
                    GLuint v3);

                WSP_DLL_EXPORT void BindAttribLocation(GLuint program,
                    GLuint index,
                    const GLchar *name);

                WSP_DLL_EXPORT void BindFragDataLocation(GLuint program,
                    GLuint colorNumber,
                    const char * name);

                WSP_DLL_EXPORT void BeginQuery(GLenum target,
                    GLuint id);

                WSP_DLL_EXPORT void EndQuery(GLenum target);

                WSP_DLL_EXPORT void WaitSync(GLsync sync,
                    GLbitfield flags,
                    GLuint64 timeout);

                WSP_DLL_EXPORT GLenum ClientWaitSync(GLsync sync,
                    GLbitfield flags,
                    GLuint64 timeout);

                // glu
                WSP_DLL_EXPORT void Ortho2D(GLdouble left,
                    GLdouble right,
                    GLdouble bottom,
                    GLdouble top);

                // glut
                WSP_DLL_EXPORT void BitmapCharacter(void *font, int character);

                WSP_DLL_EXPORT void DepthRange(GLdouble nearval, GLdouble farVal);

                WSP_DLL_EXPORT void DepthRangef(GLfloat nearval, GLfloat farVal);
            }
        }
    }
}

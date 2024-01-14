
#include <wsp/opengl/GlProxyImpl.h>
#include <wsp/common/fn-debug.h>

#ifdef _WIN32
#include <GL/glut.h>
#else
#include <GLUT/GLUT.h>
#endif

#define EXEC_GL_FUNC(gl_func) do { gl_func; GLenum gl_status = glGetError(); WSP_ASSERT(gl_status == GL_NO_ERROR, #gl_func " failed with %s(%d)", gluErrorString(gl_status),gl_status); } while(false)
#define EXEC_GL_FUNC_RET(gl_func) do { auto ret = gl_func; GLenum gl_status = glGetError(); WSP_ASSERT(gl_status == GL_NO_ERROR, #gl_func " failed with %s(%d)", gluErrorString(gl_status),gl_status); return ret; } while(false)

void wsp::opengl::glproxy::detail::BindFramebuffer(GLenum target, GLuint framebuffer)
{
    EXEC_GL_FUNC(glBindFramebufferEXT(target, framebuffer));
}

void wsp::opengl::glproxy::detail::DrawBuffer(GLenum buf)
{
    EXEC_GL_FUNC(glDrawBuffer(buf));
}

void wsp::opengl::glproxy::detail::DrawBuffers(GLsizei n, const GLenum *bufs)
{
    EXEC_GL_FUNC(glDrawBuffers(n, bufs));
}

void wsp::opengl::glproxy::detail::DepthMask(GLboolean flag)
{
    EXEC_GL_FUNC(glDepthMask(flag));
}

void wsp::opengl::glproxy::detail::Enable(GLenum cap)
{
    EXEC_GL_FUNC(glEnable(cap));
}

void wsp::opengl::glproxy::detail::Disable(GLenum cap)
{
    EXEC_GL_FUNC(glDisable(cap));
}

void wsp::opengl::glproxy::detail::Hint(GLenum target, GLenum mode)
{
    EXEC_GL_FUNC(glHint(target, mode));
}

void wsp::opengl::glproxy::detail::EnableClientState(GLenum cap)
{
    EXEC_GL_FUNC(glEnableClientState(cap));
}

void wsp::opengl::glproxy::detail::DisableClientState(GLenum cap)
{
    EXEC_GL_FUNC(glDisableClientState(cap));
}

void wsp::opengl::glproxy::detail::Clear(GLbitfield mask)
{
    EXEC_GL_FUNC(glClear(mask));
}

void wsp::opengl::glproxy::detail::ClearColor(
    GLclampf red,
    GLclampf green,
    GLclampf blue,
    GLclampf alpha)
{
    EXEC_GL_FUNC(glClearColor(red, green, blue, alpha));
}

void wsp::opengl::glproxy::detail::ClearBufferfv(
    GLenum buffer,
    GLint drawbuffer,
    const GLfloat * value)
{
    EXEC_GL_FUNC(glClearBufferfv(buffer, drawbuffer, value));
}

void wsp::opengl::glproxy::detail::ClearDepth(GLdouble depth)
{
    EXEC_GL_FUNC(glClearDepth(depth));
}

void wsp::opengl::glproxy::detail::BindTexture(
    GLenum target,
    GLuint texture)
{
    EXEC_GL_FUNC(glBindTexture(target, texture));
}

void wsp::opengl::glproxy::detail::TexImage2D(
    GLenum target,
    GLint level,
    GLint internalFormat,
    GLsizei width,
    GLsizei height,
    GLint border,
    GLenum format,
    GLenum type,
    const GLvoid * data)
{
    EXEC_GL_FUNC(glTexImage2D(target, level, internalFormat, width, height, border, format, type, data));
}

void wsp::opengl::glproxy::detail::TexParameteri(
    GLenum target,
    GLenum pname,
    GLint param)
{
    EXEC_GL_FUNC(glTexParameteri(target, pname, param));
}

void wsp::opengl::glproxy::detail::ReadBuffer(GLenum mode)
{
    EXEC_GL_FUNC(glReadBuffer(mode));
}

void wsp::opengl::glproxy::detail::BlitFramebuffer(
    GLint srcX0,
    GLint srcY0,
    GLint srcX1,
    GLint srcY1,
    GLint dstX0,
    GLint dstY0,
    GLint dstX1,
    GLint dstY1,
    GLbitfield mask,
    GLenum filter)
{
    EXEC_GL_FUNC(glBlitFramebuffer(
        srcX0,
        srcY0,
        srcX1,
        srcY1,
        dstX0,
        dstY0,
        dstX1,
        dstY1,
        mask,
        filter
    ));
}

void wsp::opengl::glproxy::detail::ReadPixels(
    GLint  	    x,
    GLint  	    y,
    GLsizei  	width,
    GLsizei  	height,
    GLenum  	format,
    GLenum  	type,
    GLvoid *  	data)
{
    EXEC_GL_FUNC(glReadPixels(
        x,
        y,
        width,
        height,
        format,
        type,
        data
    ));
}

void wsp::opengl::glproxy::detail::BindBuffer(GLenum target,
    GLuint buffer)
{
    EXEC_GL_FUNC(glBindBuffer(target, buffer));
}

void wsp::opengl::glproxy::detail::VertexAttribPointer(GLuint index,
    GLint size,
    GLenum type,
    GLboolean normalized,
    GLsizei stride,
    const GLvoid * pointer)
{
    EXEC_GL_FUNC(glVertexAttribPointer(index, size, type, normalized, stride, pointer));
}

void wsp::opengl::glproxy::detail::VertexAttribIPointer(GLuint index,
    GLint size,
    GLenum type,
    GLsizei stride,
    const GLvoid * pointer)
{
    EXEC_GL_FUNC(glVertexAttribIPointer(index, size, type, stride, pointer));
}

void wsp::opengl::glproxy::detail::BufferData(GLenum target,
    GLsizeiptr size,
    const GLvoid * data,
    GLenum usage)
{
    EXEC_GL_FUNC(glBufferData(target, size, data, usage));
}

void wsp::opengl::glproxy::detail::BindBufferBase(GLenum target,
    GLuint index,
    GLuint buffer)
{
    EXEC_GL_FUNC(glBindBufferBase(target, index, buffer));
}

void wsp::opengl::glproxy::detail::EnableVertexAttribArray(GLuint index)
{
    EXEC_GL_FUNC(glEnableVertexAttribArray(index));
}

void wsp::opengl::glproxy::detail::DisableVertexAttribArray(GLuint index)
{
    EXEC_GL_FUNC(glDisableVertexAttribArray(index));
}

void wsp::opengl::glproxy::detail::BindVertexArray(GLuint array)
{
    EXEC_GL_FUNC(glBindVertexArray(array));
}

void wsp::opengl::glproxy::detail::DrawElements(GLenum mode,
    GLsizei count,
    GLenum type,
    const GLvoid * indices)
{
    EXEC_GL_FUNC(glDrawElements(mode, count, type, indices));
}

void wsp::opengl::glproxy::detail::VertexPointer(GLint  	size,
    GLenum  	type,
    GLsizei  	stride,
    const GLvoid *  	pointer)
{
    EXEC_GL_FUNC(glVertexPointer(size, type, stride, pointer));
}

void wsp::opengl::glproxy::detail::ActiveTexture(GLenum texture)
{
    EXEC_GL_FUNC(glActiveTextureARB(texture));
}

void wsp::opengl::glproxy::detail::FramebufferTexture2D(GLenum target,
    GLenum attachment,
    GLenum textarget,
    GLuint texture,
    GLint level)
{
    EXEC_GL_FUNC(glFramebufferTexture2D(target, attachment, textarget, texture, level));
}

void wsp::opengl::glproxy::detail::UseProgram(GLuint program)
{
    EXEC_GL_FUNC(glUseProgram(program));
}

void wsp::opengl::glproxy::detail::Viewport(GLint x,
    GLint y,
    GLsizei width,
    GLsizei height)
{
    EXEC_GL_FUNC(glViewport(x, y, width, height));
}

void wsp::opengl::glproxy::detail::DrawArrays(GLenum mode,
    GLint first,
    GLsizei count)
{
    EXEC_GL_FUNC(glDrawArrays(mode, first, count));
}

void wsp::opengl::glproxy::detail::Flush(void)
{
    EXEC_GL_FUNC(glFlush());
}

void wsp::opengl::glproxy::detail::Finish(void)
{
    EXEC_GL_FUNC(glFinish());
}

void wsp::opengl::glproxy::detail::BlendEquation(GLenum mode)
{
    EXEC_GL_FUNC(glBlendEquation(mode));
}

void wsp::opengl::glproxy::detail::BlendFunc(GLenum sfactor,
    GLenum dfactor)
{
    EXEC_GL_FUNC(glBlendFunc(sfactor, dfactor));
}

void wsp::opengl::glproxy::detail::DepthFunc(GLenum func)
{
    EXEC_GL_FUNC(glDepthFunc(func));
}

void wsp::opengl::glproxy::detail::ColorMask(GLboolean red,
    GLboolean green,
    GLboolean blue,
    GLboolean alpha)
{
    EXEC_GL_FUNC(glColorMask(red, green, blue, alpha));
}

void wsp::opengl::glproxy::detail::PolygonOffset(GLfloat factor,
    GLfloat units)
{
    EXEC_GL_FUNC(glPolygonOffset(factor, units));
}

void wsp::opengl::glproxy::detail::CullFace(GLenum mode)
{
    EXEC_GL_FUNC(glCullFace(mode));
}

void wsp::opengl::glproxy::detail::PixelStorei(GLenum pname,
    GLint param)
{
    EXEC_GL_FUNC(glPixelStorei(pname, param));
}

void wsp::opengl::glproxy::detail::LineWidth(GLfloat width)
{
    EXEC_GL_FUNC(glLineWidth(width));
}

void wsp::opengl::glproxy::detail::BindRenderbuffer(GLenum target,
    GLuint renderbuffer)
{
    EXEC_GL_FUNC(glBindRenderbuffer(target, renderbuffer));
}

void wsp::opengl::glproxy::detail::RenderbufferStorage(GLenum target,
    GLenum internalformat,
    GLsizei width,
    GLsizei height)
{
    EXEC_GL_FUNC(glRenderbufferStorage(target, internalformat, width, height));
}

void wsp::opengl::glproxy::detail::FramebufferRenderbuffer(GLenum target,
    GLenum attachment,
    GLenum renderbuffertarget,
    GLuint renderbuffer)
{
    EXEC_GL_FUNC(glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer));
}

void wsp::opengl::glproxy::detail::PushMatrix(void)
{
    EXEC_GL_FUNC(glPushMatrix());
}

//void wsp::opengl::glproxy::detail::Translatef(GLfloat  	x,
//    GLfloat  	y,
//    GLfloat  	z)
//{
//    EXEC_GL_FUNC(glTranslatef(x, y, z));
//}
//
//void wsp::opengl::glproxy::detail::Scalef(GLfloat  	x,
//    GLfloat  	y,
//    GLfloat  	z)
//{
//    EXEC_GL_FUNC(glScalef(x, y, z));
//}
//
//void wsp::opengl::glproxy::detail::Rotatef(GLfloat angle,
//    GLfloat x,
//    GLfloat y,
//    GLfloat z)
//{
//    EXEC_GL_FUNC(glRotatef(angle, x, y, z));
//}

void wsp::opengl::glproxy::detail::MatrixMode(GLenum mode)
{
    EXEC_GL_FUNC(glMatrixMode(mode));
}

//void wsp::opengl::glproxy::detail::NormalPointer(GLenum  	type,
//    GLsizei  	stride,
//    const GLvoid *  	pointer)
//{
//    EXEC_GL_FUNC(glNormalPointer(type, stride, pointer));
//}
//
//void wsp::opengl::glproxy::detail::ColorPointer(GLint size,
//    GLenum type,
//    GLsizei stride,
//    const GLvoid * pointer)
//{
//    EXEC_GL_FUNC(glColorPointer(size, type, stride, pointer));
//}

void wsp::opengl::glproxy::detail::TexCoordPointer(GLint size,
    GLenum type,
    GLsizei stride,
    const GLvoid * pointer)
{
    EXEC_GL_FUNC(glTexCoordPointer(size, type, stride, pointer));
}

void wsp::opengl::glproxy::detail::PopMatrix(void)
{
    EXEC_GL_FUNC(glPopMatrix());
}

void wsp::opengl::glproxy::detail::UniformMatrix3fv(GLint location,
    GLsizei count,
    GLboolean transpose,
    const GLfloat *value)
{
    EXEC_GL_FUNC(glUniformMatrix3fv(location, count, transpose, value));
}

//void wsp::opengl::glproxy::detail::TexEnvf(GLenum target,
//    GLenum pname,
//    GLfloat param)
//{
//    EXEC_GL_FUNC(glTexEnvf(target, pname, param));
//}
//
//void wsp::opengl::glproxy::detail::TexEnvi(GLenum target,
//    GLenum pname,
//    GLint param)
//{
//    EXEC_GL_FUNC(glTexEnvi(target, pname, param));
//}
//
//void wsp::opengl::glproxy::detail::Begin(GLenum  	mode)
//{
//    EXEC_GL_FUNC(glBegin(mode));
//}
//
//void wsp::opengl::glproxy::detail::Normal3d(GLdouble nx,
//    GLdouble ny,
//    GLdouble nz)
//{
//    EXEC_GL_FUNC(glNormal3d(nx, ny, nz));
//}
//
//void wsp::opengl::glproxy::detail::Normal3f(GLfloat nx,
//    GLfloat ny,
//    GLfloat nz)
//{
//    EXEC_GL_FUNC(glNormal3f(nx, ny, nz));
//}
//
//void wsp::opengl::glproxy::detail::Vertex2f(GLfloat x,
//    GLfloat y)
//{
//    EXEC_GL_FUNC(glVertex2f(x, y));
//}
//
//void wsp::opengl::glproxy::detail::Vertex3fv(
//    const GLfloat *v)
//{
//    EXEC_GL_FUNC(glVertex3fv(v));
//}
//
//void wsp::opengl::glproxy::detail::End()
//{
//    EXEC_GL_FUNC(glEnd());
//}

void wsp::opengl::glproxy::detail::PolygonMode(GLenum face,
    GLenum mode)
{
    EXEC_GL_FUNC(glPolygonMode(face, mode));
}

//void wsp::opengl::glproxy::detail::Materialf(GLenum  	face,
//    GLenum  	pname,
//    GLfloat  	param)
//{
//    EXEC_GL_FUNC(glMaterialf(face, pname, param));
//}
//
//void wsp::opengl::glproxy::detail::Materialfv(GLenum  	face,
//    GLenum  	pname,
//    const GLfloat *  	params)
//{
//    EXEC_GL_FUNC(glMaterialfv(face, pname, params));
//}
//
//void wsp::opengl::glproxy::detail::Color3fv(
//    const GLfloat *v)
//{
//    EXEC_GL_FUNC(glColor3fv(v));
//}
//
//void wsp::opengl::glproxy::detail::Lightfv(GLenum light,
//    GLenum pname,
//    const GLfloat * params)
//{
//    EXEC_GL_FUNC(glLightfv(light, pname, params));
//}

void wsp::opengl::glproxy::detail::TexParameterf(GLenum target,
    GLenum pname,
    GLfloat param)
{
    EXEC_GL_FUNC(glTexParameterf(target, pname, param));
}

void wsp::opengl::glproxy::detail::TexParameterfv(GLenum target,
    GLenum pname,
    const GLfloat* params)
{
    EXEC_GL_FUNC(glTexParameterfv(target, pname, params));
}


void wsp::opengl::glproxy::detail::FramebufferTexture(GLenum target,
    GLenum attachment,
    GLuint texture,
    GLint level)
{
    EXEC_GL_FUNC(glFramebufferTexture(target, attachment, texture, level));
}

//void wsp::opengl::glproxy::detail::Vertex3d(
//    GLdouble x,
//    GLdouble y,
//    GLdouble z)
//{
//    EXEC_GL_FUNC(glVertex3d(x, y, z));
//}
//
void wsp::opengl::glproxy::detail::LoadIdentity(void)
{
    EXEC_GL_FUNC(glLoadIdentity());
}

void wsp::opengl::glproxy::detail::Color4f(GLfloat  	red,
    GLfloat  	green,
    GLfloat  	blue,
    GLfloat  	alpha)
{
    EXEC_GL_FUNC(glColor4f(red, green, blue, alpha));
}

void wsp::opengl::glproxy::detail::RasterPos2i(
    GLint x,
    GLint y)
{
    EXEC_GL_FUNC(glRasterPos2i(x, y));
}

void wsp::opengl::glproxy::detail::AlphaFunc(GLenum  	func,
    GLclampf  	ref)
{
    EXEC_GL_FUNC(glAlphaFunc(func, ref));
}

void wsp::opengl::glproxy::detail::Uniform1f(GLint location,
    GLfloat v0)
{
    EXEC_GL_FUNC(glUniform1f(location, v0));
}

void wsp::opengl::glproxy::detail::Uniform2f(GLint location,
    GLfloat v0,
    GLfloat v1)
{
    EXEC_GL_FUNC(glUniform2f(location, v0, v1));
}

void wsp::opengl::glproxy::detail::Uniform3f(GLint location,
    GLfloat v0,
    GLfloat v1,
    GLfloat v2)
{
    EXEC_GL_FUNC(glUniform3f(location, v0, v1, v2));
}

void wsp::opengl::glproxy::detail::Uniform4f(GLint location,
    GLfloat v0,
    GLfloat v1,
    GLfloat v2,
    GLfloat v3)
{
    EXEC_GL_FUNC(glUniform4f(location, v0, v1, v2, v3));
}

void wsp::opengl::glproxy::detail::Uniform1i(GLint location,
    GLint v0)
{
    EXEC_GL_FUNC(glUniform1i(location, v0));
}

void wsp::opengl::glproxy::detail::Uniform2i(GLint location,
    GLint v0,
    GLint v1)
{
    EXEC_GL_FUNC(glUniform2i(location, v0, v1));
}

void wsp::opengl::glproxy::detail::Uniform3i(GLint location,
    GLint v0,
    GLint v1,
    GLint v2)
{
    EXEC_GL_FUNC(glUniform3i(location, v0, v1, v2));
}

void wsp::opengl::glproxy::detail::Uniform4i(GLint location,
    GLint v0,
    GLint v1,
    GLint v2,
    GLint v3)
{
    EXEC_GL_FUNC(glUniform4i(location, v0, v1, v2, v3));
}

void wsp::opengl::glproxy::detail::Uniform1ui(GLint location,
    GLuint v0)
{
    EXEC_GL_FUNC(glUniform1ui(location, v0));
}

void wsp::opengl::glproxy::detail::Uniform2ui(GLint location,
    GLuint v0,
    GLuint v1)
{
    EXEC_GL_FUNC(glUniform2ui(location, v0, v1));
}

void wsp::opengl::glproxy::detail::Uniform3ui(GLint location,
    GLuint v0,
    GLuint v1,
    GLuint v2)
{
    EXEC_GL_FUNC(glUniform3ui(location, v0, v1, v2));
}

void wsp::opengl::glproxy::detail::Uniform4ui(GLint location,
    GLuint v0,
    GLuint v1,
    GLuint v2,
    GLuint v3)
{
    EXEC_GL_FUNC(glUniform4ui(location, v0, v1, v2, v3));
}

void wsp::opengl::glproxy::detail::Uniform1fv(GLint location,
    GLsizei count,
    const GLfloat *value)
{
    EXEC_GL_FUNC(glUniform1fv(location, count, value));
}

void wsp::opengl::glproxy::detail::Uniform2fv(GLint location,
    GLsizei count,
    const GLfloat *value)
{
    EXEC_GL_FUNC(glUniform2fv(location, count, value));
}

void wsp::opengl::glproxy::detail::Uniform3fv(GLint location,
    GLsizei count,
    const GLfloat *value)
{
    EXEC_GL_FUNC(glUniform3fv(location, count, value));
}

void wsp::opengl::glproxy::detail::Uniform4fv(GLint location,
    GLsizei count,
    const GLfloat *value)
{
    EXEC_GL_FUNC(glUniform4fv(location, count, value));
}

void wsp::opengl::glproxy::detail::Uniform1iv(GLint location,
    GLsizei count,
    const GLint *value)
{
    EXEC_GL_FUNC(glUniform1iv(location, count, value));
}

void wsp::opengl::glproxy::detail::Uniform2iv(GLint location,
    GLsizei count,
    const GLint *value)
{
    EXEC_GL_FUNC(glUniform2iv(location, count, value));
}

void wsp::opengl::glproxy::detail::Uniform3iv(GLint location,
    GLsizei count,
    const GLint *value)
{
    EXEC_GL_FUNC(glUniform3iv(location, count, value));
}

void wsp::opengl::glproxy::detail::Uniform4iv(GLint location,
    GLsizei count,
    const GLint *value)
{
    EXEC_GL_FUNC(glUniform4iv(location, count, value));
}

void wsp::opengl::glproxy::detail::Uniform1uiv(GLint location,
    GLsizei count,
    const GLuint *value)
{
    EXEC_GL_FUNC(glUniform1uiv(location, count, value));
}

void wsp::opengl::glproxy::detail::Uniform2uiv(GLint location,
    GLsizei count,
    const GLuint *value)
{
    EXEC_GL_FUNC(glUniform2uiv(location, count, value));
}

void wsp::opengl::glproxy::detail::Uniform3uiv(GLint location,
    GLsizei count,
    const GLuint *value)
{
    EXEC_GL_FUNC(glUniform3uiv(location, count, value));
}

void wsp::opengl::glproxy::detail::Uniform4uiv(GLint location,
    GLsizei count,
    const GLuint *value)
{
    EXEC_GL_FUNC(glUniform4uiv(location, count, value));
}

void wsp::opengl::glproxy::detail::UniformMatrix2fv(GLint location,
    GLsizei count,
    GLboolean transpose,
    const GLfloat *value)
{
    EXEC_GL_FUNC(glUniformMatrix2fv(location, count, transpose, value));
}

void wsp::opengl::glproxy::detail::UniformMatrix4fv(GLint location,
    GLsizei count,
    GLboolean transpose,
    const GLfloat *value)
{
    EXEC_GL_FUNC(glUniformMatrix4fv(location, count, transpose, value));
}

void wsp::opengl::glproxy::detail::UniformBlockBinding(GLuint program,
    GLuint uniformBlockIndex,
    GLuint uniformBlockBinding)
{
    EXEC_GL_FUNC(glUniformBlockBinding(program, uniformBlockIndex, uniformBlockBinding));
}


void wsp::opengl::glproxy::detail::VertexAttrib1f(GLuint index,
    GLfloat v0)
{
    EXEC_GL_FUNC(glVertexAttrib1f(index, v0));
}

void wsp::opengl::glproxy::detail::VertexAttrib1s(GLuint index,
    GLshort v0)
{
    EXEC_GL_FUNC(glVertexAttrib1s(index, v0));
}

void wsp::opengl::glproxy::detail::VertexAttrib1d(GLuint index,
    GLdouble v0)
{
    EXEC_GL_FUNC(glVertexAttrib1d(index, v0));
}

void wsp::opengl::glproxy::detail::VertexAttribI1i(GLuint index,
    GLint v0)
{
    EXEC_GL_FUNC(glVertexAttribI1i(index, v0));
}

void wsp::opengl::glproxy::detail::VertexAttribI1ui(GLuint index,
    GLuint v0)
{
    EXEC_GL_FUNC(glVertexAttribI1ui(index, v0));
}

void wsp::opengl::glproxy::detail::VertexAttrib2f(GLuint index,
    GLfloat v0,
    GLfloat v1)
{
    EXEC_GL_FUNC(glVertexAttrib2f(index, v0, v1));
}

void wsp::opengl::glproxy::detail::VertexAttrib2s(GLuint index,
    GLshort v0,
    GLshort v1)
{
    EXEC_GL_FUNC(glVertexAttrib2s(index, v0, v1));
}

void wsp::opengl::glproxy::detail::VertexAttrib2d(GLuint index,
    GLdouble v0,
    GLdouble v1)
{
    EXEC_GL_FUNC(glVertexAttrib2d(index, v0, v1));
}

void wsp::opengl::glproxy::detail::VertexAttribI2i(GLuint index,
    GLint v0,
    GLint v1)
{
    EXEC_GL_FUNC(glVertexAttribI2i(index, v0, v1));
}

void wsp::opengl::glproxy::detail::VertexAttribI2ui(GLuint index,
    GLuint v0,
    GLuint v1)
{
    EXEC_GL_FUNC(glVertexAttribI2ui(index, v0, v1));
}

void wsp::opengl::glproxy::detail::VertexAttrib3f(GLuint index,
    GLfloat v0,
    GLfloat v1,
    GLfloat v2)
{
    EXEC_GL_FUNC(glVertexAttrib3f(index, v0, v1, v2));
}

void wsp::opengl::glproxy::detail::VertexAttrib3s(GLuint index,
    GLshort v0,
    GLshort v1,
    GLshort v2)
{
    EXEC_GL_FUNC(glVertexAttrib3s(index, v0, v1, v2));
}

void wsp::opengl::glproxy::detail::VertexAttrib3d(GLuint index,
    GLdouble v0,
    GLdouble v1,
    GLdouble v2)
{
    EXEC_GL_FUNC(glVertexAttrib3d(index, v0, v1, v2));
}

void wsp::opengl::glproxy::detail::VertexAttribI3i(GLuint index,
    GLint v0,
    GLint v1,
    GLint v2)
{
    EXEC_GL_FUNC(glVertexAttribI3i(index, v0, v1, v2));
}

void wsp::opengl::glproxy::detail::VertexAttribI3ui(GLuint index,
    GLuint v0,
    GLuint v1,
    GLuint v2)
{
    EXEC_GL_FUNC(glVertexAttribI3ui(index, v0, v1, v2));
}

void wsp::opengl::glproxy::detail::VertexAttrib4f(GLuint index,
    GLfloat v0,
    GLfloat v1,
    GLfloat v2,
    GLfloat v3)
{
    EXEC_GL_FUNC(glVertexAttrib4f(index, v0, v1, v2, v3));
}

void wsp::opengl::glproxy::detail::VertexAttrib4s(GLuint index,
    GLshort v0,
    GLshort v1,
    GLshort v2,
    GLshort v3)
{
    EXEC_GL_FUNC(glVertexAttrib4s(index, v0, v1, v2, v3));
}

void wsp::opengl::glproxy::detail::VertexAttrib4d(GLuint index,
    GLdouble v0,
    GLdouble v1,
    GLdouble v2,
    GLdouble v3)
{
    EXEC_GL_FUNC(glVertexAttrib4d(index, v0, v1, v2, v3));
}

void wsp::opengl::glproxy::detail::VertexAttrib4Nub(GLuint index,
    GLubyte v0,
    GLubyte v1,
    GLubyte v2,
    GLubyte v3)
{
    EXEC_GL_FUNC(glVertexAttrib4Nub(index, v0, v1, v2, v3));
}

void wsp::opengl::glproxy::detail::VertexAttribI4i(GLuint index,
    GLint v0,
    GLint v1,
    GLint v2,
    GLint v3)
{
    EXEC_GL_FUNC(glVertexAttribI4i(index, v0, v1, v2, v3));
}

void wsp::opengl::glproxy::detail::VertexAttribI4ui(GLuint index,
    GLuint v0,
    GLuint v1,
    GLuint v2,
    GLuint v3)
{
    EXEC_GL_FUNC(glVertexAttribI4ui(index, v0, v1, v2, v3));
}

void wsp::opengl::glproxy::detail::Ortho2D(GLdouble left,
    GLdouble right,
    GLdouble bottom,
    GLdouble top)
{
    EXEC_GL_FUNC(gluOrtho2D(left, right, bottom, top));
}

void wsp::opengl::glproxy::detail::BitmapCharacter(void *font, int character)
{
    EXEC_GL_FUNC(glutBitmapCharacter(font, character));
}

void wsp::opengl::glproxy::detail::BindAttribLocation(GLuint program,
    GLuint index,
    const GLchar *name)
{
    EXEC_GL_FUNC(glBindAttribLocation(program, index, name));
}

void wsp::opengl::glproxy::detail::BindFragDataLocation(GLuint program,
    GLuint colorNumber,
    const char * name)
{
    EXEC_GL_FUNC(glBindFragDataLocation(program, colorNumber, name));
}

void wsp::opengl::glproxy::detail::BeginQuery(GLenum target,
    GLuint id)
{
    EXEC_GL_FUNC(glBeginQuery(target, id));
}

void wsp::opengl::glproxy::detail::EndQuery(GLenum target)
{
    EXEC_GL_FUNC(glEndQuery(target));
}

void wsp::opengl::glproxy::detail::WaitSync(GLsync sync,
    GLbitfield flags,
    GLuint64 timeout)
{
    EXEC_GL_FUNC(glWaitSync(sync, flags, timeout));
}

GLenum wsp::opengl::glproxy::detail::ClientWaitSync(GLsync sync,
    GLbitfield flags,
    GLuint64 timeout)
{
    EXEC_GL_FUNC_RET(glClientWaitSync(sync, flags, timeout));
}

void wsp::opengl::glproxy::detail::DepthRange(GLdouble nearVal, GLdouble farVal)
{
    EXEC_GL_FUNC(glDepthRange(nearVal, farVal));
}

void wsp::opengl::glproxy::detail::DepthRangef(GLfloat nearVal, GLfloat farVal)
{
    EXEC_GL_FUNC(glDepthRangef(nearVal, farVal));
}

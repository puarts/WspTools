
#include <wsp/opengl/GlProxy.h>
#include <wsp/common/fn-debug.h>
#include "GlCommands.h"

#ifdef _WIN32
#include <GL/glut.h>
#else
#include <GLUT/GLUT.h>
#endif

#define EXEC_GL_FUNC(gl_func) do { gl_func; GLenum gl_status = glGetError(); WSP_ASSERT(gl_status == GL_NO_ERROR, #gl_func " failed with %s", gluErrorString(gl_status)); } while(false)

using namespace wsp::opengl;

namespace {
    wsp::opengl::GpuCommandQueue* g_current_command_queue = nullptr;
}

void wsp::opengl::glproxy::SetCurrentCommandQueue(wsp::opengl::GpuCommandQueue* command_queue)
{
    g_current_command_queue = command_queue;
}

wsp::opengl::GpuCommandQueue* wsp::opengl::glproxy::GetCurrentCommandQueue()
{
    return g_current_command_queue;
}

void wsp::opengl::glproxy::ExecuteCommands()
{
    if (g_current_command_queue == nullptr)
    {
        return;
    }

    g_current_command_queue->Execute();
}

void wsp::opengl::glproxy::BindFramebuffer(GLenum target, GLuint framebuffer)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<BindFramebufferArg>({ target, framebuffer });
    }
    else
    {
        EXEC_GL_FUNC(glBindFramebufferEXT(target, framebuffer));
    }
}

void wsp::opengl::glproxy::DrawBuffer(GLenum buf)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<DrawBufferArg>({ buf });
    }
    else
    {
        EXEC_GL_FUNC(glDrawBuffer(buf));
    }
}

void wsp::opengl::glproxy::DrawBuffers(GLsizei n, const GLenum *bufs)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<DrawBuffersArg>({ n, bufs });
    }
    else
    {
        EXEC_GL_FUNC(glDrawBuffers(n, bufs));
    }
}

void wsp::opengl::glproxy::DepthMask(GLboolean flag)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<DepthMaskArg>({ flag });
    }
    else
    {
        EXEC_GL_FUNC(glDepthMask(flag));
    }
}

void wsp::opengl::glproxy::Enable(GLenum cap)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<EnableArg>({ cap });
    }
    else
    {
        EXEC_GL_FUNC(glEnable(cap));
    }
}

void wsp::opengl::glproxy::Disable(GLenum cap)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<DisableArg>({ cap });
    }
    else
    {
        EXEC_GL_FUNC(glDisable(cap));
    }
}

void wsp::opengl::glproxy::Hint(GLenum target, GLenum mode)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<HintArg>({ target, mode });
    }
    else
    {
        EXEC_GL_FUNC(glHint(target, mode));
    }
}

void wsp::opengl::glproxy::EnableClientState(GLenum cap)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<EnableClientStateArg>({ cap });
    }
    else
    {
        EXEC_GL_FUNC(glEnableClientState(cap));
    }
}

void wsp::opengl::glproxy::DisableClientState(GLenum cap)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<DisableClientStateArg>({ cap });
    }
    else
    {
        EXEC_GL_FUNC(glDisableClientState(cap));
    }
}

void wsp::opengl::glproxy::Clear(GLbitfield mask)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<ClearArg>({ mask });
    }
    else
    {
        EXEC_GL_FUNC(glClear(mask));
    }
}

void wsp::opengl::glproxy::ClearColor(
    GLclampf red,
    GLclampf green,
    GLclampf blue,
    GLclampf alpha)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<ClearColorArg>({ red, green, blue, alpha });
    }
    else
    {
        EXEC_GL_FUNC(glClearColor(red, green, blue, alpha));
    }
}

void wsp::opengl::glproxy::ClearBufferfv(
    GLenum buffer,
    GLint drawbuffer,
    const GLfloat * value)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<ClearBufferfvArg>({ buffer, drawbuffer, value });
    }
    else
    {
        EXEC_GL_FUNC(glClearBufferfv(buffer, drawbuffer, value));
    }
}

void wsp::opengl::glproxy::ClearDepth(GLdouble depth)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<ClearDepthArg>({ depth });
    }
    else
    {
        EXEC_GL_FUNC(glClearDepth(depth));
    }
}

void wsp::opengl::glproxy::BindTexture(
    GLenum target,
    GLuint texture)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<BindTextureArg>({ target, texture });
    }
    else
    {
        EXEC_GL_FUNC(glBindTexture(target, texture));
    }
}

void wsp::opengl::glproxy::TexImage2D(
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
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<TexImage2DArg>({ target, level, internalFormat, width, height, border, format, type, data });
    }
    else
    {
        EXEC_GL_FUNC(glTexImage2D(target, level, internalFormat, width, height, border, format, type, data));
    }
}

void wsp::opengl::glproxy::TexParameteri(
    GLenum target,
    GLenum pname,
    GLint param)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<TexParameteriArg>({ target, pname, param });
    }
    else
    {
        EXEC_GL_FUNC(glTexParameteri(target, pname, param));
    }
}

void wsp::opengl::glproxy::ReadBuffer(GLenum mode)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<ReadBufferArg>({ mode });
    }
    else
    {
        EXEC_GL_FUNC(glReadBuffer(mode));
    }
}

void wsp::opengl::glproxy::BlitFramebuffer(
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
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<BlitFramebufferArg>({
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
        });
    }
    else
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
}

void wsp::opengl::glproxy::ReadPixels(
    GLint  	    x,
    GLint  	    y,
    GLsizei  	width,
    GLsizei  	height,
    GLenum  	format,
    GLenum  	type,
    GLvoid *  	data)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<ReadPixelsArg>({
            x,
            y,
            width,
            height,
            format,
            type,
            data
        });
    }
    else
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
}

void wsp::opengl::glproxy::BindBuffer(GLenum target,
    GLuint buffer)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<BindBufferArg>({ target, buffer });
    }
    else
    {
        EXEC_GL_FUNC(glBindBuffer(target, buffer));
    }
}

void wsp::opengl::glproxy::VertexAttribPointer(GLuint index,
    GLint size,
    GLenum type,
    GLboolean normalized,
    GLsizei stride,
    const GLvoid * pointer)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<VertexAttribPointerArg>({ index, size, type, normalized, stride, pointer });
    }
    else
    {
        EXEC_GL_FUNC(glVertexAttribPointer(index, size, type, normalized, stride, pointer));
    }
}

void wsp::opengl::glproxy::VertexAttribIPointer(GLuint index,
    GLint size,
    GLenum type,
    GLsizei stride,
    const GLvoid * pointer)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<VertexAttribIPointerArg>({ index, size, type, stride, pointer });
    }
    else
    {
        EXEC_GL_FUNC(glVertexAttribIPointer(index, size, type, stride, pointer));
    }
}


void wsp::opengl::glproxy::BufferData(GLenum target,
    GLsizeiptr size,
    const GLvoid * data,
    GLenum usage)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<BufferDataArg>({ target, size, data, usage });
    }
    else
    {
        EXEC_GL_FUNC(glBufferData(target, size, data, usage));
    }
}

void wsp::opengl::glproxy::BindBufferBase(GLenum target,
    GLuint index,
    GLuint buffer)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<BindBufferBaseArg>({ target, index, buffer });
    }
    else
    {
        EXEC_GL_FUNC(glBindBufferBase(target, index, buffer));
    }
}

void wsp::opengl::glproxy::EnableVertexAttribArray(GLuint index)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<EnableVertexAttribArrayArg>({ index });
    }
    else
    {
        EXEC_GL_FUNC(glEnableVertexAttribArray(index));
    }
}

void wsp::opengl::glproxy::DisableVertexAttribArray(GLuint index)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<DisableVertexAttribArrayArg>({ index });
    }
    else
    {
        EXEC_GL_FUNC(glDisableVertexAttribArray(index));
    }
}

void wsp::opengl::glproxy::BindVertexArray(GLuint array)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<BindVertexArrayArg>({ array });
    }
    else
    {
        EXEC_GL_FUNC(glBindVertexArray(array));
    }
}

void wsp::opengl::glproxy::DrawElements(GLenum mode,
    GLsizei count,
    GLenum type,
    const GLvoid * indices)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<DrawElementsArg>({ mode, count, type, indices });
    }
    else
    {
        EXEC_GL_FUNC(glDrawElements(mode, count, type, indices));
    }
}

void wsp::opengl::glproxy::VertexPointer(GLint  	size,
    GLenum  	type,
    GLsizei  	stride,
    const GLvoid *  	pointer)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<VertexPointerArg>({ size, type, stride, pointer });
    }
    else
    {
        EXEC_GL_FUNC(glVertexPointer(size, type, stride, pointer));
    }
}

void wsp::opengl::glproxy::ActiveTexture(GLenum texture)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<ActiveTextureArg>({ texture });
    }
    else
    {
        EXEC_GL_FUNC(glActiveTextureARB(texture));
    }
}

void wsp::opengl::glproxy::FramebufferTexture2D(GLenum target,
    GLenum attachment,
    GLenum textarget,
    GLuint texture,
    GLint level)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<FramebufferTexture2DArg>({ target, attachment, textarget, texture, level });
    }
    else
    {
        EXEC_GL_FUNC(glFramebufferTexture2D(target, attachment, textarget, texture, level));
    }
}

void wsp::opengl::glproxy::UseProgram(GLuint program)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<UseProgramArg>({ program });
    }
    else
    {
        EXEC_GL_FUNC(glUseProgram(program));
    }
}

void wsp::opengl::glproxy::Viewport(GLint x,
    GLint y,
    GLsizei width,
    GLsizei height)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<ViewportArg>({ x, y, width, height });
    }
    else
    {
        EXEC_GL_FUNC(glViewport(x, y, width, height));
    }
}

void wsp::opengl::glproxy::DrawArrays(GLenum mode,
    GLint first,
    GLsizei count)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<DrawArraysArg>({ mode, first, count });
    }
    else
    {
        EXEC_GL_FUNC(glDrawArrays(mode, first, count));
    }
}

void wsp::opengl::glproxy::Flush(void)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<FlushArg>({ });
    }
    else
    {
        EXEC_GL_FUNC(glFlush());
    }
}

void wsp::opengl::glproxy::Finish(void)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<FinishArg>({});
    }
    else
    {
        EXEC_GL_FUNC(glFinish());
    }
}

void wsp::opengl::glproxy::BlendEquation(GLenum mode)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<BlendEquationArg>({ mode });
    }
    else
    {
        EXEC_GL_FUNC(glBlendEquation(mode));
    }
}

void wsp::opengl::glproxy::BlendFunc(GLenum sfactor,
    GLenum dfactor)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<BlendFuncArg>({ sfactor, dfactor });
    }
    else
    {
        EXEC_GL_FUNC(glBlendFunc(sfactor, dfactor));
    }
}

void wsp::opengl::glproxy::DepthFunc(GLenum func)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<DepthFuncArg>({ func });
    }
    else
    {
        EXEC_GL_FUNC(glDepthFunc(func));
    }
}

void wsp::opengl::glproxy::ColorMask(GLboolean red,
    GLboolean green,
    GLboolean blue,
    GLboolean alpha)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<ColorMaskArg>({ red, green, blue, alpha });
    }
    else
    {
        EXEC_GL_FUNC(glColorMask(red, green, blue, alpha));
    }
}

void wsp::opengl::glproxy::PolygonOffset(GLfloat factor,
    GLfloat units)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<PolygonOffsetArg>({ factor, units });
    }
    else
    {
        EXEC_GL_FUNC(glPolygonOffset(factor, units));
    }
}

void wsp::opengl::glproxy::CullFace(GLenum mode)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<CullFaceArg>({ mode });
    }
    else
    {
        EXEC_GL_FUNC(glCullFace(mode));
    }
}

void wsp::opengl::glproxy::PixelStorei(GLenum pname,
    GLint param)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<PixelStoreiArg>({ pname, param });
    }
    else
    {
        EXEC_GL_FUNC(glPixelStorei(pname, param));
    }
}

void wsp::opengl::glproxy::LineWidth(GLfloat width)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<LineWidthArg>({ width });
    }
    else
    {
        EXEC_GL_FUNC(glLineWidth(width));
    }
}

void wsp::opengl::glproxy::BindRenderbuffer(GLenum target,
    GLuint renderbuffer)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<BindRenderbufferArg>({ target, renderbuffer });
    }
    else
    {
        EXEC_GL_FUNC(glBindRenderbuffer(target, renderbuffer));
    }
}

void wsp::opengl::glproxy::RenderbufferStorage(GLenum target,
    GLenum internalformat,
    GLsizei width,
    GLsizei height)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<RenderbufferStorageArg>({ target, internalformat, width, height });
    }
    else
    {
        EXEC_GL_FUNC(glRenderbufferStorage(target, internalformat, width, height));
    }
}

void wsp::opengl::glproxy::FramebufferRenderbuffer(GLenum target,
    GLenum attachment,
    GLenum renderbuffertarget,
    GLuint renderbuffer)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<FramebufferRenderbufferArg>({ target, attachment, renderbuffertarget, renderbuffer });
    }
    else
    {
        EXEC_GL_FUNC(glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer));
    }
}

void wsp::opengl::glproxy::PushMatrix(void)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<PushMatrixArg>({ });
    }
    else
    {
        EXEC_GL_FUNC(glPushMatrix());
    }
}

//void wsp::opengl::glproxy::Translatef(GLfloat  	x,
//    GLfloat  	y,
//    GLfloat  	z)
//{
//    EXEC_GL_FUNC(glTranslatef(x, y, z));
//}
//
//void wsp::opengl::glproxy::Scalef(GLfloat  	x,
//    GLfloat  	y,
//    GLfloat  	z)
//{
//    EXEC_GL_FUNC(glScalef(x, y, z));
//}
//
//void wsp::opengl::glproxy::Rotatef(GLfloat angle,
//    GLfloat x,
//    GLfloat y,
//    GLfloat z)
//{
//    EXEC_GL_FUNC(glRotatef(angle, x, y, z));
//}

void wsp::opengl::glproxy::MatrixMode(GLenum mode)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<MatrixModeArg>({ mode });
    }
    else
    {
        EXEC_GL_FUNC(glMatrixMode(mode));
    }
}

//void wsp::opengl::glproxy::NormalPointer(GLenum  	type,
//    GLsizei  	stride,
//    const GLvoid *  	pointer)
//{
//    EXEC_GL_FUNC(glNormalPointer(type, stride, pointer));
//}
//
//void wsp::opengl::glproxy::ColorPointer(GLint size,
//    GLenum type,
//    GLsizei stride,
//    const GLvoid * pointer)
//{
//    EXEC_GL_FUNC(glColorPointer(size, type, stride, pointer));
//}

void wsp::opengl::glproxy::TexCoordPointer(GLint size,
    GLenum type,
    GLsizei stride,
    const GLvoid * pointer)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<TexCoordPointerArg>({ size, type, stride, pointer });
    }
    else
    {
        EXEC_GL_FUNC(glTexCoordPointer(size, type, stride, pointer));
    }
}

void wsp::opengl::glproxy::PopMatrix(void)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<PopMatrixArg>({ });
    }
    else
    {
        EXEC_GL_FUNC(glPopMatrix());
    }
}


void wsp::opengl::glproxy::UniformMatrix3f(GLint location,
    GLboolean transpose,
    const GLfloat *value)
{
    if (g_current_command_queue)
    {
        UniformMatrix3fArg arg = { location, transpose };
        memcpy(arg.value, value, sizeof(GLfloat) * 9);
        g_current_command_queue->EnqueueByArg<UniformMatrix3fArg>(arg);
    }
    else
    {
        EXEC_GL_FUNC(glUniformMatrix3fv(location, 1, transpose, value));
    }
}

void wsp::opengl::glproxy::UniformMatrix3fv(GLint location,
    GLsizei count,
    GLboolean transpose,
    const GLfloat *value)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<UniformMatrix3fvArg>({ location, count, transpose, value });
    }
    else
    {
        EXEC_GL_FUNC(glUniformMatrix3fv(location, count, transpose, value));
    }
}

//void wsp::opengl::glproxy::TexEnvf(GLenum target,
//    GLenum pname,
//    GLfloat param)
//{
//    EXEC_GL_FUNC(glTexEnvf(target, pname, param));
//}
//
//void wsp::opengl::glproxy::TexEnvi(GLenum target,
//    GLenum pname,
//    GLint param)
//{
//    EXEC_GL_FUNC(glTexEnvi(target, pname, param));
//}
//
//void wsp::opengl::glproxy::Begin(GLenum  	mode)
//{
//    EXEC_GL_FUNC(glBegin(mode));
//}
//
//void wsp::opengl::glproxy::Normal3d(GLdouble nx,
//    GLdouble ny,
//    GLdouble nz)
//{
//    EXEC_GL_FUNC(glNormal3d(nx, ny, nz));
//}
//
//void wsp::opengl::glproxy::Normal3f(GLfloat nx,
//    GLfloat ny,
//    GLfloat nz)
//{
//    EXEC_GL_FUNC(glNormal3f(nx, ny, nz));
//}
//
//void wsp::opengl::glproxy::Vertex2f(GLfloat x,
//    GLfloat y)
//{
//    EXEC_GL_FUNC(glVertex2f(x, y));
//}
//
//void wsp::opengl::glproxy::Vertex3fv(
//    const GLfloat *v)
//{
//    EXEC_GL_FUNC(glVertex3fv(v));
//}
//
//void wsp::opengl::glproxy::End()
//{
//    EXEC_GL_FUNC(glEnd());
//}

void wsp::opengl::glproxy::PolygonMode(GLenum face,
    GLenum mode)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<PolygonModeArg>({ face, mode });
    }
    else
    {
        EXEC_GL_FUNC(glPolygonMode(face, mode));
    }
}

//void wsp::opengl::glproxy::Materialf(GLenum  	face,
//    GLenum  	pname,
//    GLfloat  	param)
//{
//    EXEC_GL_FUNC(glMaterialf(face, pname, param));
//}
//
//void wsp::opengl::glproxy::Materialfv(GLenum  	face,
//    GLenum  	pname,
//    const GLfloat *  	params)
//{
//    EXEC_GL_FUNC(glMaterialfv(face, pname, params));
//}
//
//void wsp::opengl::glproxy::Color3fv(
//    const GLfloat *v)
//{
//    EXEC_GL_FUNC(glColor3fv(v));
//}
//
//void wsp::opengl::glproxy::Lightfv(GLenum light,
//    GLenum pname,
//    const GLfloat * params)
//{
//    EXEC_GL_FUNC(glLightfv(light, pname, params));
//}

void wsp::opengl::glproxy::TexParameterf(GLenum target,
    GLenum pname,
    GLfloat param)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<TexParameterfArg>({ target, pname, param });
    }
    else
    {
        EXEC_GL_FUNC(glTexParameterf(target, pname, param));
    }
}

void wsp::opengl::glproxy::TexParameterfv(GLenum target,
    GLenum pname,
    const GLfloat* params)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<TexParameterfvArg>({ target, pname, params });
    }
    else
    {
        EXEC_GL_FUNC(glTexParameterfv(target, pname, params));
    }
}


void wsp::opengl::glproxy::FramebufferTexture(GLenum target,
    GLenum attachment,
    GLuint texture,
    GLint level)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<FramebufferTextureArg>({ target, attachment, texture, level });
    }
    else
    {
        EXEC_GL_FUNC(glFramebufferTexture(target, attachment, texture, level));
    }
}

//void wsp::opengl::glproxy::Vertex3d(
//    GLdouble x,
//    GLdouble y,
//    GLdouble z)
//{
//    EXEC_GL_FUNC(glVertex3d(x, y, z));
//}
//
void wsp::opengl::glproxy::LoadIdentity(void)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<LoadIdentityArg>({ });
    }
    else
    {
        EXEC_GL_FUNC(glLoadIdentity());
    }
}

void wsp::opengl::glproxy::Color4f(GLfloat  	red,
    GLfloat  	green,
    GLfloat  	blue,
    GLfloat  	alpha)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<Color4fArg>({ red, green, blue, alpha });
    }
    else
    {
        EXEC_GL_FUNC(glColor4f(red, green, blue, alpha));
    }
}

void wsp::opengl::glproxy::RasterPos2i(
    GLint x,
    GLint y)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<RasterPos2iArg>({ x, y });
    }
    else
    {
        EXEC_GL_FUNC(glRasterPos2i(x, y));
    }
}

void wsp::opengl::glproxy::AlphaFunc(GLenum  	func,
    GLclampf  	ref)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<AlphaFuncArg>({ func, ref });
    }
    else
    {
        EXEC_GL_FUNC(glAlphaFunc(func, ref));
    }
}

void wsp::opengl::glproxy::Uniform1f(GLint location,
    GLfloat v0)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<Uniform1fArg>({ location, v0 });
    }
    else
    {
        EXEC_GL_FUNC(glUniform1f(location, v0));
    }
}

void wsp::opengl::glproxy::Uniform2f(GLint location,
    GLfloat v0,
    GLfloat v1)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<Uniform2fArg>({ location, v0, v1 });
    }
    else
    {
        EXEC_GL_FUNC(glUniform2f(location, v0, v1));
    }
}

void wsp::opengl::glproxy::Uniform3f(GLint location,
    GLfloat v0,
    GLfloat v1,
    GLfloat v2)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<Uniform3fArg>({ location, v0, v1, v2 });
    }
    else
    {
        EXEC_GL_FUNC(glUniform3f(location, v0, v1, v2));
    }
}

void wsp::opengl::glproxy::Uniform4f(GLint location,
    GLfloat v0,
    GLfloat v1,
    GLfloat v2,
    GLfloat v3)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<Uniform4fArg>({ location, v0, v1, v2, v3 });
    }
    else
    {
        EXEC_GL_FUNC(glUniform4f(location, v0, v1, v2, v3));
    }
}

void wsp::opengl::glproxy::Uniform1i(GLint location,
    GLint v0)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<Uniform1iArg>({ location, v0 });
    }
    else
    {
        EXEC_GL_FUNC(glUniform1i(location, v0));
    }
}

void wsp::opengl::glproxy::Uniform2i(GLint location,
    GLint v0,
    GLint v1)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<Uniform2iArg>({ location, v0, v1 });
    }
    else
    {
        EXEC_GL_FUNC(glUniform2i(location, v0, v1));
    }
}

void wsp::opengl::glproxy::Uniform3i(GLint location,
    GLint v0,
    GLint v1,
    GLint v2)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<Uniform3iArg>({ location, v0, v1, v2 });
    }
    else
    {
        EXEC_GL_FUNC(glUniform3i(location, v0, v1, v2));
    }
}

void wsp::opengl::glproxy::Uniform4i(GLint location,
    GLint v0,
    GLint v1,
    GLint v2,
    GLint v3)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<Uniform4iArg>({ location, v0, v1, v2, v3 });
    }
    else
    {
        EXEC_GL_FUNC(glUniform4i(location, v0, v1, v2, v3));
    }
}

void wsp::opengl::glproxy::Uniform1ui(GLint location,
    GLuint v0)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<Uniform1uiArg>({ location, v0 });
    }
    else
    {
        EXEC_GL_FUNC(glUniform1ui(location, v0));
    }
}

void wsp::opengl::glproxy::Uniform2ui(GLint location,
    GLuint v0,
    GLuint v1)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<Uniform2uiArg>({ location, v0, v1 });
    }
    else
    {
        EXEC_GL_FUNC(glUniform2ui(location, v0, v1));
    }
}

void wsp::opengl::glproxy::Uniform3ui(GLint location,
    GLuint v0,
    GLuint v1,
    GLuint v2)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<Uniform3uiArg>({ location, v0, v1, v2 });
    }
    else
    {
        EXEC_GL_FUNC(glUniform3ui(location, v0, v1, v2));
    }
}

void wsp::opengl::glproxy::Uniform4ui(GLint location,
    GLuint v0,
    GLuint v1,
    GLuint v2,
    GLuint v3)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<Uniform4uiArg>({ location, v0, v1, v2, v3 });
    }
    else
    {
        EXEC_GL_FUNC(glUniform4ui(location, v0, v1, v2, v3));
    }
}

void wsp::opengl::glproxy::Uniform1fv(GLint location,
    GLsizei count,
    const GLfloat *value)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<Uniform1fvArg>({ location, count, value });
    }
    else
    {
        EXEC_GL_FUNC(glUniform1fv(location, count, value));
    }
}

void wsp::opengl::glproxy::Uniform2fv(GLint location,
    GLsizei count,
    const GLfloat *value)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<Uniform2fvArg>({ location, count, value });
    }
    else
    {
        EXEC_GL_FUNC(glUniform2fv(location, count, value));
    }
}

void wsp::opengl::glproxy::Uniform3fv(GLint location,
    GLsizei count,
    const GLfloat *value)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<Uniform3fvArg>({ location, count, value });
    }
    else
    {
        EXEC_GL_FUNC(glUniform3fv(location, count, value));
    }
}

void wsp::opengl::glproxy::Uniform4fv(GLint location,
    GLsizei count,
    const GLfloat *value)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<Uniform4fvArg>({ location, count, value });
    }
    else
    {
        EXEC_GL_FUNC(glUniform4fv(location, count, value));
    }
}

void wsp::opengl::glproxy::Uniform1iv(GLint location,
    GLsizei count,
    const GLint *value)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<Uniform1ivArg>({ location, count, value });
    }
    else
    {
        EXEC_GL_FUNC(glUniform1iv(location, count, value));
    }
}

void wsp::opengl::glproxy::Uniform2iv(GLint location,
    GLsizei count,
    const GLint *value)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<Uniform2ivArg>({ location, count, value });
    }
    else
    {
        EXEC_GL_FUNC(glUniform2iv(location, count, value));
    }
}

void wsp::opengl::glproxy::Uniform3iv(GLint location,
    GLsizei count,
    const GLint *value)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<Uniform3ivArg>({ location, count, value });
    }
    else
    {
        EXEC_GL_FUNC(glUniform3iv(location, count, value));
    }
}

void wsp::opengl::glproxy::Uniform4iv(GLint location,
    GLsizei count,
    const GLint *value)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<Uniform4ivArg>({ location, count, value });
    }
    else
    {
        EXEC_GL_FUNC(glUniform4iv(location, count, value));
    }
}

void wsp::opengl::glproxy::Uniform1uiv(GLint location,
    GLsizei count,
    const GLuint *value)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<Uniform1uivArg>({ location, count, value });
    }
    else
    {
        EXEC_GL_FUNC(glUniform1uiv(location, count, value));
    }
}

void wsp::opengl::glproxy::Uniform2uiv(GLint location,
    GLsizei count,
    const GLuint *value)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<Uniform2uivArg>({ location, count, value });
    }
    else
    {
        EXEC_GL_FUNC(glUniform2uiv(location, count, value));
    }
}

void wsp::opengl::glproxy::Uniform3uiv(GLint location,
    GLsizei count,
    const GLuint *value)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<Uniform3uivArg>({ location, count, value });
    }
    else
    {
        EXEC_GL_FUNC(glUniform3uiv(location, count, value));
    }
}

void wsp::opengl::glproxy::Uniform4uiv(GLint location,
    GLsizei count,
    const GLuint *value)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<Uniform4uivArg>({ location, count, value });
    }
    else
    {
        EXEC_GL_FUNC(glUniform4uiv(location, count, value));
    }
}

void wsp::opengl::glproxy::UniformMatrix2fv(GLint location,
    GLsizei count,
    GLboolean transpose,
    const GLfloat *value)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<UniformMatrix2fvArg>({ location, count, transpose, value });
    }
    else
    {
        EXEC_GL_FUNC(glUniformMatrix2fv(location, count, transpose, value));
    }
}

void wsp::opengl::glproxy::UniformMatrix4f(GLint location,
    GLboolean transpose,
    const GLfloat *value)
{
    if (g_current_command_queue)
    {
        UniformMatrix4fArg arg = {location, transpose};
        memcpy(arg.value, value, sizeof(GLfloat) * 16);
        g_current_command_queue->EnqueueByArg<UniformMatrix4fArg>(arg);
    }
    else
    {
        EXEC_GL_FUNC(glUniformMatrix4fv(location, 1, transpose, value));
    }
}

void wsp::opengl::glproxy::UniformMatrix4fv(GLint location,
    GLsizei count,
    GLboolean transpose,
    const GLfloat *value)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<UniformMatrix4fvArg>({ location, count, transpose, value });
    }
    else
    {
        EXEC_GL_FUNC(glUniformMatrix4fv(location, count, transpose, value));
    }
}

void wsp::opengl::glproxy::UniformBlockBinding(GLuint program,
    GLuint uniformBlockIndex,
    GLuint uniformBlockBinding)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<UniformBlockBindingArg>({ program, uniformBlockIndex, uniformBlockBinding });
    }
    else
    {
        EXEC_GL_FUNC(glUniformBlockBinding(program, uniformBlockIndex, uniformBlockBinding));
    }
}


void wsp::opengl::glproxy::VertexAttrib1f(GLuint index,
    GLfloat v0)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<VertexAttrib1fArg>({ index, v0 });
    }
    else
    {
        EXEC_GL_FUNC(glVertexAttrib1f(index, v0));
    }
}

void wsp::opengl::glproxy::VertexAttrib1s(GLuint index,
    GLshort v0)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<VertexAttrib1sArg>({ index, v0 });
    }
    else
    {
        EXEC_GL_FUNC(glVertexAttrib1s(index, v0));
    }
}

void wsp::opengl::glproxy::VertexAttrib1d(GLuint index,
    GLdouble v0)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<VertexAttrib1dArg>({ index, v0 });
    }
    else
    {
        EXEC_GL_FUNC(glVertexAttrib1d(index, v0));
    }
}

void wsp::opengl::glproxy::VertexAttribI1i(GLuint index,
    GLint v0)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<VertexAttribI1iArg>({ index, v0 });
    }
    else
    {
        EXEC_GL_FUNC(glVertexAttribI1i(index, v0));
    }
}

void wsp::opengl::glproxy::VertexAttribI1ui(GLuint index,
    GLuint v0)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<VertexAttribI1uiArg>({ index, v0 });
    }
    else
    {
        EXEC_GL_FUNC(glVertexAttribI1ui(index, v0));
    }
}

void wsp::opengl::glproxy::VertexAttrib2f(GLuint index,
    GLfloat v0,
    GLfloat v1)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<VertexAttrib2fArg>({ index, v0, v1 });
    }
    else
    {
        EXEC_GL_FUNC(glVertexAttrib2f(index, v0, v1));
    }
}

void wsp::opengl::glproxy::VertexAttrib2s(GLuint index,
    GLshort v0,
    GLshort v1)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<VertexAttrib2sArg>({ index, v0, v1 });
    }
    else
    {
        EXEC_GL_FUNC(glVertexAttrib2s(index, v0, v1));
    }
}

void wsp::opengl::glproxy::VertexAttrib2d(GLuint index,
    GLdouble v0,
    GLdouble v1)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<VertexAttrib2dArg>({ index, v0, v1 });
    }
    else
    {
        EXEC_GL_FUNC(glVertexAttrib2d(index, v0, v1));
    }
}

void wsp::opengl::glproxy::VertexAttribI2i(GLuint index,
    GLint v0,
    GLint v1)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<VertexAttribI2iArg>({ index, v0, v1 });
    }
    else
    {
        EXEC_GL_FUNC(glVertexAttribI2i(index, v0, v1));
    }
}

void wsp::opengl::glproxy::VertexAttribI2ui(GLuint index,
    GLuint v0,
    GLuint v1)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<VertexAttribI2uiArg>({ index, v0, v1 });
    }
    else
    {
        EXEC_GL_FUNC(glVertexAttribI2ui(index, v0, v1));
    }
}

void wsp::opengl::glproxy::VertexAttrib3f(GLuint index,
    GLfloat v0,
    GLfloat v1,
    GLfloat v2)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<VertexAttrib3fArg>({ index, v0, v1, v2 });
    }
    else
    {
        EXEC_GL_FUNC(glVertexAttrib3f(index, v0, v1, v2));
    }
}

void wsp::opengl::glproxy::VertexAttrib3s(GLuint index,
    GLshort v0,
    GLshort v1,
    GLshort v2)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<VertexAttrib3sArg>({ index, v0, v1, v2 });
    }
    else
    {
        EXEC_GL_FUNC(glVertexAttrib3s(index, v0, v1, v2));
    }
}

void wsp::opengl::glproxy::VertexAttrib3d(GLuint index,
    GLdouble v0,
    GLdouble v1,
    GLdouble v2)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<VertexAttrib3dArg>({ index, v0, v1, v2 });
    }
    else
    {
        EXEC_GL_FUNC(glVertexAttrib3d(index, v0, v1, v2));
    }
}

void wsp::opengl::glproxy::VertexAttribI3i(GLuint index,
    GLint v0,
    GLint v1,
    GLint v2)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<VertexAttribI3iArg>({ index, v0, v1, v2 });
    }
    else
    {
        EXEC_GL_FUNC(glVertexAttribI3i(index, v0, v1, v2));
    }
}

void wsp::opengl::glproxy::VertexAttribI3ui(GLuint index,
    GLuint v0,
    GLuint v1,
    GLuint v2)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<VertexAttribI3uiArg>({ index, v0, v1, v2 });
    }
    else
    {
        EXEC_GL_FUNC(glVertexAttribI3ui(index, v0, v1, v2));
    }
}

void wsp::opengl::glproxy::VertexAttrib4f(GLuint index,
    GLfloat v0,
    GLfloat v1,
    GLfloat v2,
    GLfloat v3)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<VertexAttrib4fArg>({ index, v0, v1, v2, v3 });
    }
    else
    {
        EXEC_GL_FUNC(glVertexAttrib4f(index, v0, v1, v2, v3));
    }
}

void wsp::opengl::glproxy::VertexAttrib4s(GLuint index,
    GLshort v0,
    GLshort v1,
    GLshort v2,
    GLshort v3)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<VertexAttrib4sArg>({ index, v0, v1, v2, v3 });
    }
    else
    {
        EXEC_GL_FUNC(glVertexAttrib4s(index, v0, v1, v2, v3));
    }
}

void wsp::opengl::glproxy::VertexAttrib4d(GLuint index,
    GLdouble v0,
    GLdouble v1,
    GLdouble v2,
    GLdouble v3)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<VertexAttrib4dArg>({ index, v0, v1, v2, v3 });
    }
    else
    {
        EXEC_GL_FUNC(glVertexAttrib4d(index, v0, v1, v2, v3));
    }
}

void wsp::opengl::glproxy::VertexAttrib4Nub(GLuint index,
    GLubyte v0,
    GLubyte v1,
    GLubyte v2,
    GLubyte v3)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<VertexAttrib4NubArg>({ index, v0, v1, v2, v3 });
    }
    else
    {
        EXEC_GL_FUNC(glVertexAttrib4Nub(index, v0, v1, v2, v3));
    }
}

void wsp::opengl::glproxy::VertexAttribI4i(GLuint index,
    GLint v0,
    GLint v1,
    GLint v2,
    GLint v3)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<VertexAttribI4iArg>({ index, v0, v1, v2, v3 });
    }
    else
    {
        EXEC_GL_FUNC(glVertexAttribI4i(index, v0, v1, v2, v3));
    }
}

void wsp::opengl::glproxy::VertexAttribI4ui(GLuint index,
    GLuint v0,
    GLuint v1,
    GLuint v2,
    GLuint v3)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<VertexAttribI4uiArg>({ index, v0, v1, v2, v3 });
    }
    else
    {
        EXEC_GL_FUNC(glVertexAttribI4ui(index, v0, v1, v2, v3));
    }
}

void wsp::opengl::glproxy::Ortho2D(GLdouble left,
    GLdouble right,
    GLdouble bottom,
    GLdouble top)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<Ortho2DArg>({ left, right, bottom, top });
    }
    else
    {
        EXEC_GL_FUNC(gluOrtho2D(left, right, bottom, top));
    }
}

void wsp::opengl::glproxy::BitmapCharacter(void *font, int character)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<BitmapCharacterArg>({ font, character });
    }
    else
    {
        EXEC_GL_FUNC(glutBitmapCharacter(font, character));
    }
}

void wsp::opengl::glproxy::BindAttribLocation(GLuint program,
    GLuint index,
    const GLchar *name)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<BindAttribLocationArg>({ program, index, name });
    }
    else
    {
        EXEC_GL_FUNC(glBindAttribLocation(program, index, name));
    }
}

void wsp::opengl::glproxy::BindFragDataLocation(GLuint program,
    GLuint colorNumber,
    const char * name)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<BindFragDataLocationArg>({ program, colorNumber, name });
    }
    else
    {
        EXEC_GL_FUNC(glBindFragDataLocation(program, colorNumber, name));
    }
}

void wsp::opengl::glproxy::BeginQuery(GLenum target,
    GLuint id)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<BeginQueryArg>({ target, id });
    }
    else
    {
        EXEC_GL_FUNC(glBeginQuery(target, id));
    }
}

void wsp::opengl::glproxy::EndQuery(GLenum target)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<EndQueryArg>({ target });
    }
    else
    {
        EXEC_GL_FUNC(glEndQuery(target));
    }
}

void wsp::opengl::glproxy::WaitSync(GLsync sync,
    GLbitfield flags,
    GLuint64 timeout)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<WaitSyncArg>({ sync, flags, timeout });
    }
    else
    {
        EXEC_GL_FUNC(glWaitSync(sync, flags, timeout));
    }
}

void wsp::opengl::glproxy::ClientWaitSync(GLsync sync,
    GLbitfield flags,
    GLuint64 timeout)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<ClientWaitSyncArg>({ sync, flags, timeout });
    }
    else
    {
        EXEC_GL_FUNC(glClientWaitSync(sync, flags, timeout));
    }
}

void wsp::opengl::glproxy::DepthRange(GLdouble nearVal, GLdouble farVal)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<DepthRangeArg>({ nearVal, farVal });
    }
    else
    {
        EXEC_GL_FUNC(glDepthRange(nearVal, farVal));
    }
}

void wsp::opengl::glproxy::DepthRangef(GLfloat nearVal, GLfloat farVal)
{
    if (g_current_command_queue)
    {
        g_current_command_queue->EnqueueByArg<DepthRangefArg>({ nearVal, farVal });
    }
    else
    {
        EXEC_GL_FUNC(glDepthRangef(nearVal, farVal));
    }
}

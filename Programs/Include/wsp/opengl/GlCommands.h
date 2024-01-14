#pragma once

#include "CommandQueue.h"
#include "GlProxyImpl.h"
#include <gl/glew.h>

namespace wsp {
    namespace opengl {

        struct BindFramebufferArg
        {
            GLenum target;
            GLuint framebuffer;
        };

        template<>
        inline void EnqueueGpuCommand<BindFramebufferArg>(GpuCommandQueue* queue, const BindFramebufferArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<BindFramebufferArg>(arg, [](BindFramebufferArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::BindFramebuffer(exec_arg.target, exec_arg.framebuffer);
            }));
        }

        struct DrawBufferArg
        {
            GLenum buf;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const DrawBufferArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<DrawBufferArg>(arg, [](DrawBufferArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::DrawBuffer(exec_arg.buf);
            }));
        }

        struct DrawBuffersArg
        {
            GLsizei n;
            const GLenum* bufs;
        };

        template<> inline void EnqueueGpuCommand(GpuCommandQueue* queue, const DrawBuffersArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<DrawBuffersArg>(arg, [](DrawBuffersArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::DrawBuffers(exec_arg.n, exec_arg.bufs);
            }));
        }

        struct DepthMaskArg
        {
            GLboolean flag;
        };

        template<> inline void EnqueueGpuCommand(GpuCommandQueue* queue, const DepthMaskArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<DepthMaskArg>(arg, [](DepthMaskArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::DepthMask(exec_arg.flag);
            }));
        }

        struct EnableArg
        {
            GLenum cap;
        };

        template<> inline void EnqueueGpuCommand(GpuCommandQueue* queue, const EnableArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<EnableArg>(arg, [](EnableArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::Enable(exec_arg.cap);
            }));
        }

        struct DisableArg
        {
            GLenum cap;
        };

        template<> inline void EnqueueGpuCommand(GpuCommandQueue* queue, const DisableArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<DisableArg>(arg, [](DisableArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::Disable(exec_arg.cap);
            }));
        }

        struct HintArg
        {
            GLenum target;
            GLenum mode;
        };

        template<> inline void EnqueueGpuCommand(GpuCommandQueue* queue, const HintArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<HintArg>(arg, [](HintArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::Hint(exec_arg.target, exec_arg.mode);
            }));
        }

        struct EnableClientStateArg
        {
            GLenum cap;
        };

        template<> inline void EnqueueGpuCommand(GpuCommandQueue* queue, const EnableClientStateArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<EnableClientStateArg>(arg, [](EnableClientStateArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::EnableClientState(exec_arg.cap);
            }));
        }

        struct DisableClientStateArg
        {
            GLenum cap;
        };

        template<> inline void EnqueueGpuCommand(GpuCommandQueue* queue, const DisableClientStateArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<DisableClientStateArg>(arg, [](DisableClientStateArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::DisableClientState(exec_arg.cap);
            }));
        }

        struct DrawElementsArg
        {
            GLenum mode;
            GLsizei count;
            GLenum type;
            const GLvoid * indices;
        };

        template<> inline void EnqueueGpuCommand(GpuCommandQueue* queue, const DrawElementsArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<DrawElementsArg>(arg, [](DrawElementsArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::DrawElements(
                    exec_arg.mode, exec_arg.count, exec_arg.type, exec_arg.indices);
            }));
        }

        struct ClearArg
        {
            GLbitfield mask;
        };

        template<> inline void EnqueueGpuCommand(GpuCommandQueue* queue, const ClearArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<ClearArg>(arg, [](ClearArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::Clear(exec_arg.mask);
            }));
        }

        struct ClearColorArg
        {
            GLclampf red;
            GLclampf green;
            GLclampf blue;
            GLclampf alpha;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const ClearColorArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<ClearColorArg>(arg, [](ClearColorArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::ClearColor(exec_arg.red, exec_arg.green, exec_arg.blue, exec_arg.alpha);
            }));
        }

        struct ClearDepthArg
        {
            GLdouble depth;
        };

        template<> inline void EnqueueGpuCommand(GpuCommandQueue* queue, const ClearDepthArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<ClearDepthArg>(arg, [](ClearDepthArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::ClearDepth(exec_arg.depth);
            }));
        }

        struct ClearBufferfvArg
        {
            GLenum buffer;
            GLint drawbuffer;
            const GLfloat * value;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const ClearBufferfvArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<ClearBufferfvArg>(arg, [](ClearBufferfvArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::ClearBufferfv(exec_arg.buffer, exec_arg.drawbuffer, exec_arg.value);
            }));
        }

        struct BindTextureArg
        {
            GLenum target;
            GLuint texture;
        };

        template<> inline void EnqueueGpuCommand(GpuCommandQueue* queue, const BindTextureArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<BindTextureArg>(arg, [](BindTextureArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::BindTexture(exec_arg.target, exec_arg.texture);
            }));
        }

        struct TexImage2DArg
        {
            GLenum target;
            GLint level;
            GLint internalFormat;
            GLsizei width;
            GLsizei height;
            GLint border;
            GLenum format;
            GLenum type;
            const GLvoid * data;
        };

        template<> inline void EnqueueGpuCommand(GpuCommandQueue* queue, const TexImage2DArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<TexImage2DArg>(arg, [](TexImage2DArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::TexImage2D(exec_arg.target, exec_arg.level, exec_arg.internalFormat, exec_arg.width, exec_arg.height, exec_arg.border, exec_arg.format, exec_arg.type, exec_arg.data);
            }));
        }

        struct TexParameteriArg
        {
            GLenum target;
            GLenum pname;
            GLint param;
        };

        template<> inline void EnqueueGpuCommand(GpuCommandQueue* queue, const TexParameteriArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<TexParameteriArg>(arg, [](TexParameteriArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::TexParameteri(exec_arg.target, exec_arg.pname, exec_arg.param);
            }));
        }

        struct ReadBufferArg
        {
            GLenum mode;
        };

        template<> inline void EnqueueGpuCommand(GpuCommandQueue* queue, const ReadBufferArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<ReadBufferArg>(arg, [](ReadBufferArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::ReadBuffer(exec_arg.mode);
            }));
        }

        struct BlitFramebufferArg
        {
            GLint srcX0;
            GLint srcY0;
            GLint srcX1;
            GLint srcY1;
            GLint dstX0;
            GLint dstY0;
            GLint dstX1;
            GLint dstY1;
            GLbitfield mask;
            GLenum filter;
        };

        template<> inline void EnqueueGpuCommand(GpuCommandQueue* queue, const BlitFramebufferArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<BlitFramebufferArg>(arg, [](BlitFramebufferArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::BlitFramebuffer(exec_arg.srcX0, exec_arg.srcY0, exec_arg.srcX1, exec_arg.srcY1, exec_arg.dstX0, exec_arg.dstY0, exec_arg.dstX1, exec_arg.dstY1, exec_arg.mask, exec_arg.filter);
            }));
        }

        struct ReadPixelsArg
        {
            GLint  	x;
            GLint  	y;
            GLsizei  	width;
            GLsizei  	height;
            GLenum  	format;
            GLenum  	type;
            GLvoid *  	data;
        };

        template<> inline void EnqueueGpuCommand(GpuCommandQueue* queue, const ReadPixelsArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<ReadPixelsArg>(arg, [](ReadPixelsArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::ReadPixels(exec_arg.x, exec_arg.y, exec_arg.width, exec_arg.height, exec_arg.format, exec_arg.type, exec_arg.data);
            }));
        }

        struct BindBufferArg
        {
            GLenum target;
            GLuint buffer;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const BindBufferArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<BindBufferArg>(arg, [](BindBufferArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::BindBuffer(exec_arg.target, exec_arg.buffer);
            }));
        }

        struct VertexAttribPointerArg
        {
            GLuint index;
            GLint size;
            GLenum type;
            GLboolean normalized;
            GLsizei stride;
            const GLvoid * pointer;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const VertexAttribPointerArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<VertexAttribPointerArg>(arg, [](VertexAttribPointerArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::VertexAttribPointer(exec_arg.index, exec_arg.size, exec_arg.type, exec_arg.normalized, exec_arg.stride, exec_arg.pointer);
            }));
        }

        struct VertexAttribIPointerArg
        {
            GLuint index;
            GLint size;
            GLenum type;
            GLsizei stride;
            const GLvoid * pointer;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const VertexAttribIPointerArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<VertexAttribIPointerArg>(arg, [](VertexAttribIPointerArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::VertexAttribIPointer(exec_arg.index, exec_arg.size, exec_arg.type, exec_arg.stride, exec_arg.pointer);
            }));
        }

        struct BufferDataArg
        {
            GLenum target;
            GLsizeiptr size;
            const GLvoid * data;
            GLenum usage;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const BufferDataArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<BufferDataArg>(arg, [](BufferDataArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::BufferData(exec_arg.target, exec_arg.size, exec_arg.data, exec_arg.usage);
            }));
        }

        struct BindBufferBaseArg
        {
            GLenum target;
            GLuint index;
            GLuint buffer;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const BindBufferBaseArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<BindBufferBaseArg>(arg, [](BindBufferBaseArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::BindBufferBase(exec_arg.target, exec_arg.index, exec_arg.buffer);
            }));
        }

        struct EnableVertexAttribArrayArg
        {
            GLuint index;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const EnableVertexAttribArrayArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<EnableVertexAttribArrayArg>(arg, [](EnableVertexAttribArrayArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::EnableVertexAttribArray(exec_arg.index);
            }));
        }

        struct DisableVertexAttribArrayArg
        {
            GLuint index;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const DisableVertexAttribArrayArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<DisableVertexAttribArrayArg>(arg, [](DisableVertexAttribArrayArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::DisableVertexAttribArray(exec_arg.index);
            }));
        }

        struct BindVertexArrayArg
        {
            GLuint array;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const BindVertexArrayArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<BindVertexArrayArg>(arg, [](BindVertexArrayArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::BindVertexArray(exec_arg.array);
            }));
        }

        struct VertexPointerArg
        {
            GLint  	size;
            GLenum  	type;
            GLsizei  	stride;
            const GLvoid *  	pointer;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const VertexPointerArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<VertexPointerArg>(arg, [](VertexPointerArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::VertexPointer(exec_arg.size, exec_arg.type, exec_arg.stride, exec_arg.pointer);
            }));
        }

        struct ActiveTextureArg
        {
            GLenum texture;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const ActiveTextureArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<ActiveTextureArg>(arg, [](ActiveTextureArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::ActiveTexture(exec_arg.texture);
            }));
        }

        struct FramebufferTexture2DArg
        {
            GLenum target;
            GLenum attachment;
            GLenum textarget;
            GLuint texture;
            GLint level;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const FramebufferTexture2DArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<FramebufferTexture2DArg>(arg, [](FramebufferTexture2DArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::FramebufferTexture2D(exec_arg.target, exec_arg.attachment, exec_arg.textarget, exec_arg.texture, exec_arg.level);
            }));
        }

        struct UseProgramArg
        {
            GLuint program;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const UseProgramArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<UseProgramArg>(arg, [](UseProgramArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::UseProgram(exec_arg.program);
            }));
        }

        struct ViewportArg
        {
            GLint x;
            GLint y;
            GLsizei width;
            GLsizei height;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const ViewportArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<ViewportArg>(arg, [](ViewportArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::Viewport(exec_arg.x, exec_arg.y, exec_arg.width, exec_arg.height);
            }));
        }

        struct DrawArraysArg
        {
            GLenum mode;
            GLint first;
            GLsizei count;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const DrawArraysArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<DrawArraysArg>(arg, [](DrawArraysArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::DrawArrays(exec_arg.mode, exec_arg.first, exec_arg.count);
            }));
        }

        struct FlushArg
        {
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const FlushArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<FlushArg>(arg, [](FlushArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::Flush();
            }));
        }

        struct FinishArg
        {
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const FinishArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<FinishArg>(arg, [](FinishArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::Finish();
            }));
        }

        struct BlendEquationArg
        {
            GLenum mode;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const BlendEquationArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<BlendEquationArg>(arg, [](BlendEquationArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::BlendEquation(exec_arg.mode);
            }));
        }

        struct BlendFuncArg
        {
            GLenum sfactor;
            GLenum dfactor;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const BlendFuncArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<BlendFuncArg>(arg, [](BlendFuncArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::BlendFunc(exec_arg.sfactor, exec_arg.dfactor);
            }));
        }

        struct DepthFuncArg
        {
            GLenum func;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const DepthFuncArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<DepthFuncArg>(arg, [](DepthFuncArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::DepthFunc(exec_arg.func);
            }));
        }

        struct ColorMaskArg
        {
            GLboolean red;
            GLboolean green;
            GLboolean blue;
            GLboolean alpha;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const ColorMaskArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<ColorMaskArg>(arg, [](ColorMaskArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::ColorMask(exec_arg.red, exec_arg.green, exec_arg.blue, exec_arg.alpha);
            }));
        }

        struct PolygonOffsetArg
        {
            GLfloat factor;
            GLfloat units;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const PolygonOffsetArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<PolygonOffsetArg>(arg, [](PolygonOffsetArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::PolygonOffset(exec_arg.factor, exec_arg.units);
            }));
        }

        struct CullFaceArg
        {
            GLenum mode;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const CullFaceArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<CullFaceArg>(arg, [](CullFaceArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::CullFace(exec_arg.mode);
            }));
        }

        struct PixelStoreiArg
        {
            GLenum pname;
            GLint param;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const PixelStoreiArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<PixelStoreiArg>(arg, [](PixelStoreiArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::PixelStorei(exec_arg.pname, exec_arg.param);
            }));
        }

        struct LineWidthArg
        {
            GLfloat width;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const LineWidthArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<LineWidthArg>(arg, [](LineWidthArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::LineWidth(exec_arg.width);
            }));
        }

        struct BindRenderbufferArg
        {
            GLenum target;
            GLuint renderbuffer;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const BindRenderbufferArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<BindRenderbufferArg>(arg, [](BindRenderbufferArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::BindRenderbuffer(exec_arg.target, exec_arg.renderbuffer);
            }));
        }

        struct RenderbufferStorageArg
        {
            GLenum target;
            GLenum internalformat;
            GLsizei width;
            GLsizei height;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const RenderbufferStorageArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<RenderbufferStorageArg>(arg, [](RenderbufferStorageArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::RenderbufferStorage(exec_arg.target, exec_arg.internalformat, exec_arg.width, exec_arg.height);
            }));
        }

        struct FramebufferRenderbufferArg
        {
            GLenum target;
            GLenum attachment;
            GLenum renderbuffertarget;
            GLuint renderbuffer;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const FramebufferRenderbufferArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<FramebufferRenderbufferArg>(arg, [](FramebufferRenderbufferArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::FramebufferRenderbuffer(exec_arg.target, exec_arg.attachment, exec_arg.renderbuffertarget, exec_arg.renderbuffer);
            }));
        }

        struct PushMatrixArg
        {
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const PushMatrixArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<PushMatrixArg>(arg, [](PushMatrixArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::PushMatrix();
            }));
        }

        struct MatrixModeArg
        {
            GLenum mode;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const MatrixModeArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<MatrixModeArg>(arg, [](MatrixModeArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::MatrixMode(exec_arg.mode);
            }));
        }

        struct TexCoordPointerArg
        {
            GLint size;
            GLenum type;
            GLsizei stride;
            const GLvoid * pointer;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const TexCoordPointerArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<TexCoordPointerArg>(arg, [](TexCoordPointerArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::TexCoordPointer(exec_arg.size, exec_arg.type, exec_arg.stride, exec_arg.pointer);
            }));
        }

        struct PopMatrixArg
        {
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const PopMatrixArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<PopMatrixArg>(arg, [](PopMatrixArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::PopMatrix();
            }));
        }

        struct TexParameterfArg
        {
            GLenum target;
            GLenum pname;
            GLfloat param;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const TexParameterfArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<TexParameterfArg>(arg, [](TexParameterfArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::TexParameterf(exec_arg.target, exec_arg.pname, exec_arg.param);
            }));
        }

        struct TexParameterfvArg
        {
            GLenum target;
            GLenum pname;
            const GLfloat* params;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const TexParameterfvArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<TexParameterfvArg>(arg, [](TexParameterfvArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::TexParameterfv(exec_arg.target, exec_arg.pname, exec_arg.params);
            }));
        }

        struct LoadIdentityArg
        {
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const LoadIdentityArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<LoadIdentityArg>(arg, [](LoadIdentityArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::LoadIdentity();
            }));
        }

        struct Color4fArg
        {
            GLfloat  	red;
            GLfloat  	green;
            GLfloat  	blue;
            GLfloat  	alpha;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const Color4fArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<Color4fArg>(arg, [](Color4fArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::Color4f(exec_arg.red, exec_arg.green, exec_arg.blue, exec_arg.alpha);
            }));
        }

        struct RasterPos2iArg
        {
            GLint x;
            GLint y;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const RasterPos2iArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<RasterPos2iArg>(arg, [](RasterPos2iArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::RasterPos2i(exec_arg.x, exec_arg.y);
            }));
        }

        struct PolygonModeArg
        {
            GLenum face;
            GLenum mode;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const PolygonModeArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<PolygonModeArg>(arg, [](PolygonModeArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::PolygonMode(exec_arg.face, exec_arg.mode);
            }));
        }

        struct FramebufferTextureArg
        {
            GLenum target;
            GLenum attachment;
            GLuint texture;
            GLint level;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const FramebufferTextureArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<FramebufferTextureArg>(arg, [](FramebufferTextureArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::FramebufferTexture(exec_arg.target, exec_arg.attachment, exec_arg.texture, exec_arg.level);
            }));
        }

        struct AlphaFuncArg
        {
            GLenum  	func;
            GLclampf  	ref;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const AlphaFuncArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<AlphaFuncArg>(arg, [](AlphaFuncArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::AlphaFunc(exec_arg.func, exec_arg.ref);
            }));
        }

        struct Uniform1fArg
        {
            GLint location;
            GLfloat v0;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const Uniform1fArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<Uniform1fArg>(arg, [](Uniform1fArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::Uniform1f(exec_arg.location, exec_arg.v0);
            }));
        }

        struct Uniform2fArg
        {
            GLint location;
            GLfloat v0;
            GLfloat v1;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const Uniform2fArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<Uniform2fArg>(arg, [](Uniform2fArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::Uniform2f(exec_arg.location, exec_arg.v0, exec_arg.v1);
            }));
        }

        struct Uniform3fArg
        {
            GLint location;
            GLfloat v0;
            GLfloat v1;
            GLfloat v2;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const Uniform3fArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<Uniform3fArg>(arg, [](Uniform3fArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::Uniform3f(exec_arg.location, exec_arg.v0, exec_arg.v1, exec_arg.v2);
            }));
        }

        struct Uniform4fArg
        {
            GLint location;
            GLfloat v0;
            GLfloat v1;
            GLfloat v2;
            GLfloat v3;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const Uniform4fArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<Uniform4fArg>(arg, [](Uniform4fArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::Uniform4f(exec_arg.location, exec_arg.v0, exec_arg.v1, exec_arg.v2, exec_arg.v3);
            }));
        }

        struct Uniform1iArg
        {
            GLint location;
            GLint v0;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const Uniform1iArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<Uniform1iArg>(arg, [](Uniform1iArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::Uniform1i(exec_arg.location, exec_arg.v0);
            }));
        }

        struct Uniform2iArg
        {
            GLint location;
            GLint v0;
            GLint v1;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const Uniform2iArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<Uniform2iArg>(arg, [](Uniform2iArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::Uniform2i(exec_arg.location, exec_arg.v0, exec_arg.v1);
            }));
        }

        struct Uniform3iArg
        {
            GLint location;
            GLint v0;
            GLint v1;
            GLint v2;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const Uniform3iArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<Uniform3iArg>(arg, [](Uniform3iArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::Uniform3i(exec_arg.location, exec_arg.v0, exec_arg.v1, exec_arg.v2);
            }));
        }

        struct Uniform4iArg
        {
            GLint location;
            GLint v0;
            GLint v1;
            GLint v2;
            GLint v3;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const Uniform4iArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<Uniform4iArg>(arg, [](Uniform4iArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::Uniform4i(exec_arg.location, exec_arg.v0, exec_arg.v1, exec_arg.v2, exec_arg.v3);
            }));
        }

        struct Uniform1uiArg
        {
            GLint location;
            GLuint v0;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const Uniform1uiArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<Uniform1uiArg>(arg, [](Uniform1uiArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::Uniform1ui(exec_arg.location, exec_arg.v0);
            }));
        }

        struct Uniform2uiArg
        {
            GLint location;
            GLuint v0;
            GLuint v1;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const Uniform2uiArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<Uniform2uiArg>(arg, [](Uniform2uiArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::Uniform2ui(exec_arg.location, exec_arg.v0, exec_arg.v1);
            }));
        }

        struct Uniform3uiArg
        {
            GLint location;
            GLuint v0;
            GLuint v1;
            GLuint v2;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const Uniform3uiArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<Uniform3uiArg>(arg, [](Uniform3uiArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::Uniform3ui(exec_arg.location, exec_arg.v0, exec_arg.v1, exec_arg.v2);
            }));
        }

        struct Uniform4uiArg
        {
            GLint location;
            GLuint v0;
            GLuint v1;
            GLuint v2;
            GLuint v3;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const Uniform4uiArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<Uniform4uiArg>(arg, [](Uniform4uiArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::Uniform4ui(exec_arg.location, exec_arg.v0, exec_arg.v1, exec_arg.v2, exec_arg.v3);
            }));
        }

        struct Uniform1fvArg
        {
            GLint location;
            GLsizei count;
            const GLfloat *value;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const Uniform1fvArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<Uniform1fvArg>(arg, [](Uniform1fvArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::Uniform1fv(exec_arg.location, exec_arg.count, exec_arg.value);
            }));
        }

        struct Uniform2fvArg
        {
            GLint location;
            GLsizei count;
            const GLfloat *value;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const Uniform2fvArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<Uniform2fvArg>(arg, [](Uniform2fvArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::Uniform2fv(exec_arg.location, exec_arg.count, exec_arg.value);
            }));
        }

        struct Uniform3fvArg
        {
            GLint location;
            GLsizei count;
            const GLfloat *value;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const Uniform3fvArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<Uniform3fvArg>(arg, [](Uniform3fvArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::Uniform3fv(exec_arg.location, exec_arg.count, exec_arg.value);
            }));
        }

        struct Uniform4fvArg
        {
            GLint location;
            GLsizei count;
            const GLfloat *value;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const Uniform4fvArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<Uniform4fvArg>(arg, [](Uniform4fvArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::Uniform4fv(exec_arg.location, exec_arg.count, exec_arg.value);
            }));
        }

        struct Uniform1ivArg
        {
            GLint location;
            GLsizei count;
            const GLint *value;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const Uniform1ivArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<Uniform1ivArg>(arg, [](Uniform1ivArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::Uniform1iv(exec_arg.location, exec_arg.count, exec_arg.value);
            }));
        }

        struct Uniform2ivArg
        {
            GLint location;
            GLsizei count;
            const GLint *value;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const Uniform2ivArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<Uniform2ivArg>(arg, [](Uniform2ivArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::Uniform2iv(exec_arg.location, exec_arg.count, exec_arg.value);
            }));
        }

        struct Uniform3ivArg
        {
            GLint location;
            GLsizei count;
            const GLint *value;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const Uniform3ivArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<Uniform3ivArg>(arg, [](Uniform3ivArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::Uniform3iv(exec_arg.location, exec_arg.count, exec_arg.value);
            }));
        }

        struct Uniform4ivArg
        {
            GLint location;
            GLsizei count;
            const GLint *value;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const Uniform4ivArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<Uniform4ivArg>(arg, [](Uniform4ivArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::Uniform4iv(exec_arg.location, exec_arg.count, exec_arg.value);
            }));
        }

        struct Uniform1uivArg
        {
            GLint location;
            GLsizei count;
            const GLuint *value;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const Uniform1uivArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<Uniform1uivArg>(arg, [](Uniform1uivArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::Uniform1uiv(exec_arg.location, exec_arg.count, exec_arg.value);
            }));
        }

        struct Uniform2uivArg
        {
            GLint location;
            GLsizei count;
            const GLuint *value;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const Uniform2uivArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<Uniform2uivArg>(arg, [](Uniform2uivArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::Uniform2uiv(exec_arg.location, exec_arg.count, exec_arg.value);
            }));
        }

        struct Uniform3uivArg
        {
            GLint location;
            GLsizei count;
            const GLuint *value;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const Uniform3uivArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<Uniform3uivArg>(arg, [](Uniform3uivArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::Uniform3uiv(exec_arg.location, exec_arg.count, exec_arg.value);
            }));
        }

        struct Uniform4uivArg
        {
            GLint location;
            GLsizei count;
            const GLuint *value;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const Uniform4uivArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<Uniform4uivArg>(arg, [](Uniform4uivArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::Uniform4uiv(exec_arg.location, exec_arg.count, exec_arg.value);
            }));
        }

        struct UniformMatrix2fvArg
        {
            GLint location;
            GLsizei count;
            GLboolean transpose;
            const GLfloat *value;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const UniformMatrix2fvArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<UniformMatrix2fvArg>(arg, [](UniformMatrix2fvArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::UniformMatrix2fv(exec_arg.location, exec_arg.count, exec_arg.transpose, exec_arg.value);
            }));
        }

        struct UniformMatrix3fArg
        {
            GLint location;
            GLboolean transpose;
            GLfloat value[9];
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const UniformMatrix3fArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<UniformMatrix3fArg>(arg, [](UniformMatrix3fArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::UniformMatrix3fv(exec_arg.location, 1, exec_arg.transpose, exec_arg.value);
            }));
        }


        struct UniformMatrix3fvArg
        {
            GLint location;
            GLsizei count;
            GLboolean transpose;
            const GLfloat *value;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const UniformMatrix3fvArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<UniformMatrix3fvArg>(arg, [](UniformMatrix3fvArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::UniformMatrix3fv(exec_arg.location, exec_arg.count, exec_arg.transpose, exec_arg.value);
            }));
        }

        struct UniformMatrix4fvArg
        {
            GLint location;
            GLsizei count;
            GLboolean transpose;
            const GLfloat *value;
        };

        struct UniformMatrix4fvImplArg
        {
            GLint location;
            GLsizei count;
            GLboolean transpose;
            GLfloat value[16];
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const UniformMatrix4fvArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<UniformMatrix4fvArg>(arg, [](UniformMatrix4fvArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::UniformMatrix4fv(exec_arg.location, exec_arg.count, exec_arg.transpose, exec_arg.value);
            }));
        }

        struct UniformMatrix4fArg
        {
            GLint location;
            GLboolean transpose;
            GLfloat value[16];
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const UniformMatrix4fArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<UniformMatrix4fArg>(arg, [](UniformMatrix4fArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::UniformMatrix4fv(exec_arg.location, 1, exec_arg.transpose, exec_arg.value);
            }));
        }


        struct UniformBlockBindingArg
        {
            GLuint program;
            GLuint uniformBlockIndex;
            GLuint uniformBlockBinding;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const UniformBlockBindingArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<UniformBlockBindingArg>(arg, [](UniformBlockBindingArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::UniformBlockBinding(exec_arg.program, exec_arg.uniformBlockIndex, exec_arg.uniformBlockBinding);
            }));
        }

        struct VertexAttrib1fArg
        {
            GLuint index;
            GLfloat v0;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const VertexAttrib1fArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<VertexAttrib1fArg>(arg, [](VertexAttrib1fArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::VertexAttrib1f(exec_arg.index, exec_arg.v0);
            }));
        }

        struct VertexAttrib1sArg
        {
            GLuint index;
            GLshort v0;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const VertexAttrib1sArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<VertexAttrib1sArg>(arg, [](VertexAttrib1sArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::VertexAttrib1s(exec_arg.index, exec_arg.v0);
            }));
        }

        struct VertexAttrib1dArg
        {
            GLuint index;
            GLdouble v0;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const VertexAttrib1dArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<VertexAttrib1dArg>(arg, [](VertexAttrib1dArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::VertexAttrib1d(exec_arg.index, exec_arg.v0);
            }));
        }

        struct VertexAttribI1iArg
        {
            GLuint index;
            GLint v0;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const VertexAttribI1iArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<VertexAttribI1iArg>(arg, [](VertexAttribI1iArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::VertexAttribI1i(exec_arg.index, exec_arg.v0);
            }));
        }

        struct VertexAttribI1uiArg
        {
            GLuint index;
            GLuint v0;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const VertexAttribI1uiArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<VertexAttribI1uiArg>(arg, [](VertexAttribI1uiArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::VertexAttribI1ui(exec_arg.index, exec_arg.v0);
            }));
        }

        struct VertexAttrib2fArg
        {
            GLuint index;
            GLfloat v0;
            GLfloat v1;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const VertexAttrib2fArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<VertexAttrib2fArg>(arg, [](VertexAttrib2fArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::VertexAttrib2f(exec_arg.index, exec_arg.v0, exec_arg.v1);
            }));
        }

        struct VertexAttrib2sArg
        {
            GLuint index;
            GLshort v0;
            GLshort v1;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const VertexAttrib2sArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<VertexAttrib2sArg>(arg, [](VertexAttrib2sArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::VertexAttrib2s(exec_arg.index, exec_arg.v0, exec_arg.v1);
            }));
        }

        struct VertexAttrib2dArg
        {
            GLuint index;
            GLdouble v0;
            GLdouble v1;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const VertexAttrib2dArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<VertexAttrib2dArg>(arg, [](VertexAttrib2dArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::VertexAttrib2d(exec_arg.index, exec_arg.v0, exec_arg.v1);
            }));
        }

        struct VertexAttribI2iArg
        {
            GLuint index;
            GLint v0;
            GLint v1;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const VertexAttribI2iArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<VertexAttribI2iArg>(arg, [](VertexAttribI2iArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::VertexAttribI2i(exec_arg.index, exec_arg.v0, exec_arg.v1);
            }));
        }

        struct VertexAttribI2uiArg
        {
            GLuint index;
            GLuint v0;
            GLuint v1;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const VertexAttribI2uiArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<VertexAttribI2uiArg>(arg, [](VertexAttribI2uiArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::VertexAttribI2ui(exec_arg.index, exec_arg.v0, exec_arg.v1);
            }));
        }

        struct VertexAttrib3fArg
        {
            GLuint index;
            GLfloat v0;
            GLfloat v1;
            GLfloat v2;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const VertexAttrib3fArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<VertexAttrib3fArg>(arg, [](VertexAttrib3fArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::VertexAttrib3f(exec_arg.index, exec_arg.v0, exec_arg.v1, exec_arg.v2);
            }));
        }

        struct VertexAttrib3sArg
        {
            GLuint index;
            GLshort v0;
            GLshort v1;
            GLshort v2;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const VertexAttrib3sArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<VertexAttrib3sArg>(arg, [](VertexAttrib3sArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::VertexAttrib3s(exec_arg.index, exec_arg.v0, exec_arg.v1, exec_arg.v2);
            }));
        }

        struct VertexAttrib3dArg
        {
            GLuint index;
            GLdouble v0;
            GLdouble v1;
            GLdouble v2;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const VertexAttrib3dArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<VertexAttrib3dArg>(arg, [](VertexAttrib3dArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::VertexAttrib3d(exec_arg.index, exec_arg.v0, exec_arg.v1, exec_arg.v2);
            }));
        }

        struct VertexAttribI3iArg
        {
            GLuint index;
            GLint v0;
            GLint v1;
            GLint v2;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const VertexAttribI3iArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<VertexAttribI3iArg>(arg, [](VertexAttribI3iArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::VertexAttribI3i(exec_arg.index, exec_arg.v0, exec_arg.v1, exec_arg.v2);
            }));
        }

        struct VertexAttribI3uiArg
        {
            GLuint index;
            GLuint v0;
            GLuint v1;
            GLuint v2;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const VertexAttribI3uiArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<VertexAttribI3uiArg>(arg, [](VertexAttribI3uiArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::VertexAttribI3ui(exec_arg.index, exec_arg.v0, exec_arg.v1, exec_arg.v2);
            }));
        }

        struct VertexAttrib4fArg
        {
            GLuint index;
            GLfloat v0;
            GLfloat v1;
            GLfloat v2;
            GLfloat v3;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const VertexAttrib4fArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<VertexAttrib4fArg>(arg, [](VertexAttrib4fArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::VertexAttrib4f(exec_arg.index, exec_arg.v0, exec_arg.v1, exec_arg.v2, exec_arg.v3);
            }));
        }

        struct VertexAttrib4sArg
        {
            GLuint index;
            GLshort v0;
            GLshort v1;
            GLshort v2;
            GLshort v3;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const VertexAttrib4sArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<VertexAttrib4sArg>(arg, [](VertexAttrib4sArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::VertexAttrib4s(exec_arg.index, exec_arg.v0, exec_arg.v1, exec_arg.v2, exec_arg.v3);
            }));
        }

        struct VertexAttrib4dArg
        {
            GLuint index;
            GLdouble v0;
            GLdouble v1;
            GLdouble v2;
            GLdouble v3;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const VertexAttrib4dArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<VertexAttrib4dArg>(arg, [](VertexAttrib4dArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::VertexAttrib4d(exec_arg.index, exec_arg.v0, exec_arg.v1, exec_arg.v2, exec_arg.v3);
            }));
        }

        struct VertexAttrib4NubArg
        {
            GLuint index;
            GLubyte v0;
            GLubyte v1;
            GLubyte v2;
            GLubyte v3;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const VertexAttrib4NubArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<VertexAttrib4NubArg>(arg, [](VertexAttrib4NubArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::VertexAttrib4Nub(exec_arg.index, exec_arg.v0, exec_arg.v1, exec_arg.v2, exec_arg.v3);
            }));
        }

        struct VertexAttribI4iArg
        {
            GLuint index;
            GLint v0;
            GLint v1;
            GLint v2;
            GLint v3;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const VertexAttribI4iArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<VertexAttribI4iArg>(arg, [](VertexAttribI4iArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::VertexAttribI4i(exec_arg.index, exec_arg.v0, exec_arg.v1, exec_arg.v2, exec_arg.v3);
            }));
        }

        struct VertexAttribI4uiArg
        {
            GLuint index;
            GLuint v0;
            GLuint v1;
            GLuint v2;
            GLuint v3;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const VertexAttribI4uiArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<VertexAttribI4uiArg>(arg, [](VertexAttribI4uiArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::VertexAttribI4ui(exec_arg.index, exec_arg.v0, exec_arg.v1, exec_arg.v2, exec_arg.v3);
            }));
        }

        struct BindAttribLocationArg
        {
            GLuint program;
            GLuint index;
            const GLchar *name;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const BindAttribLocationArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<BindAttribLocationArg>(arg, [](BindAttribLocationArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::BindAttribLocation(exec_arg.program, exec_arg.index, exec_arg.name);
            }));
        }

        struct BindFragDataLocationArg
        {
            GLuint program;
            GLuint colorNumber;
            const char * name;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const BindFragDataLocationArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<BindFragDataLocationArg>(arg, [](BindFragDataLocationArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::BindFragDataLocation(exec_arg.program, exec_arg.colorNumber, exec_arg.name);
            }));
        }

        struct BeginQueryArg
        {
            GLenum target;
            GLuint id;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const BeginQueryArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<BeginQueryArg>(arg, [](BeginQueryArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::BeginQuery(exec_arg.target, exec_arg.id);
            }));
        }

        struct EndQueryArg
        {
            GLenum target;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const EndQueryArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<EndQueryArg>(arg, [](EndQueryArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::EndQuery(exec_arg.target);
            }));
        }

        struct Ortho2DArg
        {
            GLdouble left;
            GLdouble right;
            GLdouble bottom;
            GLdouble top;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const Ortho2DArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<Ortho2DArg>(arg, [](Ortho2DArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::Ortho2D(exec_arg.left, exec_arg.right, exec_arg.bottom, exec_arg.top);
            }));
        }

        struct BitmapCharacterArg
        {
            void *font;
            int character;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const BitmapCharacterArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<BitmapCharacterArg>(arg, [](BitmapCharacterArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::BitmapCharacter(exec_arg.font, exec_arg.character);
            }));
        }

        struct WaitSyncArg
        {
            GLsync sync;
            GLbitfield flags;
            GLuint64 timeout;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const WaitSyncArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<WaitSyncArg>(arg, [](WaitSyncArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::WaitSync(exec_arg.sync, exec_arg.flags, exec_arg.timeout);
            }));
        }

        struct ClientWaitSyncArg
        {
            GLsync sync;
            GLbitfield flags;
            GLuint64 timeout;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const ClientWaitSyncArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<ClientWaitSyncArg>(arg, [](ClientWaitSyncArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::ClientWaitSync(exec_arg.sync, exec_arg.flags, exec_arg.timeout);
            }));
        }

        struct DepthRangeArg
        {
            GLdouble nearVal;
            GLdouble farVal;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const DepthRangeArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<DepthRangeArg>(arg, [](DepthRangeArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::DepthRange(exec_arg.nearVal, exec_arg.farVal);
            }));
        }

        struct DepthRangefArg
        {
            GLfloat nearVal;
            GLfloat farVal;
        };

        template<>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const DepthRangefArg& arg)
        {
            queue->Enqueue(WSP_NEW GpuCommand<DepthRangefArg>(arg, [](DepthRangefArg& exec_arg)
            {
                wsp::opengl::glproxy::detail::DepthRangef(exec_arg.nearVal, exec_arg.farVal);
            }));
        }
    }
}

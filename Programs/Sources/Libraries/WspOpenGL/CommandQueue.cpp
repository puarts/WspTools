
#include <wsp/opengl/CommandQueue.h>
#include <wsp/common/fn-debug.h>
#include <gl/glew.h>
#include <wsp/opengl/GlProxy.h>

wsp::opengl::GpuCommandQueue::GpuCommandQueue()
    : command_queue_(WSP_NEW std::vector<IGpuCommand*>())
{
}

wsp::opengl::GpuCommandQueue::~GpuCommandQueue()
{
    Clear();
    delete command_queue_;
}

void wsp::opengl::GpuCommandQueue::Execute()
{
    for (int i = 0, count = static_cast<int>(command_queue_->size()); i < count; ++i)
    {
        IGpuCommand* command = (*command_queue_)[i];
        command->Execute();
    }
}

void wsp::opengl::GpuCommandQueue::Enqueue(IGpuCommand* command)
{
    (*command_queue_).push_back(command);
}

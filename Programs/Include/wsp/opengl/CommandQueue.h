#pragma once

#include <wsp/common/_define_commoncore.h>
#include <wsp/common/fn-debug.h>
#include <vector>
#include <functional>

namespace wsp {
    namespace opengl {

        class IGpuCommand
        {
        public:
            virtual ~IGpuCommand()
            {
            }

            virtual void Execute() = 0;
        };

        template<typename TCommandArg>
        class GpuCommand : public IGpuCommand
        {
        public:
            explicit GpuCommand(
                const TCommandArg& arg,
                const std::function<void(TCommandArg&)>& exec_func)
                : arg_(arg)
                , exec_func_(exec_func)
            {
            }

            virtual void Execute()
            {
                exec_func_(arg_);
            }

        private:
            TCommandArg arg_;
            std::function<void(TCommandArg&)> exec_func_;
        };

        class WSP_DLL_EXPORT GpuCommandQueue
        {
        public:
            GpuCommandQueue();
            ~GpuCommandQueue();

            void Execute();

            void Enqueue(IGpuCommand* command);

            template<typename TGpuCommandArg>
            void EnqueueByArg(const TGpuCommandArg& arg);

            int GetCommandCount() const
            {
                return (int)command_queue_->size();
            }

            const IGpuCommand* GetCommand(int index) const
            {
                return (*command_queue_)[index];
            }

            IGpuCommand* GetCommand(int index)
            {
                return (*command_queue_)[index];
            }

            inline void Clear()
            {
                (*command_queue_).clear();
            }

        private:
            std::vector<IGpuCommand*>* command_queue_;
        };

        template<typename TGpuCommandArg>
        inline void EnqueueGpuCommand(GpuCommandQueue* queue, const TGpuCommandArg& arg)
        {
            WSP_FATAL("Specified TGpuCommandArg command is not implemented");
        }

        template<typename TGpuCommandArg>
        void GpuCommandQueue::EnqueueByArg(const TGpuCommandArg& arg)
        {
            EnqueueGpuCommand<TGpuCommandArg>(this, arg);
        }
    }
}

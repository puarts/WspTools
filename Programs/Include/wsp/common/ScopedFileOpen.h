#pragma once

#include <stdio.h>

namespace wsp {
    namespace common {
        class ScopedOpenFile
        {
        public:
            ScopedOpenFile(const char* file_name, const char* mode)
            {
                fp_ = fopen(file_name, mode);
            }

            ~ScopedOpenFile()
            {
                if (fp_)
                {
                    fclose(fp_);
                }
            }

            FILE* GetOpenedFilePointer()
            {
                return fp_;
            }

        private:
            FILE* fp_;
        };
    }
}

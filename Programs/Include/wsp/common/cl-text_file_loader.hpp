/**
 * @file cl-text_file_loader.hpp
 * 
 */

#ifndef __WSP_CORE_CLASS_TEXT_FILE_LOADER_HPP__
#define __WSP_CORE_CLASS_TEXT_FILE_LOADER_HPP__

#include <string>

#include "result.h"

namespace wsp{
    class WSP_DLL_EXPORT TextFileLoader
    {
    public:
        TextFileLoader() {}

        explicit TextFileLoader( const char *file_path );

        wsp::Result Load( const char *file_path );

        std::string data() const
        { 
            return data_; 
        }

    private:
        std::string data_;
    };
}

#endif

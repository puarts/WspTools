/**
 * @file cl-text_file_loader.cpp
 * 
 */

#include "cl-text_file_loader.hpp"

#include "fn-debug.h"
#include "common_result.h"

#include <fstream>
#include <iostream>

wsp::TextFileLoader::TextFileLoader( const char *file_path )
{
    WSP_FATAL_IF_RESULT_FAILED( Load(file_path), "Loading %s failed", file_path );
}

wsp::Result wsp::TextFileLoader::Load( const char *file_path )
{
    if( ::WSP_Exists(file_path)==false )
    {
        WSP_THROW_RESULT( wsp::ResultNotFound() );
    }

    try
    {
        std::ifstream ifs( file_path );
        std::string str;
        ifs >> data_;
        while(getline(ifs, str)) 
        {
            data_ += str;
        }

        ifs.close();
    }
    catch( std::ifstream::failure e )
    {
        WSP_THROW_RESULT( wsp::ResultUnknownError() );
    }

    WSP_COMMON_DEBUG_LOG("Successfully loaded %s..\n", file_path);

    return wsp::ResultSuccess();
}

/**
 * @file fn-type_conversion.c
 * @author 
 */

#include <string.h>

#include "fn-type_conversion.h"

WSP_ImageFormat WSP_GetImageFormat( const char *filename )
{
    const char *ptr;
    const char *end_ptr;
    const WSP_ImageFormatInfo *info_ptr;
    const WSP_ImageFormatInfo *end_info_ptr;
    size_t len = strlen( filename );
    const char *ext_ptr;

    {
        ptr     = filename + len - 1;
        end_ptr = filename - 1;
        for( ; ptr!=end_ptr; --ptr)
        {
            if( *ptr=='.' )
            {
                ext_ptr = ptr+1;
                break;
            }
        }
        if( ptr==end_ptr ){ return WSP_IMAGE_FORMAT_UNKNOWN; }
    }

    {
        info_ptr     = WSP_IMAGE_FORMAT_INFO_LIST;
        end_info_ptr = info_ptr + WSP_IMAGE_FORMAT_INFO_LIST_SIZE;
        for( ; info_ptr!=end_info_ptr; ++info_ptr)
        {
            if( strcmp(ext_ptr, (*info_ptr).extension) == 0 )
            {
                return (*info_ptr).format_id;
            }
        }
    }
    return WSP_IMAGE_FORMAT_UNKNOWN;
}



WSP_ImageFormat WSP_GetImageFormatID(const char *extension)
{
    if(strcmp(extension, "adri")==0){ return WSP_IMAGE_FORMAT_ADRI; }
    if(strcmp(extension, "bin") || strcmp(extension, "raw")){ return WSP_IMAGE_FORMAT_RAW; }
    if(strcmp(extension, "ppm")==0){  return WSP_IMAGE_FORMAT_PPM; }
    if(strcmp(extension, "pgm")==0){  return WSP_IMAGE_FORMAT_PGM; }
    return WSP_IMAGE_FORMAT_UNKNOWN;
}
/**
 * @file _tpl_fn-img_mem_manage.hpp
 * @author 
 */

#ifndef WSP_IMAGE_BASE_TEMPLATE_FUNC_IMG_MEM_MANAGE_H_
#define WSP_IMAGE_BASE_TEMPLATE_FUNC_IMG_MEM_MANAGE_H_

#ifndef __WSP_IMAGEUTIL_DEFINE_IMAGEUTIL_H__
#include "_define_image.h"
#endif


namespace wsp{ namespace img{

    inline void AllocImg (wsp::img::ImgPtr8U &img, size_t allocLength);
    inline void AllocImg(wsp::img::ImgPtr16U &img, size_t allocLength);
    inline void AllocImg(wsp::img::ImgPtr32U &img, size_t allocLength);

    inline void ReleaseImg(wsp::img::ImgPtr8U  &img);
    inline void ReleaseImg(wsp::img::ImgPtr16U &img);
    inline void ReleaseImg(wsp::img::ImgPtr32U &img);

    template<typename T> 
    T RgbToGray(T r, T g, T b);
    
    /**
     * @group WspImage
     * Memory management function for image data.
     * @param set_new_channels_to_zero if the number of channels increase and set_new_channels_to_zero=true,
     *         than increased channels are filled with zero, 
     *         else increased channels are filled with the value which the last channel has.
     */
    template<typename T> 
    inline wsp::State ReallocImageData(T **data, uchar **alpha_data, uchar **state_data, 
        int alloc_size, int length_new, int num_of_channels_new=1, 
        int length=0, int num_of_channels=0, bool set_new_channels_to_zero=false);

    template<typename T>
    inline wsp::State AllocImageData(T **data, uchar **alpha_data, uchar **state_data, int mem_size, int length, int num_of_channels=1);

    template<typename T> 
    inline wsp::State FreeImageData(T **data, uchar **alpha_data, uchar **state_data);
}}

#include "_tpl_fn-img_mem_manag.inl"

#endif

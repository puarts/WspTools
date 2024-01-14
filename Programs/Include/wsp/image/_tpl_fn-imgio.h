/**
 * @file _tpl_fn-imgio.h
 * 
 * @brief functions which provide image I/O functionality.
 */

#ifndef WspImage_WspFnImgIO_tpl_H_
#define WspImage_WspFnImgIO_tpl_H_

#ifndef __WSP_IMAGEUTIL_DEFINE_IMAGEUTIL_H__
#include "_define_image.h"
#endif

namespace wsp{ namespace img{

    /**
     * @brief Save image as raw data.
     * @param[in] in_data             Image data for saving.
     * @param[in] in_alpha            Alpha channel data for saving.
     * @param[in] in_state            State channel data for saving.
     * @param[in] width               Width of input image.
     * @param[in] height              Height of input image.
     * @param[in] num_channels        Number of channels of input image.
     * @param[in] in_file_name_format File name format of image saved.
     */
    template<typename T> wsp::State SaveRawData(
                                        const T *in_data, 
                                        const uchar *in_alpha, 
                                        const uchar *in_state, 
                                        int width, int height, int num_channels, 
                                        const char *in_file_name_format, ...
                                    );
    /**
     * @brief Load raw data from storage.
     * @param[out] o_data         Destination image data for loading.
     * @param[out] o_alpha        Alpha channel data for loading.
     * @param[out] o_state        State channel data for loading.
     * @param[out] o_width        Width of output image.
     * @param[out] o_height       Height of output image.
     * @param[out] o_num_channels Number of channels of output image.
     * @param[in]  file_name      File name for loading.
     */
    template<typename T> wsp::State LoadRawData(
                                        T **o_data, 
                                        uchar **o_alpha, 
                                        uchar ** o_state, 
                                        int &o_width, int &o_height, int &o_num_channels, 
                                        const char *file_name
                                    );

    /**
     * @brief Compress input data into ADRI(Adaptive Dynamic Range Image).
     */
    template<typename T> wsp::State CompressInADR(int *o_bin, const T *in_src, int length, int dynamic_range_bit, T im_min);
    /**
     * @brief Decompress input data from ADRI(Adaptive Dynamic Range Image).
     */
    template<typename T> wsp::State DecompressADR(T *o_img, const int *in_src, int length, int dynamic_range_bit, int im_min);
    /**
     * @brief Load ADRI(Adaptive Dynamic Range Image) from image file saved on storage.
     */
    template<typename T> wsp::State LoadADRI(T **o_img, int &o_width, int &o_height, int &o_num_of_channels, const char *filename);
    /**
     * @brief Save as ADRI(Adaptive Dynamic Range Image) on storage.
     */
    template<typename T> wsp::State SaveAsADRI(const T *in_img, int width, int height, int num_of_channels, const char *filename);

}}

#include "_tpl_fn-imgio.inl"


#endif
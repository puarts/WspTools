/**
 * @file tpl_cl-image.h
 * 
 * @brief Base class for any type of image class
 */

#ifndef _WSP_IMAGE_TEMPLATE_CLASS_IMAGE_H_
#define _WSP_IMAGE_TEMPLATE_CLASS_IMAGE_H_

#ifndef __WSP_COMMONUTIL_TEMPLATE_CLASS_VECTOR_H__
#include <wsp/common/tpl_cl-vector.h>
#endif
#include "_tpl_fn-img_mem_manag.hpp"
#include "_tpl_fn-imgblend.hpp"

namespace wsp{
    //******************************************************************************
    //* CLASS DECLARATION (wsp::Image)
    /**
     * @description wsp::Image is a class which provides access to some of image manipulation functionality. 
     * @group WspImage
     * @brief Class for image data manipulation 
     */
    template<typename T=u8, int MemAllocStep=50>
    class Image
        : public wsp::Vector<T, MemAllocStep>
    {
    public:
        Image();

        explicit Image(int width, int hieght, int num_of_channels = 1);
        
        /** @brief  if dst_nChannels=0, nChannels will be the same as src_nChannels */
        template<typename U> 
        explicit Image(const wsp::Image<U> &in_img, int dst_num_of_channels=0);
        
        template<typename U> 
        explicit Image(const U *in_data, const uchar *in_alpha, const uchar *in_state, int width, int height, int nChannels);
        
        Image(const wsp::Image<T, MemAllocStep> &img);
        
        Image(int length); //! for int casting 
        
        ~Image();


        // Accessors: Getter -------------------------------
        int                      width() const;
        int                      height() const;
        u32                      GetWidthStep() const{ return sizeof(T)*wsp::Vector<T, MemAllocStep>::num_of_channels_*width_; }
        int*                     width_ptr()            { return &width_; }
        int*                     height_ptr()           { return &height_; }
        wsp::img::ImageParam     image_param() const    { return wsp::img::ImageParam(width_, height_, wsp::Vector<T, MemAllocStep>::num_of_channels_); }
        wsp::DataType            data_type() const      { return wsp::GetDataType<T>(); }
        uchar*                   byte_data()            { return reinterpret_cast<uchar*>(wsp::Vector<T, MemAllocStep>::data_); }
        uchar*                   alpha_data();
        uchar*                   state_data();
        const                    uchar* alpha_data() const;
        const                    uchar* state_data() const;
        int                      GetDataSize() const;
        int                      GetImageSize() const;
        T                        GetElem(int x, int y, int c=0) const;
        T                        GetElem(int index) const;
        uchar                    GetAlpha(int x, int y) const;
        uchar                    GetAlpha(int index) const;
        void                     CopyAlphaInto(wsp::Image<uchar> &o_alpha) const;
        uchar                    GetState(int x, int y) const;
        uchar                    GetState(int index) const;
        void                     CopyStateInto(wsp::Image<uchar> &o_state) const;
        wsp::img::DepthValueType GetValueType() const{ return value_type_; }
        T                        GetMin(uchar alpha_thre=0) const;
        T                        GetMax(uchar alpha_thre=0) const;
        void                     GetMaxLoc(int *o_x, int *o_y, T *o_value) const;
        void                     GetMinLoc(int *o_x, int *o_y, T *o_value) const;
        void                     GetMaxLoc(double *o_x, double *o_y, T *o_value) const;
        void                     GetMinLoc(double *o_x, double *o_y, T *o_value) const;
        void                     GetMaxIndex(int *o_index, T *o_value) const;
        bool                     IsOutOfBounds(int x, int y, int channel=0) const;
        template<typename U> 
        bool                     IsTheSameSize(const wsp::Image<U> &in_img) const;
        int                      GetNumOfIndex() const;
        wsp::State               GetIndexList(wsp::Vector<int> &o_indexList) const;

        // Accessors: Setter -------------------------------
        wsp::Result Shift( s32 x, s32 y );
        wsp::Result BlendImage( const wsp::Image<T> &in_image, WSP_ImageBlendMode blend_mode );

        /** @brief  if dataSize and nChannels=0, parameters become the same as source */
        template<typename U> 
        void       CopyIntoAnyImage(wsp::Image<U> &o_dst, int num_of_channels_dst=0) const;
        
        template<typename U> 
        void       CopyIntoAnyImageAsU8(wsp::Image<U> &o_dst) const;

        template<typename U> 
        wsp::State CopyFrom( const wsp::Image<U>& img, int dst_num_of_channels=0);
        template<typename U> 
        wsp::State CopyFrom( const U* data, const uchar* alpha, const uchar* state, 
                             int width, int height, int src_num_of_channels, int dst_num_of_channels=0 );
        void       CopyFrom( const void* data, const uchar* alpha, const uchar* state, 
                             wsp::DataType type, int width, int height, int num_of_channels );
        void       SetDataFrom(T *data, int width, int height, int num_of_channels);
        void       SetDataFrom(T *data, uchar *alpha, uchar *state, int width, int height, int num_of_channels);
        void       SetDataInto(T *o_data, int *o_width, int *o_height, int *o_num_of_channels);
        void       SetDataInto(T **o_data, uchar **o_alpha, uchar **o_state, int *o_width, int *o_height, int *o_num_of_channels);
        void       SetElem(const T& element, int x, int y, int channel=0);
        void       SetElem(const T& element, int index);
        void       SetAlpha(uchar element, int x, int y);
        void       SetAlpha(uchar element, int index);
        void       AddElem(const T& element, int x, int y, int channel=0);
        void       AddAlpha(uchar element, int x, int y);
        void       SetState(uchar element, int x, int y);
        void       SetState(uchar element, int index);
        void       SetValueType(wsp::img::DepthValueType type){ value_type_ = type; }
        
        /**
         * SetSize is faster than Resize and ResizeCanvas but this treat image as vector
         * nChannels=0 means keep old num of channels
         */
        wsp::State SetSize(int width, int height, int nChannels=0, bool set_new_channels_to_zero=false); 
        
        // TO DO: ‰æ‘œ‚ª1x1‚Ì‚Æ‚«‚ÉResize‚Å—Ž‚¿‚é
        wsp::Result ScaleImage( u32 width, u32 height );
        
        wsp::State ResizeCanvas(int width, int height, u8 expand_alpha_val=255 );
        wsp::State SetNumOfChannels(int num_of_channels){ return SetSize(width_, height_, num_of_channels); }
        void       ResetAlpha(const uchar alpha);
        void       ResetState(const wsp::img::PixelState pixStat);
        void       ResetTargetChannel( T value, s32 target_channel_index );
        void       SetTransparentStateByAlpha(uchar thre_alpha=255);
        void       SetAlphaAtTargetStatePix(uchar alpha, wsp::img::PixelState target_state);
        
        /** @brief  fill_pixel_data must has the same length data as num_of_channels_ */
        void       SetValueAtTargetStatePixel(const T *in_fill_pixel_data, wsp::img::PixelState target_state); 
        wsp::State SetSizeToPowerOfTwo(bool set_also_height=true);
        wsp::State Clear();
        
        /* @brief invert all pixel values */
        void       Invert();
        void       Normalize(T min_norm, T max_norm, uchar thre_alpha=0, T min_src=0, T max_src=0);
        void       NormalizeAlpha(uchar min_norm=0, uchar max_norm=255);
        void       Transpose();

        // IO ----------------------------------------------
        wsp::State Load(const char *filename, bool as_alpha=false);
        wsp::State Save(const char *filename) const;
        wsp::State LoadImageFile(const char *filename, int nChannels=0); //! if nChannels=0, nChannels depend on loaded file
        wsp::State LoadImageFileAsAlpha(const char *filename);
        wsp::State SaveImage(const char *filename, wsp::img::PixelState redState=WSP_PIXEL_STATE_NORMAL) const;
        wsp::State SaveAlpha(const char *filename) const;
        wsp::State SaveState(const char *filename) const;
        wsp::State SaveState(const char *filename, wsp::img::PixelState state) const;
        wsp::State LoadADRI(const char *filename, const char *filename_alpha=NULL, int nChannels=0U);
        wsp::State SaveAsADRI(const char *filename, const char *filename_alpha=NULL) const;

        wsp::State SaveRawData(const char *in_fileNameFormat,...) const;
        wsp::State LoadRawData(const char *fileName);

        // Operators ---------------------------------------
        wsp::Image<T, MemAllocStep>& operator=(const wsp::Image<T, MemAllocStep>& other);
        bool operator==(const wsp::Image<T, MemAllocStep>& other) const;
        bool operator!=(const wsp::Image<T, MemAllocStep>& other) const;

    private:
        int                      width_;
        int                      height_;
        uchar*                   alpha_data_;
        uchar*                   state_data_;
        wsp::img::DepthValueType value_type_;
    };

    //template class Image<u8>;
    //template class Image<float>;

    //template Image<u8>::Image(const wsp::Image<u8> &in_img, int dst_num_of_channels);
    //template Image<u8>::Image(const u8 *in_data, const uchar *in_alpha, const uchar *in_state, int width, int height, int nChannels);

    //template Image<float>::Image(const wsp::Image<float> &in_img, int dst_num_of_channels);
    //template Image<float>::Image(const float *in_data, const uchar *in_alpha, const uchar *in_state, int width, int height, int nChannels);
}

#include "tpl_cl-image.inl"

#endif

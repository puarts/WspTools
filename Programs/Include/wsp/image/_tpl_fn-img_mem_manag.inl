/**
 * @file _tpl_fn-img_mem_manage.inl
 * @author 
 */

#ifndef WSP_IMAGE_BASE_TEMPLATE_FUNC_IMG_MEM_MANAGE_INL_
#define WSP_IMAGE_BASE_TEMPLATE_FUNC_IMG_MEM_MANAGE_INL_


#include <wsp/common/_tpl_fn-mem_manag.hpp>

#define USE_OPTIMIZEED_ReallocImageData
#undef USE_OPTIMIZEED_ReallocImageData

inline void wsp::img::AllocImg (wsp::img::ImgPtr8U &img, size_t allocLength){ img = WSP_NEW uchar[allocLength]; }
inline void wsp::img::AllocImg(wsp::img::ImgPtr16U &img, size_t allocLength){ img = WSP_NEW ushort[allocLength]; }
inline void wsp::img::AllocImg(wsp::img::ImgPtr32U &img, size_t allocLength){ img = WSP_NEW u32[allocLength]; }

inline void wsp::img::ReleaseImg(wsp::img::ImgPtr8U  &img){ delete[] img; }
inline void wsp::img::ReleaseImg(wsp::img::ImgPtr16U &img){ delete[] img; }
inline void wsp::img::ReleaseImg(wsp::img::ImgPtr32U &img){ delete[] img; }

template<typename T> 
T wsp::img::RgbToGray(T r, T g, T b){ return (T)(0.229*(double)r+0.587*(double)g+0.114*(double)b); }

/**
    * @group WspImage
    * Memory management function for image data.
    * @param set_new_channels_to_zero if the number of channels increase and set_new_channels_to_zero=true,
    *         than increased channels are filled with zero, 
    *         else increased channels are filled with the value which the last channel has.
    */
template<typename T> 
inline wsp::State wsp::img::ReallocImageData(
    T **data,
    uchar **alpha_data,
    uchar **state_data, 
    int alloc_size,
    int length_new,
    int num_of_channels_new, 
    int length,
    int num_of_channels,
    bool set_new_channels_to_zero)
{
    if(alloc_size<length_new*num_of_channels_new)
    {
        WSP_COMMON_ERROR_LOG("alloc memory size must be bigger than length*num_of_channels\n");
        return WSP_STATE_INVALID_PARAMETER;
    }
    wsp::State state;
    if(data != NULL)
    {
        int size = length*num_of_channels;
        T *tmp_data = NULL;
        // keep old data in temporary data for converting channels
        if(num_of_channels!=0 && num_of_channels_new!=num_of_channels)
        {
            tmp_data = WSP_NEW T[size];
            memcpy(tmp_data, *data, size*sizeof(T));
            //for(i=0; i<size; i++){ tmp_data[i] = (*data)[i]; }
        }
        //state = wsp::ReallocArrayMem(data, size_new, size);

        // putting in the same alloc size on both of new and old mem size means 
        // applying no operation of copying in wsp::ReallocArrayMem, it is faster
        state = wsp::ReallocArrayMem(data, alloc_size, alloc_size); 
        if(state!=WSP_STATE_SUCCESS)
        {
            delete[] tmp_data;
            WSP_COMMON_ERROR_LOG("Realloc data failed\n");
            return state;
        }

        // convert channels
        int copy_len = length;
        if( copy_len > length_new )
        {
            copy_len = length_new;
        }

        if(num_of_channels != 0 && num_of_channels_new != num_of_channels)
        {
            if(num_of_channels_new < num_of_channels)
            {
#ifdef USE_OPTIMIZEED_ReallocImageData
                T *dst_ptr = *data;
                T *src_ptr = tmp_data;
                T *end_ptr = dst_ptr + size_new;
                int cnt = 0;
                int diff_nc = abs(num_of_channels - num_of_channels_new);
                // optimized code ----------
                for(; dst_ptr!=end_ptr; ++dst_ptr)
                {
                    *dst_ptr = *src_ptr;
                    ++cnt;
                    if(cnt==num_of_channels_new){
                        src_ptr+=diff_nc;
                        cnt=0;
                        continue;
                    }
                    ++src_ptr;
                }
#else
                // un-optimized code -------
                for(int i=0; i<copy_len; ++i)
                {
                    int i_nc_new = i*num_of_channels_new;
                    int i_nc_old = i*num_of_channels;
                    for(int c=0; c<num_of_channels_new; ++c)
                    {
                        (*data)[i_nc_new+c] = tmp_data[i_nc_old+c];
                    }
                }
#endif
            }
            else
            {
                if(set_new_channels_to_zero == false)
                {
#ifdef USE_OPTIMIZEED_ReallocImageData
                    // optimized code ---------------
                    T *end_c_ptr;
                    int dbg_c=0;
                    for(; dst_ptr!=end_ptr; ++src_ptr, ++dbg_c){
                        *dst_ptr = *src_ptr;
                        ++cnt;
                        if(cnt==num_of_channels){
                            end_c_ptr = dst_ptr + diff_nc;
                            for(; dst_ptr!=end_c_ptr; ++dst_ptr){
                                *dst_ptr = *src_ptr;
                            }
                            cnt=0;
                            continue;
                        }
                        ++dst_ptr;
                    }
#else
                    // unoptimized code -------------
                    for(int i=0; i<copy_len; ++i)
                    {
                        int i_nc_new = i*num_of_channels_new;
                        int i_nc_old = i*num_of_channels;
                        for(int c=0; c<num_of_channels_new; ++c){
                            if(c<num_of_channels){
                                (*data)[i_nc_new+c] = tmp_data[i_nc_old+c];
                                continue;
                            }
                            (*data)[i_nc_new+c] = tmp_data[i_nc_old+num_of_channels-1];
                        }
                    }
#endif
                }
                else
                {
#ifdef USE_OPTIMIZEED_ReallocImageData
                    // optimized code ----------------
                    memset(*data, 0, size_new*sizeof(T));
                    for(; dst_ptr!=end_ptr; ++dst_ptr, ++src_ptr)
                    {
                        *dst_ptr = *src_ptr;
                        ++cnt;
                        if(cnt==num_of_channels){ 
                            dst_ptr+=diff_nc; 
                            cnt=0; 
                        }
                    }
#else
                    // un-optimized code -------------
                    for(int i=0; i<copy_len; ++i)
                    {
                        int i_nc_new = i*num_of_channels_new;
                        int i_nc_old = i*num_of_channels;
                        for(int c=0; c<num_of_channels_new; ++c)
                        {
                            if(c < num_of_channels)
                            {
                                (*data)[i_nc_new+c] = tmp_data[i_nc_old+c];
                                continue;
                            }
                            (*data)[i_nc_new+c] = 0;
                        }
                    }
#endif
                }

            }
            delete[] tmp_data;
        }
    }
    int alloc_size_alpha_state = alloc_size/num_of_channels_new;
    if(alpha_data!=NULL)
    {
        state = wsp::ReallocArrayMem(alpha_data, alloc_size_alpha_state, length, 0xff);
        if(state!=WSP_STATE_SUCCESS)
        {
            WSP_COMMON_ERROR_LOG("Realloc alpha data failed\n");
            return state;
        }
    }
    if(state_data!=NULL)
    {
        state = wsp::ReallocArrayMem(state_data, alloc_size_alpha_state, length, WSP_PIXEL_STATE_NORMAL);
        if(state!=WSP_STATE_SUCCESS)
        {
            WSP_COMMON_ERROR_LOG("Realloc state data failed\n");
            return state;
        }
    }
    return WSP_STATE_SUCCESS;
}

template<typename T>
inline wsp::State wsp::img::AllocImageData(T **data, uchar **alpha_data, uchar **state_data, int mem_size, int length, int num_of_channels){
    return wsp::img::ReallocImageData(data, alpha_data, state_data, mem_size, length, num_of_channels, 0, 0);
}

template<typename T> 
inline wsp::State wsp::img::FreeImageData(T **data, uchar **alpha_data, uchar **state_data){
    wsp::State state;
    if(data!=NULL){
        state = wsp::FreeArrayMem(data); 
        if(state!=WSP_STATE_SUCCESS){ WSP_COMMON_ERROR_LOG("Freeing memory failed\n"); return state; }
    }
    if(alpha_data!=NULL){ 
        state = wsp::FreeArrayMem(alpha_data); 
        if(state!=WSP_STATE_SUCCESS){ WSP_COMMON_ERROR_LOG("Freeing memory failed\n"); return state; }
    }
    if(state_data!=NULL){ 
        state = wsp::FreeArrayMem(state_data); 
        if(state!=WSP_STATE_SUCCESS){ WSP_COMMON_ERROR_LOG("Freeing memory failed\n"); return state; }
    }
    return WSP_STATE_SUCCESS;
}

#endif

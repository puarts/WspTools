
#ifndef _WSP_MATH_BASE_TEMPLATE_FUNCTION_MATRIX_INL_
#define _WSP_MATH_BASE_TEMPLATE_FUNCTION_MATRIX_INL_

//** Matrix --------------------------------------------------------------

template<typename T> 
void wsp::math::Mult(T *m_l, T *m_r, T *m_res, int row, int col){
    //** optimized code
    {
        T *lp = m_l, *rp = m_r, *res = m_res;
        T *lp_h, *lp_v, *rp_v, *rp_h;
        T *end_lh, *end_lv, *end_rh;
        end_lv = lp + row*col;
        for(lp_v=lp; lp_v!=end_lv; lp_v+=col){
            end_rh = rp + col;
            for(rp_h=rp; rp_h!=end_rh;++rp_h, ++res){
                *res = (T)0.0;
                end_lh = lp_v + col;
                for(rp_v=rp_h, lp_h=lp_v; lp_h!=end_lh; ++lp_h, rp_v+=col){
                    *res += (*lp_h) * (*rp_v);
                }
            }
        }
    }
    //** simple code
    {
        //int i, j;
        //for(i=0; i<col; ++i){
        //    for(j=0; j<row; ++j){
        //        m_res[i*col+j] = m_l[i*col+j] * m_r[j*col+i];
        //    }
        //}
    }
}


template<typename T>
inline void wsp::math::Transpose(T *io_img, int row_size, int col_size)
{
    int len = row_size * col_size;
    T *tmp_data = WSP_NEW T[len];
    //** copy data
    {
        T *dst_ptr = tmp_data;
        T *src_ptr = io_img;
        T *end_ptr = dst_ptr + len;
        for(;dst_ptr!=end_ptr; ++dst_ptr, ++src_ptr){
            *dst_ptr = *src_ptr;
        }
    }
    //** transpose
    {
        T *src_ptr = tmp_data;
        T *end_src = tmp_data + len - 1;
        T *dst_ptr = io_img;
        T *end_dst = io_img + len;
        int back_pix = len - 1;
        //int i=0, j=0;
        for(; dst_ptr!=end_dst; ++dst_ptr, src_ptr+=row_size){
            if(src_ptr>end_src){
                src_ptr -= back_pix;
                //i -=back_pix;
            }
            *dst_ptr = *src_ptr;
            //printf("[%d] = [%d]\n", j, i);
            //i+=row_size, ++j;
        }
    }
    delete[] tmp_data;
}
template<typename T>
inline void wsp::math::SwapQuadrantHor(const T *in_src, T *o_dst, int row_size, int col_size)
{
    int length = row_size * col_size;
    int hw = row_size/2;
    int hh = col_size/2;
    //** swap
    {
        int x, y, i, j;
        for(y=0; y<col_size; ++y){
            for(x=0; x<row_size; ++x){
                j = y*row_size + x;
                if(y<hh && x<hw){
                    i = y*row_size + x + hw;
                }else if(y<hh && x>=hw){
                    i = y*row_size + x - hw;
                }else if(y>=hh && x<hw){
                    i = y*row_size + x + hw;
                }else{
                    i = y*row_size + x - hw;
                }
                o_dst[j] = in_src[i];
            }
        }

    }
}
template<typename T>
inline void wsp::math::SwapQuadrantVer(const T *in_src, T *o_dst, int row_size, int col_size)
{
    int length = row_size * col_size;
    int hw = row_size/2;
    int hh = col_size/2;
    //** swap
    {
        int x, y, i, j;
        for(y=0; y<col_size; ++y){
            for(x=0; x<row_size; ++x){
                j = y*row_size + x;
                if(y<hh && x<hw){
                    i = (y+hh)*row_size + x;
                }else if(y<hh && x>=hw){
                    i = (y+hh)*row_size + x;
                }else if(y>=hh && x<hw){
                    i = (y-hh)*row_size + x;
                }else{
                    i = (y-hh)*row_size + x;
                }
                o_dst[j] = in_src[i];
            }
        }
    }
}
template<typename T>
inline void wsp::math::SwapQuadrantHV(T *tmp_img, T *io_img, int row_size, int col_size)
{
    wsp::math::SwapQuadrantHor(io_img, tmp_img, row_size, col_size);
    wsp::math::SwapQuadrantVer(tmp_img, io_img, row_size, col_size);
}

template<typename T>
inline void wsp::math::SwapComplexQuadrantHor(const T *in_src, T *o_dst, int row_size, int col_size)
{
    int length = row_size * col_size;
    int hw = row_size/2;
    int hh = col_size/2;
    //** swap
    {
        int x, y;
        for(y=0; y<height; y++){
            for(x=0; x<width; x++){
                j = (y*width + x)*2;
                if(y<hh && x<hw){
                    i = (y*width + x + hw)*2;
                }else if(y<hh && x>=hw){
                    i = (y*width + x - hw)*2;
                }else if(y>=hh && x<hw){
                    i = (y*width + x + hw)*2;
                }else{
                    i = (y*width + x - hw)*2;
                }
                o_dst[j] = in_src[i];
                o_dst[j+1] = in_src[i+1];
            }
        }
    }
}
template<typename T>
inline void wsp::math::SwapComplexQuadrantVer(const T *in_src, T *o_dst, int row_size, int col_size)
{
    int length = row_size * col_size;
    int hw = row_size/2;
    int hh = col_size/2;
    //** swap
    {
        int x, y, i, j;
        for(y=0; y<col_size; ++y){
            for(x=0; x<row_size; ++x){
                j = (y*row_size + x)*2;
                if(y<hh && x<hw){
                    i = ((y+hh)*row_size + x)*2;
                }else if(y<hh && x>=hw){
                    i = ((y+hh)*row_size + x)*2;
                }else if(y>=hh && x<hw){
                    i = ((y-hh)*row_size + x)*2;
                }else{
                    i = ((y-hh)*row_size + x)*2;
                }
                o_dst[j] = in_src[i];
                o_dst[j+1] = in_src[i+1];
            }
        }
    }
}
template<typename T>
inline void wsp::math::SwapComplexQuadrantHV(T *tmp_complex, T *io_complex, int row_size, int col_size)
{
    wsp::math::SwapComplexQuadrantHor(io_complex, tmp_complex, row_size, col_size);
    wsp::math::SwapComplexQuadrantVer(tmp_complex, io_complex, row_size, col_size);
}

#endif

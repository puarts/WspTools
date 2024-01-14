/**
 * @file fn-imgfilter.cpp
 * 
 */

#include <wsp/common/WspFloat4.h>
#include <wsp/common/WspRgb.h>
#include "fn-imgfilter.h"


void wsp::img::GaussianFilterSimpleRecursive(u32 *o_dst, u32 *in_src, u32 w, u32 h, float a)
{
    for(int x=0; x<w; ++x)
    {
        if (x >= w){ continue; }
    
        in_src += x;    // advance pointers to correct column
        o_dst += x;

        // forward pass
        WspFloat4 pix, pix_dst, pix_pre, pix_tmp;  // previous output
        pix_pre.FromAbgr(*in_src);
        for (int y = 0; y < h; ++y) 
        {
            pix.FromAbgr(*in_src);
            /* simple lerp between current and previous value */
            pix_dst = pix + (pix_pre - pix) * a;
            *o_dst = pix_dst.ToAbgr();

            /* move to next row */
            in_src += w; 
            o_dst += w;
            pix_pre = pix_dst;
        }

        // reset pointers to point to last element in column
        in_src-= w;
        o_dst -= w;

        // reverse pass
        // ensures response is symmetrical
        pix_pre.FromAbgr(*in_src);
        for (int y = h-1; y >= 0; --y) 
        {
            pix.FromAbgr(*in_src);
            pix_dst = pix + (pix_pre - pix)*a;
            pix_tmp.FromAbgr(*o_dst);
            *o_dst = ((pix_tmp + pix_dst)*0.5f).ToAbgr();
            // move to previous row
            in_src -= w; 
            o_dst -= w;  
            pix_pre = pix_dst;
        }
    }
}


//
//void wsp::img::MedianRgb(wsp::Image<WspRgb> &io_img, u32 block_size)
//{
//    int itr_x, itr_y, itr_i, itr_j; //! iterators
//    int xx, yy; //! coord within block
//    int cnt, index;
//    int bsize=(int)block_size;
//    int width=(int)io_img.width(), height=(int)io_img.height();
//    uchar bufary[3][MAX_BUFFER];
//    WspRgb pix;
//    wsp::Image<WspRgb> src(io_img);
//
//    if(width*height==0 || block_size == 0){ return; }
//
//    printf("MEDIAN: block_size=%d\n\n", block_size);
//
//    for(itr_y = 0; itr_y < height; itr_y++){
//        for(itr_x = 0; itr_x < width; itr_x++){
//            cnt=0;
//            //! iteration within block
//            for(itr_i = -bsize; itr_i <= bsize; itr_i++){
//                for(itr_j = -bsize; itr_j <= bsize; itr_j++){
//                    yy = itr_y + itr_i;
//                    xx = itr_x + itr_j;
//                    if( src.IsOutOfBounds((u32)xx, (u32)yy)){ continue; }
//                    bufary[0][cnt] = src.GetElem((u32)xx, (u32)yy).r;
//                    bufary[1][cnt] = src.GetElem((u32)xx, (u32)yy).g;
//                    bufary[2][cnt] = src.GetElem((u32)xx, (u32)yy).b;
//                    cnt++;
//                }
//            }
//            if(cnt==0){ continue; }
//
//            //wsp::QuickSort<T>(bufary, (u32)cnt);
//            qsort(bufary[0], (size_t)cnt, sizeof(uchar), wsp::CompA<uchar>);
//            qsort(bufary[1], (size_t)cnt, sizeof(uchar), wsp::CompA<uchar>);
//            qsort(bufary[2], (size_t)cnt, sizeof(uchar), wsp::CompA<uchar>);
//
//            index = cnt/2;
//            pix = WspRgb(bufary[0][index], bufary[1][index], bufary[2][index]);
//            io_img.SetElem(pix, (u32)itr_x, (u32)itr_y);
//        }
//    }
//}

wsp::State wsp::img::LaplacianFilter(wsp::Image<double> &o_edge, const wsp::Image<uchar> &in_img)
{
    int width, height, nc;
    int itr_x, itr_y, itr_c, itr_u, itr_v; //! iterators
    int index_x, index_y, index_u, index_v;
    int filterSize = 1;
    double filter[3][3]={{-1, -1, -1},
                         {-1,  8, -1},
                         {-1, -1, -1}};
    double sum_rgb[3];
    double max;

    width = (int)in_img.width();
    height = (int)in_img.height();
    nc = (int)in_img.num_of_channels();
    o_edge.SetSize(width, height);

    printf("LaplacianFilter: width=%d, height=%d\n", width, height);

    for(itr_y=0; itr_y<height; itr_y++){
        for(itr_x=0; itr_x<width; itr_x++)
        {
            //! Ignoring edge process
            if(itr_y==0 || itr_x==0 || itr_y==height-1 || itr_x==width-1){
                o_edge.SetElem(0, (double)itr_x, (double)itr_y);
                continue;
            }
            sum_rgb[0] = sum_rgb[1] = sum_rgb[2]=0;
            for(itr_v=-filterSize; itr_v<=filterSize; itr_v++){
                for(itr_u=-filterSize; itr_u<=filterSize; itr_u++){
                    index_x = itr_x+itr_u;         
                    index_y = itr_y+itr_v;
                    index_u = itr_u+filterSize;    
                    index_v = itr_v+filterSize;
                    for(itr_c=0; itr_c<nc; itr_c++){
                        sum_rgb[itr_c] += in_img[(index_y*width+index_x)*nc+itr_c] * filter[index_v][index_u];
                    }
                    //sum_rgb[0] += in_img.GetElem(index_x, index_y).r * filter[index_v][index_u];
                    //sum_rgb[1] += in_img.GetElem(index_x, index_y).g * filter[index_v][index_u];
                    //sum_rgb[2] += in_img.GetElem(index_x, index_y).b * filter[index_v][index_u];
                }
            }
            //! using max value in RGB
            max = sum_rgb[0];
            if(max < sum_rgb[1]){ max = sum_rgb[1]; }
            if(max < sum_rgb[2]){ max = sum_rgb[2]; }
 
            o_edge.SetElem(max, (u32)itr_x, (u32)itr_y);
        }
    }
    return WSP_STATE_SUCCESS;
}

//
//wsp::State wsp::img::LaplacianFilter(wsp::Image<double> &o_edge, const wsp::Image<WspRgb> &in_img)
//{
//    int width, height;
//    int itr_x, itr_y, itr_u, itr_v; //! iterators
//    int index_x, index_y, index_u, index_v;
//    int filterSize = 1;
//    double filter[3][3]={{-1, -1, -1},
//                         {-1,  8, -1},
//                         {-1, -1, -1}};
//    double sum_rgb[3];
//    double max;
//
//    width = (int)in_img.width();
//    height = (int)in_img.height();
//    o_edge.SetSize(width, height);
//
//    printf("LaplacianFilter: width=%d, height=%d\n", width, height);
//
//    for(itr_y=0; itr_y<height; itr_y++){
//        for(itr_x=0; itr_x<width; itr_x++)
//        {
//            //! Ignoring edge process
//            if(itr_y==1 || itr_x==1 || itr_y==height-1 || itr_x==width-1){
//                o_edge.SetElem(0, (double)itr_x, (double)itr_y);
//                continue;
//            }
//            sum_rgb[0] = sum_rgb[1] = sum_rgb[2]=0;
//            for(itr_v=-filterSize; itr_v<=filterSize; itr_v++){
//                for(itr_u=-filterSize; itr_u<=filterSize; itr_u++){
//                    index_x = itr_x+itr_u;         index_y = itr_y+itr_v;
//                    index_u = itr_u+filterSize;    index_v = itr_v+filterSize;
//                    sum_rgb[0] += in_img.GetElem(index_x, index_y).r * filter[index_v][index_u];
//                    sum_rgb[1] += in_img.GetElem(index_x, index_y).g * filter[index_v][index_u];
//                    sum_rgb[2] += in_img.GetElem(index_x, index_y).b * filter[index_v][index_u];
//                }
//            }
//            //! using max value in RGB
//            max = sum_rgb[0];
//            if(max < sum_rgb[1]){ max = sum_rgb[1]; }
//            if(max < sum_rgb[2]){ max = sum_rgb[2]; }
//
//            o_edge.SetElem(max, (u32)itr_x, (u32)itr_y);
//        }
//    }
//    return WSP_STATE_SUCCESS;
//}


wsp::State wsp::img::SobelFilter(double *o_edge, const uchar *in_img, u32 width, u32 height, u32 num_of_channels)
{
    u32 *sumw, *sumh;
    int y, x, k, l, c, index;
    double v;
    int w=static_cast<int>(width), h=static_cast<int>(height), nc=static_cast<int>(num_of_channels);

    if(w==0 || h==0 || nc==0){
        WSP_COMMON_ERROR_LOG("Input image is invalid\n"); return WSP_STATE_FAILURE;
    }
    sumw = WSP_NEW u32[nc]; //! buffer for horizontal differentiation
    sumh = WSP_NEW u32[nc]; //! buffer for vertical differentiation

    for(y=1; y<h-1; y++){
        for(x=1; x<w-1; x++){
            for(c=0; c<nc; c++){
                sumw[c]=0; sumh[c]=0;
            }
            for(k=-1; k<=1; k++){
                for(l=-1; l<=1; l++){
                    for(c=0; c<nc; c++){
                        index = ((y+k)*w+x+l)*nc+c;
                        sumw[c] += (!k ? 2*l : l) * in_img[index];
                        sumh[c] += (!l ? 2*k : k) * in_img[index];
                    }
                }
            }
            v = 0.0;
            for(c=0; c<nc; c++){
                v += sqrt(static_cast<double>(sumw[c]*sumw[c] + sumh[c]*sumh[c]));
            }
            o_edge[y*w+x]=v;
        }
    }
    delete[] sumw;
    delete[] sumh;
    return WSP_STATE_SUCCESS;
}
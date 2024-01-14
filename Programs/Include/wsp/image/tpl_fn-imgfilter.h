/**
 * @file tpl_fn-imgfilter.h
 * 
 * Functions used to perform various linear or non-linear filtering operations on 2D images
 */

#ifndef WspImage_WspFnImgFilter_tpl_H_
#define WspImage_WspFnImgFilter_tpl_H_

#include "tpl_cl-image.h"


namespace wsp{ 
    namespace img{


    template<typename T> 
    inline wsp::State ApplyFilterItar(
        wsp::State (*filter_func)(
            wsp::Image<T> &o_img, 
            u32 filter_size, 
            const wsp::Image<uchar> *in_mask
        ), 
        wsp::Image<T> &io_img, 
        wsp::img::ImgFilterParam &param, 
        const wsp::Image<uchar> *in_mask=NULL
    );

    // ==============================
    // Smoothing Filters
    // ==============================

    // core functions
    template<typename T> wsp::State Median(wsp::Image<T> &io_img, u32 block_size, const wsp::Image<uchar> *in_mask=NULL);
    template<typename T> wsp::State AverageFilter(wsp::Image<T> &io_img, u32 filterSize=1, const wsp::Image<uchar> *in_mask=NULL);
    template<typename T> wsp::State AverageFilterSegm(wsp::Image<T> &io_img, const wsp::Image<int> &in_segm, u32 block_size, u32 iteration=1);

    // overwrapped functions
    template<typename T> inline wsp::State AverageFilterIter(wsp::Image<T> &io_img, wsp::img::ImgFilterParam &param, const wsp::Image<uchar> *in_mask=NULL){
        return wsp::img::ApplyFilterItar<T>(wsp::img::AverageFilter<T>, io_img, param, in_mask);
    }
    template<typename T> inline wsp::State MedianFilterIter(wsp::Image<T> &io_img, wsp::img::ImgFilterParam &param, const wsp::Image<uchar> *in_mask=NULL){
        return wsp::img::ApplyFilterItar<T>(wsp::img::Median<T>, io_img, param, in_mask);
    }


    template<typename T>
    wsp::State LaplacianFilter(wsp::Image<double> &o_edge, const wsp::Image<T> &in_img);
    }
}

//! Smoothing Filters ------------------------------------------------
template<typename T> inline wsp::State wsp::img::ApplyFilterItar(
    wsp::State (*filter_func)(wsp::Image<T> &o_img, u32 filter_size, const wsp::Image<uchar> *in_mask), 
    wsp::Image<T> &io_img, wsp::img::ImgFilterParam &param, const wsp::Image<uchar> *in_mask)
{
    wsp::State state;
    wsp::Image<uchar> *mask_img=NULL;
    if(param.use_alpha_as_mask==false && in_mask!=NULL){
        mask_img = WSP_NEW wsp::Image<uchar>();
        *mask_img = *in_mask;
    }
    //for(int i=0; i<io_img.length(); i++){
    //    printf("%d  %d\n", i, io_img.alpha_data()[i]);
    //}
    if(param.use_alpha_as_mask){
        mask_img = WSP_NEW wsp::Image<uchar>();
        io_img.CopyAlphaInto(*mask_img);
        //mask_img = WSP_NEW wsp::Image<uchar>(io_img.alpha_data(), NULL, NULL, io_img.width(), io_img.height(), 1);
    }
    //for(int i=0; i<mask_img->length(); i++){
    //    printf("%d  %d\n", i, (*mask_img)[i]);
    //}
    for(int i=0; i<param.num_of_iteration; i++)
    {
        state=filter_func(io_img, param.filter_size, mask_img);
        if(state!=WSP_STATE_SUCCESS)
        { 
            WSP_COMMON_ERROR_LOG("Applying image filter failed\n"); 
            return state; 
        }
    }
    if(mask_img!=NULL){
        delete mask_img;
    }
    return WSP_STATE_SUCCESS;
}

template<typename T> 
wsp::State wsp::img::AverageFilterSegm(wsp::Image<T> &io_img, const wsp::Image<int> &in_segm, u32 filter_size, u32 iteration)
{
    if(in_segm.num_of_channels()!=1){
        WSP_COMMON_ERROR_LOG("Numbe of channels of segmentation image must be 1\n");
        return WSP_STATE_FAILURE;
    }
    u32 w=io_img.width(), h=io_img.height(), nc=io_img.num_of_channels();
    wsp::Vector<int> segmList;
    wsp::Image<uchar> mask;
    char filename[MAX_LENGTH_OF_PATH];
    in_segm.GetIndexList(segmList);
    int nSegm = segmList.length();
    mask.SetSize(w, h, 1);

    printf("AverageFilterSegm: filter size=%d, num of segm=%d\n", filter_size, nSegm);

    int segm;
    int i_segm, i;
    wsp::img::ImgFilterParam param(filter_size, iteration, false);
    for(i_segm=0; i_segm<nSegm; i_segm++){
        segm=segmList[i_segm];
        mask.Reset(0);
        for(i=0; i<mask.length(); i++){
            if(in_segm[i]==segm){
                mask[i]=255;
                continue;
            }
        }
        sprintf(filename, "mask_%d.png", segm);
        mask.SaveImage(filename);
        //wsp::img::AverageFilter(io_img, filter_size, &mask);
        wsp::img::AverageFilterIter(io_img, param, &mask);
    }
    return WSP_STATE_SUCCESS;
}

template<typename T> 
wsp::State wsp::img::AverageFilter(wsp::Image<T> &io_img, u32 filterSize, const wsp::Image<uchar> *in_mask)
{
    int x, y, i, j, c;
    int index;
    int cnt;
    //wsp::Image<T> sumImg;
    double *val;
    int blockSize = static_cast<int>(filterSize); //! actual size is 2*blockSize + 1
    int width = (int)io_img.width(), height = (int)io_img.height(), nc = (int)io_img.num_of_channels();
    //sumImg.SetSize(width, height);
    val = WSP_NEW double[nc];
    uchar thre_alpha = 128;

    ////! Get sum of x lines
    //for(y=0; y<height; y++){
    //    val=0;
    //    for(x=0; x<width; x++){
    //        val += io_img.GetElem(x, y);
    //        sumImg.SetElem(val, x, y);
    //    }
    //}
    ////! Get sum of y lines
    //for(x=0; x<width; x++){
    //    val=0;
    //    for(y=0; y<height; y++){
    //        val += sumImg.GetElem(x, y);
    //        sumImg.SetElem(val, x, y);
    //    }
    //}
    ////! Get Average from summed image
    //for(y=0; y<height; y++){
    //    for(x=0; x<width; x++){
    //        val = ((sumImg.GetElem(x+blockSize, y+blockSize) + sumImg.GetElem(x-blockSize-1, y+blockSize))
    //              -(sumImg.GetElem(x+blockSize, y-blockSize-1) - sumImg.GetElem(x-blockSize-1, y-blockSize-1))) / 9.0;
    //        io_img.SetElem(val, x, y);
    //    }
    //}

    //! Get Average
    for(y=0; y<height; y++){
        for(x=0; x<width; x++){
            index=y*width+x;
            if(in_mask!=NULL){
                if((*in_mask)[index]<thre_alpha){ continue; }
            }
            for(c=0; c<nc; c++){
                val[c] = 0;
            }
            cnt = 0;
            for(j=-blockSize; j<=blockSize; j++){
                for(i=-blockSize; i<=blockSize; i++){
                    if(in_mask!=NULL){
                        if((*in_mask)[index]<thre_alpha){ continue; }
                    }
                    if(x+i<0 || x+i>=width || y+j<0 || y+j>=height){
                        continue;
                    }
                    for(c=0; c<nc; c++){
                        val[c] += (double)io_img[((y+j)*width+x+i)*nc+c];
                    }
                    cnt++;
                }
            }
            if(cnt==0){
                continue;
            }
            for(c=0; c<nc; c++){
                io_img[index*nc+c]=(T)((double)val[c]/(double)cnt);
            }
        }
    }
    delete[] val;

    return WSP_STATE_SUCCESS;
}

template<typename T> wsp::State wsp::img::Median(wsp::Image<T> &io_img, u32 block_size, const wsp::Image<uchar> *in_mask)
{
    int itr_x, itr_y, itr_i, itr_j, c; //! iterators
    int index;
    int xx, yy; //! coord within block
    int cnt, bsize=(int)block_size;
    int width=(int)io_img.width(), height=(int)io_img.height(), nc=(int)io_img.num_of_channels();
    T **bufary;
    T pix;
    wsp::Image<T> src(io_img);
    uchar thre_alpha = 128;

    if(width*height==0 || block_size == 0){ return WSP_STATE_FAILURE; }
    bufary = WSP_NEW T*[nc];
    for(c=0; c<nc; c++){
        bufary[c] = WSP_NEW T[(bsize*2+1)*(bsize*2+1)];
    }

    printf("MEDIAN: filter_size=%d\n\n", block_size);



    for(itr_y = 0; itr_y < height; itr_y++){
        for(itr_x = 0; itr_x < width; itr_x++){
            index = itr_y * width + itr_x;
            if(in_mask!=NULL){
                if((*in_mask)[index]<thre_alpha){ 
                    //printf("skipped: %d\n", (*in_mask)[index]); 
                    continue; 
                }
            }
            cnt=0;
            //! iteration within block
            for(itr_i = -bsize; itr_i <= bsize; itr_i++){
                for(itr_j = -bsize; itr_j <= bsize; itr_j++){
                    yy = itr_y + itr_i;
                    xx = itr_x + itr_j;
                    if( src.IsOutOfBounds((u32)xx, (u32)yy)){ continue; }
                    if(in_mask!=NULL){
                        if((*in_mask)[yy*width+xx]<thre_alpha){ continue; }
                    }
                    for(c=0; c<nc; c++){
                        bufary[c][cnt] = src[(yy*width+xx)*nc+c];
                    }
                    //bufary[cnt] = src.GetElem((u32)xx, (u32)yy);
                    cnt++;
                }
            }
            if(cnt==0){ continue; }
            //wsp::QuickSort<T>(bufary, (u32)cnt);
            for(c=0; c<nc; c++){
                qsort(bufary[c], cnt, sizeof(T), wsp::CompA<T>);
                pix = bufary[c][cnt/2];
                io_img[(itr_y*width+itr_x)*nc+c] = pix;
            }
        }
    }
    //! freeing memory
    for(c=0; c<nc; c++){
        delete[] bufary[c];
    }
    delete[] bufary;
    return WSP_STATE_SUCCESS;
}


template<typename T>
wsp::State wsp::img::LaplacianFilter(wsp::Image<double> &o_edge, const wsp::Image<T> &in_img)
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

    width = in_img.width();
    height = in_img.height();
    nc = in_img.num_of_channels();
    o_edge.SetSize(width, height);

    printf("LaplacianFilter: width=%d, height=%d\n", width, height);

    for(itr_y=0; itr_y<height; itr_y++){
        for(itr_x=0; itr_x<width; itr_x++)
        {
            // Ignoring edge process
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
            // using max value in RGB
            max = sum_rgb[0];
            if(max < sum_rgb[1]){ max = sum_rgb[1]; }
            if(max < sum_rgb[2]){ max = sum_rgb[2]; }
 
            o_edge.SetElem(max, itr_x, itr_y);
        }
    }
    return WSP_STATE_SUCCESS;
}


#endif
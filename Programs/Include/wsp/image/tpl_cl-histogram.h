/**
 * @file tpl_cl-histogram.h
 * 
 */

#ifndef _WSP_IMG_CL_HISTOGRAM_H_
#define _WSP_IMG_CL_HISTOGRAM_H_

#ifndef __WSP_COMMONUTIL_TEMPLATE_CLASS_VECTOR_H__
#include <wsp/common/tpl_cl-vector.h>
#endif



namespace wsp{
    #define WSP_INIT_HISTOGRAM range_min_(0), range_max_(0)

    //! \ingroup WspImage
    //! \brief histogram manipulation 
    /*!
      wsp::Histogram is a class which provides histogram functionality. 
    */
    template<typename T>
    class Histogram
        : public wsp::Vector<int>
    {
    public:
        Histogram();
        Histogram(T *data, int length, int num_channels, T min=0, T max=255);

        // accessor -------------------------------------------
        T range_min() const{ return range_min_; }
        T range_max() const{ return range_max_; }
        int total_num_pixels() const{ return total_num_pixels_; }
        int CountNumOfLevelsContainingPixels() const;

        // mutator --------------------------------------------
        void SetData(const T *data, int length, int num_channels, T min=0, T max=255);
        void SetData(wsp::Vector<T> &ary, T min=0, T max=255){
            SetData(ary.data(), ary.length(), ary.num_of_channels(), min, max);
        }

        // I/O ------------------------------------------------
        void PrintData(int channel=-1); //! arg channel=-1 means sum of all channels

    private:
        T range_min_, range_max_;
        int total_num_pixels_;
    };
}

// Constructor ------------------------------------------------
template<typename T>
wsp::Histogram<T>::Histogram()
    : WSP_INIT_HISTOGRAM
{}

template<typename T>
wsp::Histogram<T>::Histogram(T *data, int length, int num_channels, T min, T max)
    : WSP_INIT_HISTOGRAM
{
    SetData(data, length, num_channels, min, max);
}

template<typename T>
int wsp::Histogram<T>::CountNumOfLevelsContainingPixels() const
{
    const int *ptr = this->data();
    const int *end_ptr = ptr + this->num_of_elem();
    int cnt = 0;
    for( ; ptr!=end_ptr; ++ptr)
    {
        if( *ptr != 0 ){
            ++cnt;
        }
    }
    return cnt;
}

// Mutators ---------------------------------------------------
template<typename T>
void wsp::Histogram<T>::SetData(const T *data, int length, int num_channels, T min, T max)
{
    if(length==0 || num_channels==0){ WSP_COMMON_ERROR_LOG("length is zero\n"); return; }
    T *ptr, *end_ptr;

    // allocate memory
    this->SetLength(static_cast<int>(max-min+1), num_channels);
    //this->Reset(0);
    range_min_ = static_cast<T>(min);
    range_max_ = static_cast<T>(max);
    total_num_pixels_ = length*num_channels;

    //**initialize histogram
    //memset(data_, 0, length_*sizeof(int));
    Reset(0);

    // input data of histogram
    int index, c;
    ptr = const_cast<T*>(data);
    end_ptr = ptr + total_num_pixels_;
    c=0;
    for(; ptr!=end_ptr; ){
        for(c=0; c<num_channels; ++c, ++ptr){
            index = static_cast<int>(*ptr - min)*num_channels+c;
            ++(data_[index]);
        }
    }
}

// I/O --------------------------------------------------------
template<typename T>
void wsp::Histogram<T>::PrintData(int channel)
{
    int *ptr, *end_ptr;

    if(channel=-1){
        //! show sum of all channels
        ptr = data_;
        end_ptr = ptr + length_*num_of_channels_;
        for(int c=0, sum=0, i=0; ptr!=end_ptr; ++ptr){
            sum+=*ptr;
            ++c;
            if(c==num_of_channels_){
                printf("%d : %d\n", i, sum);
                c=0;
                sum=0;
                ++i;
            }
        }
    }else{
        //! show only one channel
        ptr = data_ + channel;
        end_ptr = ptr + length_*num_of_channels_;
        for(int i=0; ptr!=end_ptr; ptr+=num_of_channels_, ++i){
            printf("%d : %d\n", i, *ptr);
        }
    }
}



#endif
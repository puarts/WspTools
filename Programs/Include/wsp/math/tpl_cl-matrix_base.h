// @file tpl_cl-matrix_base.h
// 
// Description: Base class for any type of image class

#ifndef _WSP_MATH_TEMPLATE_CLASS_MATRIX_BASE_H_
#define _WSP_MATH_TEMPLATE_CLASS_MATRIX_BASE_H_

#ifndef __WSP_COMMONUTIL_TEMPLATE_CLASS_VECTOR_H__
#include <wsp/common/tpl_cl-vector.h>
#endif
#include <wsp/image/_fn-imgio.h>
//#include <wsp/common/WspRgb.h>
#ifndef __WSP_COMMONUTIL_FN_UTIL_H__
#include <wsp/common/fn-util.h>
#endif

#ifdef USE_QT
#include <wsp/common/_define_qt.h>
#include <QtGui/QtGui>
#endif


namespace wsp{
    //! \ingroup WspMath
    //! \brief Base matrix manipulation class allocated in heap memory
    template<class T>
    class MatrixBase
        : public wsp::Vector<T>
    {
    protected:
        u32 column_size_, row_size_;

    public:
        MatrixBase();
        explicit MatrixBase(u32 column_size, u32 hieght, u32 nChannels=1);
        //! if dst_nChannels=0, nChannels will be the same as src_nChannels
        //template<typename U> explicit MatrixBase(const U* ary, u32 column_size, u32 row_size, u32 src_nChannels=1, u32 dst_nChannels=0); 
        template<typename U> explicit MatrixBase(const wsp::MatrixBase<U> &in_img, u32 dst_num_of_channels=0);
        template<typename U> explicit MatrixBase(const U *in_data, u32 column_size, u32 row_size, u32 nChannels);
        MatrixBase(const wsp::MatrixBase<T> &img);
        MatrixBase(int length); //! for int casting 
        ~MatrixBase();


        // Accessors: Getter -------------------------------
        inline u32 column_size() const;
        inline u32 row_size() const;

        inline u32* column_size_ptr(){ return &column_size_; }
        inline u32* row_size_ptr(){ return &row_size_; }

        inline u32 GetDataSize() const;
        inline u32 GetMatrixBaseSize() const;

        inline T GetElem(u32 x, u32 y, u32 c=0) const;
        inline T GetElem(int x, int y, int c=0) const;
        inline T GetElem(u32 index) const;
        inline T GetElem(int index) const;
        //inline T*Data() const;




        void GetMaxLoc(int *o_x, int *o_y, T *o_value) const;
        void GetMinLoc(int *o_x, int *o_y, T *o_value) const;
        void GetMaxLoc(double *o_x, double *o_y, T *o_value) const;
        void GetMinLoc(double *o_x, double *o_y, T *o_value) const;
        void GetMaxIndex(int *o_index, T *o_value) const;
        inline bool IsOutOfBounds(int x, int y, int channel=0) const;
        template<typename U> inline bool IsTheSameSize(const wsp::MatrixBase<U> &in_img) const;

        //! if dataSize and nChannels=0, parameters become the same as source
        template<typename U> void CopyIntoAnyMatrixBase(wsp::MatrixBase<U> &o_dst, u32 num_of_channels_dst=0) const;
        template<typename U> void CopyIntoAnyMatrixBaseAsU8(wsp::MatrixBase<U> &o_dst) const;
        int wsp::MatrixBase<T>::GetNumOfIndex() const;
        wsp::State GetIndexList(wsp::Vector<T> &o_indexList) const;

        // Accessors: Setter -------------------------------
        template<typename U> inline wsp::State CopyFrom(const wsp::MatrixBase<U>& img, u32 dst_num_of_channels=0);
        template<typename U> wsp::State CopyFrom(const U* data, u32 column_size, u32 row_size, u32 src_num_of_channels, u32 dst_num_of_channels=0);
        inline void SetDataFrom(T *data, u32 column_size, u32 row_size, u32 num_of_channels);
        inline void SetDataFrom(T *data, u32 column_size, u32 row_size, u32 num_of_channels);
        inline void SetDataInto(T *o_data, u32 *o_column_size, u32 *o_row_size, u32 *o_num_of_channels);
        inline void SetDataInto(T *o_data, u32 *o_column_size, u32 *o_row_size, u32 *o_num_of_channels);
        inline void AddElem(const T& element, u32 x, u32 y, u32 channel=0);
        inline void SetValueType(wsp::img::DepthValueType type){ value_type_ = type; }
        //! nChannels=0 means keep old num of channels
        wsp::State SetSize(u32 column_size, u32 row_size, u32 nChannels=0); 
        inline wsp::State SetNumOfChannels(u32 num_of_channels){ return SetSize(column_size_, row_size_, num_of_channels); }
        //inline wsp::State SetSize(int column_size, int row_size, int nChannels=0);
        //inline void Reset(const T& value);
        //! fill_pixel_data must has the same length data as num_of_channels_
        inline void SetValueAtTargetStatePixel(const T *in_fill_pixel_data, wsp::img::PixelState target_state); 

        inline wsp::State Clear();
        void Invert(); //! invert all pixel values
        void Transpose();


        //! IO ----------------------------------------------
        wsp::State LoadMatrixBase(const char *filename, u32 nChannels=0); //! if nChannels=0, nChannels depend on loaded file
        wsp::State SaveMatrixBase(const char *filename) const;

        //! Save image as tiff using Libtiff
        #ifdef USE_LIBTIFF
        wsp::State LoadTiff(const char *filename);
        wsp::State SaveAsTiff(const char *filename, wsp::img::PixelState redState=WSP_PIXEL_STATE_NORMAL) const;
        #endif

        #ifdef USE_QT
        //! get file name from dialog
        inline wsp::State SaveMatrixBaseDialog(wsp::img::PixelState redStat=WSP_PIXEL_STATE_NORMAL, const char *title="Save As") const;
        inline wsp::State LoadMatrixBaseDialog(u32 nChannels=0, const char *title="Open");
        #endif

        wsp::State SaveRawData(const char *in_fileNameFormat,...);
        wsp::State LoadRawData(const char *fileName);

        // Operators ---------------------------------------
        wsp::MatrixBase<T>& operator=(const wsp::MatrixBase<T>& other);
        bool operator==(const wsp::MatrixBase<T>& other) const;
        bool operator!=(const wsp::MatrixBase<T>& other) const;

        //! Qt -----------------------------------------------
        #ifdef USE_QT
        wsp::State CopyInto(QMatrixBase &o_qimg, bool withOcclusion) const;
        wsp::State CopyFrom(const QMatrixBase &in_src);
        #endif

        //! GUI ----------------------------------------------
        #ifdef USE_OPENCV
        void ShowMatrixBase();
        #endif
    



    };

}


// Constructor, Destructor -------------------------------------------------------------------

#define INIT_WSP_IMAGE column_size_(0),row_size_(0)

template<class T> wsp::MatrixBase<T>::MatrixBase()
    :INIT_WSP_IMAGE, wsp::Vector<T>()
{}
template<class T> wsp::MatrixBase<T>::MatrixBase(u32 column_size, u32 row_size, u32 nChannels)
    :INIT_WSP_IMAGE, wsp::Vector<T>()//, wsp::Vector<T>(column_size*row_size*nChannels)
{
    SetSize(column_size, row_size, nChannels);
}

template<class T> template<typename U>
wsp::MatrixBase<T>::MatrixBase(const wsp::MatrixBase<U> &in_img, u32 num_of_channels_dst)
    :INIT_WSP_IMAGE, wsp::Vector<T>()
//wsp::MatrixBase<T>(in_img.data(), in_img.column_size(), in_img.row_size(), in_img.num_of_channels(), dst_num_of_channels)
{
    //has_own_data_=true;
    in_img.CopyIntoAnyMatrixBase<T>(*this, num_of_channels_dst);
}
template<class T> template<typename U>
wsp::MatrixBase<T>::MatrixBase(const U *in_data, u32 column_size, u32 row_size, u32 nChannels)
    :INIT_WSP_IMAGE, wsp::Vector<T>()
{
    //has_own_data_=true;
    CopyFrom(in_data,column_size, row_size, nChannels);
}

//! Copy Constructor
template<class T> wsp::MatrixBase<T>::MatrixBase(const wsp::MatrixBase<T>& im)
    :INIT_WSP_IMAGE, wsp::Vector<T>()
{
    if(im.data() != NULL && im.length() != 0){
        CopyFrom(im);
    }
}
template<class T> wsp::MatrixBase<T>::MatrixBase(int length)
    :INIT_WSP_IMAGE, wsp::Vector<T>()
{
    if(length<=0){ Clear(); return; }
    SetSize(length, 1);
}
//! Destructor
template<class T> wsp::MatrixBase<T>::~MatrixBase(){}
// end Constructor, Destructor ---------------------------------------------------------------


// Accessors: Getter --------------------------------------------------------------------------
template<class T> inline u32 wsp::MatrixBase<T>::column_size() const{ return column_size_; }
template<class T> inline u32 wsp::MatrixBase<T>::row_size() const{ return row_size_; }

template<class T> inline u32 wsp::MatrixBase<T>::GetMatrixBaseSize() const{ return row_size_*column_size_; }
template<class T> inline u32 wsp::MatrixBase<T>::GetDataSize() const{ return GetMatrixBaseSize() * sizeof(T); }

template<class T> inline T wsp::MatrixBase<T>::GetElem(u32 x, u32 y, u32 c) const{
    u32 index = (y*column_size_ + x)*num_of_channels_ + c;
    if(index<column_size_*row_size_){    return data_[index]; }
    return data_[column_size_*row_size_-1];
}
template<class T> inline T wsp::MatrixBase<T>::GetElem(int x, int y, int c) const{
    return wsp::MatrixBase<T>::GetElem((u32)x, (u32)y, (u32)c);
}
template<class T> inline T wsp::MatrixBase<T>::GetElem(u32 index) const{
    return wsp::MatrixBase<T>::operator[](index);
}
template<class T> inline T wsp::MatrixBase<T>::GetElem(int index) const{
    return wsp::MatrixBase<T>::operator[](static_cast<u32>(index));
}

template<class T> inline bool wsp::MatrixBase<T>::IsOutOfBounds(int x, int y, int c) const{
    return (x>=static_cast<int>(column_size_) || y>=static_cast<int>(row_size_) || x<0 || y<0 || c>=static_cast<int>(num_of_channels_) || c<0);
}


template<class T> void wsp::MatrixBase<T>::GetMaxLoc(int *o_x, int *o_y, T *o_value) const{
    if(column_size_*row_size_==0 || data_==NULL){ return; }
    int maxX=0, maxY=0;
    T maxVal = data_[0];
    for(int itr_y=0; itr_y<row_size_; itr_y++){
        for(int itr_x=0; itr_x<column_size_; itr_x++){
            if(GetElem((u32)itr_x, (u32)itr_y) > maxVal){
                maxVal = GetElem((u32)itr_x, (u32)itr_y);
                maxX = itr_x;
                maxY = itr_y;
            }
        }
    }
    if(o_value != NULL){ *o_value = maxVal; }
    if(o_x != NULL){ *o_x = maxX; }
    if(o_y != NULL){ *o_y = maxY; }
}
template<class T> void wsp::MatrixBase<T>::GetMinLoc(int *o_x, int *o_y, T *o_value) const{
    if(column_size_*row_size_==0 || data_==NULL){ return; }
    int minX=0, minY=0;
    T minVal = data_[0];
    for(int itr_y=0; itr_y<row_size_; itr_y++){
        for(int itr_x=0; itr_x<column_size_; itr_x++){
            if(GetElem((u32)itr_x, (u32)itr_y) < minVal){
                minVal = GetElem((u32)itr_x, (u32)itr_y);
                minX = itr_x;
                minY = itr_y;
            }
        }
    }
    if(o_value!=NULL){ *o_value = minVal; }
    if(o_x!=NULL){ *o_x = minX; }
    if(o_y!=NULL){ *o_y = minY; }
}
template<class T> void wsp::MatrixBase<T>::GetMaxLoc(double *o_x, double *o_y, T *o_value) const{
    int px, py;
    GetMaxLoc(&px, &py, o_value);
    if(o_x!=NULL){ *o_x = (double)px; }
    if(o_y!=NULL){ *o_y = (double)py; }
}
template<class T> void wsp::MatrixBase<T>::GetMinLoc(double *o_x, double *o_y, T *o_value) const{
    int px, py;
    GetMinLoc(&px, &py, o_value);
    if(o_x!=NULL){ *o_x = (double)px; }
    if(o_y!=NULL){ *o_y = (double)py; }
}
template<class T> void wsp::MatrixBase<T>::GetMaxIndex(int *o_index, T *o_value) const{
    double px, py;
    GetMaxLoc(&px, &py, o_value);
    if(o_index!=NULL){ *o_index = (int)(py*column_size_ + px); }
}

template<class T> template<typename U>
void wsp::MatrixBase<T>::CopyIntoAnyMatrixBase(wsp::MatrixBase<U> &o_dst, u32 num_of_channels_dst) const
{
    u32 i, c, spp, nc = num_of_channels_dst;

    if(num_of_channels_dst==0){ nc = num_of_channels_; }

    o_dst.SetSize(column_size_, row_size_, nc);
    wsp::MatrixBase<T> normImg(*this);

    for(i=0; i<length_; i++){
        for(c=0; c<nc; c++){
            if(c<num_of_channels_){
                o_dst[i*nc+c] = static_cast<U>(data_[i*num_of_channels_+c]);
                continue;
            }
            o_dst[i*nc+c] = static_cast<U>(data_[i*num_of_channels_+num_of_channels_-1]);
        }
    }
}
template<class T> template<typename U> 
void wsp::MatrixBase<T>::CopyIntoAnyMatrixBaseAsU8(wsp::MatrixBase<U> &o_dst) const
{
    u32 i, c, spp;

    spp = num_of_channels_*sizeof(T);
    o_dst.SetSize(column_size_, row_size_, spp);
    uchar *data = (uchar*)data_;
    for(i=0; i<length_; i++){
        for(c=0; c<spp; c++){
            o_dst[i*num_of_channels_+c] = (U)data_[i*spp+c];
        }
    }
}
template<class T> template<typename U> 
inline bool wsp::MatrixBase<T>::IsTheSameSize(const wsp::MatrixBase<U> &in_img) const
{
    return (column_size_==in_img.column_size() && row_size_==in_img.row_size() && num_of_channels_==in_img.num_of_channels());
}
template<class T>
int wsp::MatrixBase<T>::GetNumOfIndex() const{
    wsp::Vector<int> indexList;
    wsp::MatrixBase<T>::GetIndexList(indexList);
    return (int)indexList.length();
}

template<class T>
wsp::State wsp::MatrixBase<T>::GetIndexList(wsp::Vector<T> &o_indexList) const{
    int index, i, c;
    o_indexList.SetLength(0);
    for(i=0; i<length_; i++){
        index=0;
        for(c=0; c<num_of_channels_; c++){
            index += data_[i]<<(c*8);
        }
        if(o_indexList.HasElem(index)==false){
            o_indexList.Append(index);
        }
    }
    return WSP_STATE_SUCCESS;    
}


// end Accessors: Getter ----------------------------------------------------------------------

// Accessors: Setter --------------------------------------------------------------------------
template<class T> template<typename U>
inline wsp::State wsp::MatrixBase<T>::CopyFrom(const wsp::MatrixBase<U>& img, u32 dst_num_of_channels){
    return CopyFrom<U>(img.data(), img.column_size(), img.row_size(), img.num_of_channels(), dst_num_of_channels);
}
template<class T> template<typename U>
wsp::State wsp::MatrixBase<T>::CopyFrom(const U* data, u32 column_size, u32 row_size, u32 src_num_of_channels, u32 dst_num_of_channels){
    if(has_own_data_==false)
    { WSP_COMMON_ERROR_LOG("You can not reallocate image data because current data is external data\n"); return WSP_STATE_FAILURE; }

    if((data==NULL) || row_size*column_size==0){
        return WSP_STATE_SUCCESS;
    }
    u32 i, c, nc = src_num_of_channels;
    if(dst_num_of_channels!=0){
        nc = dst_num_of_channels;
    }
    SetSize(column_size, row_size, nc);

    //memcpy(data_, data, size_*sizeof(T));
    for(i=0; i<length_; i++){
        for(c=0; c<num_of_channels_; c++){
            if(c<src_num_of_channels){
                data_[i*num_of_channels_+c] = static_cast<T>(data[i*src_num_of_channels+c]);
                continue;
            }
            data_[i*num_of_channels_+c] = static_cast<T>(data[i*src_num_of_channels+src_num_of_channels-1]);
        }
    }

    return WSP_STATE_SUCCESS;
}
template<class T>
inline void wsp::MatrixBase<T>::SetDataFrom(T *data, u32 column_size, u32 row_size, u32 num_of_channels){
    SetDataFrom(data, NULL, NULL, column_size, row_size, num_of_channels);
}
template<class T>
inline void wsp::MatrixBase<T>::SetDataFrom(T *data, u32 column_size, u32 row_size, u32 num_of_channels){
    Clear();
    data_=data;
    column_size_ = column_size;
    row_size_ = row_size;
    length_ = column_size_*row_size_;
    num_of_channels_ = num_of_channels;
    has_own_data_=false;
}
template<class T>
inline void wsp::MatrixBase<T>::SetDataInto(T *o_data, u32 *o_column_size, u32 *o_row_size, u32 *o_num_of_channels){
    SetDataInto(data, NULL, NULL, column_size, row_size, num_of_channels);
}
template<class T>
inline void wsp::MatrixBase<T>::SetDataInto(T *o_data, u32 *o_column_size, u32 *o_row_size, u32 *o_num_of_channels){
    o_data = data_;
    data_=NULL;

    if(o_column_size!=NULL){ *o_column_size=column_size_; }
    if(o_row_size!=NULL){ *o_row_size=row_size_; }
    if(o_num_of_channels!=NULL){ *o_num_of_channels=num_of_channels_; }
    column_size_=0; row_size_=0; num_of_channels_=0; length_=0;
    has_own_data_=true;
}

template<class T> inline void wsp::MatrixBase<T>::SetElem(const T& element, u32 x, u32 y, u32 channel){
    u32 index = (y*column_size_+x)*num_of_channels_+channel;
    wsp::MatrixBase<T>::SetElem(element, index);
}
template<class T> inline void wsp::MatrixBase<T>::SetElem(const T& element, u32 index){
    if(data_==NULL || column_size_*row_size_==0){
        return;
    }
    if(index<column_size_*row_size_*num_of_channels_){
        data_[index]=element;
    }
}

template<class T> inline void wsp::MatrixBase<T>::AddElem(const T& element, u32 x, u32 y, u32 channel){
    if(data_==NULL || column_size_*row_size_*num_of_channels_==0){
        return;
    }
    u32 index = (y*column_size_+x)*num_of_channels_+channel;
    if(index<length_*num_of_channels_){
        data_[index]+=element;
    }
}



template<class T> wsp::State wsp::MatrixBase<T>::SetSize(u32 column_size, u32 row_size, u32 nChannels){
    if(column_size_==column_size && row_size_==row_size && num_of_channels_==nChannels){ return WSP_STATE_SUCCESS; }
    if(has_own_data_==false )
    { WSP_COMMON_ERROR_LOG("You can not reallocate image data because current data is external data\n"); return WSP_STATE_FAILURE; }
    if(column_size==0||row_size==0){ Clear(); return WSP_STATE_SUCCESS; }
    wsp::State state = SetLength(column_size_*row_size_, nChannels);
    if(state!=WSP_STATE_SUCCESS){ WSP_COMMON_ERROR_LOG("Memory allocation has failed\n"); return state; }
    column_size_ = column_size;
    row_size_ = row_size;

    return WSP_STATE_SUCCESS;
}
template<class T> inline wsp::State wsp::MatrixBase<T>::Clear(){
    if(has_own_data_==false)
    { WSP_COMMON_ERROR_LOG("You can not reallocate image data because current data is external data\n"); return WSP_STATE_FAILURE; }
    ::wsp::Vector<T>::Clear();
    column_size_ = 0; row_size_ = 0; length_ = 0; size_=0;
    return WSP_STATE_SUCCESS;
}
template<class T> void wsp::MatrixBase<T>::Invert(){
    //if(sizeof(T)>4){ return; }
    T pix, dmax = this->GetMax();
    u32 i, c;
    u32 len=length_*num_of_channels_;
    switch(sizeof(T)){
    case 8:
        //! if image is double image
        for(i=0; i<len; i++){
            data_[i]*=-1;
        }
        //cout<<dmax<<endl;
        break;
    default:
        for(i=0; i<len; i++){
            pix = dmax - data_[i];
            data_[i] = pix;
        }
    }
}


template<class T> void wsp::MatrixBase<T>::Transpose(){
    //if(m_row==0 || m_column==0){ return; }
    wsp::MatrixBase<T> tmp_img(*this);
    u32 x, y, c, i_src, i_dst;

    SetSize(row_size_, column_size_, num_of_channels_);
    for(y=0; y<row_size_; y++){
        for(x=0; x<column_size_; x++){
            i_src=x*row_size_+y;
            i_dst=y*column_size_+x;
            for(c=0; c<num_of_channels_; c++){
                data_[i_dst*num_of_channels_+c]=tmp_img[i_src*num_of_channels_+c];
            }
            //SetElem(mat_tmp.GetElem(itr_c, itr_r), itr_r, itr_c); 
        }
    }
}

// end Accessors ------------------------------------------------------------------------------

//! IO -----------------------------------------------------------------------------------------
#ifdef USE_DEVIL


template<class T> wsp::State wsp::MatrixBase<T>::LoadMatrixBase(const char *filename, u32 nChannels){
    if(has_own_data_==false)
    { WSP_COMMON_ERROR_LOG("You can not reallocate image data because current data is external data\n"); return WSP_STATE_FAILURE; }
    u32 i, c;
    u32 tmp_w, tmp_h, tmp_nc;
    wsp::State stat;
    uchar *tmp_data=NULL;

    stat=wsp::img::LoadMatrixBaseAsU8(&tmp_data, NULL, tmp_w, tmp_h, tmp_nc, filename);
    if(stat!=WSP_STATE_SUCCESS){
        WSP_COMMON_ERROR_LOG("Loading image failed\n");
        delete[] tmp_data; delete[] alpha_data;
        Clear();
        return stat;
    }
    if(nChannels!=0){
        SetSize(tmp_w, tmp_h, nChannels);
    }else{
        SetSize(tmp_w, tmp_h, tmp_nc);
    }

    //! Copying image data
    for(i=0; i<length_; i++){
        for(c=0; c<num_of_channels_; c++){
            if(c<tmp_nc){
                data_[i*num_of_channels_+c] = static_cast<T>(tmp_data[i*tmp_nc+c]);
                //data[i*spp+j]=(T)tmp_data[i*nChannel+j];
                continue;
            }
            data_[i*num_of_channels_+c] = (T)tmp_data[i*tmp_nc+tmp_nc-1];
            //data[i*spp+j]=(T)tmp_data[i*nChannel+nChannel-1];
        }
    }
    //data_ = (T*)data;

    printf("column_size=%d, row_size=%d, nChannels=%d\n", column_size_, row_size_, num_of_channels_);


    delete[] tmp_data;
    return WSP_STATE_SUCCESS;
}
template<class T> wsp::State wsp::MatrixBase<T>::SaveMatrixBase(const char *filename) const{
    wsp::State state;
    //uchar *data=(uchar*)data_;
    //u32 spp = sizeof(T) * num_of_channels_; //! sample per pixel
    u32 i, c;
    //if(redState!=WSP_PIXEL_STATE_NORMAL){
    T range=this->GetMax()-this->GetMin();
    wsp::MatrixBase<T> tmp_img(*this);
    if(range!=(T)0 && (range>=(T)255 || range<(T)1) ){
        tmp_img.Normalize(0, 255);
    }
    uchar *tmp_data = WSP_NEW uchar[3*length_];
    //! fill up occluded pixel in red and save as RGB image
    for(i=0; i<length_; i++){
        for(c=0; c<3; c++){
            if(c<num_of_channels_){
                //tmp_data[i*3+j] = data[i*spp+j];
                tmp_data[i*3+c] = static_cast<uchar>(tmp_img[i*num_of_channels_+c]);
                continue;
            }
            //tmp_data[i*3+j] =data[i*spp+spp-1];
            tmp_data[i*3+c] = static_cast<uchar>(tmp_img[i*num_of_channels_+num_of_channels_-1]);
        }
    }
    state = wsp::img::SaveMatrixBase24(tmp_data, NULL, column_size_, row_size_, filename);
    delete[] tmp_data;

    if(state!=WSP_STATE_SUCCESS){ WSP_COMMON_ERROR_LOG("Saving %s failed\n", filename); return state; }
    return WSP_STATE_SUCCESS;
}





#elif defined USE_OPENCV
template<class T> wsp::State wsp::MatrixBase<T>::LoadMatrixBase(const char *filename, u32 nChannels){
    u32 x, y, index, column_size, row_size;
    IplMatrixBase *img;
    img = cvLoadMatrixBase(filename, CV_LOAD_IMAGE_DATA_TYPE_COLOR);
    if(img == NULL){ return WSP_STATE_FAILURE; }
    column_size = (u32)img->column_size;
    row_size = (u32)img->row_size;
    SetSize(column_size, row_size);
    uchar *ptr=(uchar*)data_;
    for(y=0; y<row_size; y++){
        for(x=0; x<column_size; x++){
            index = y*column_size + x; 
            for(s=0; s<spp; s++){
                ptr[index*spp+s] = img->imageData[index*3+s];
            }
        }
    }
    cvReleaseMatrixBase(&img);
    return WSP_STATE_SUCCESS;
}
template<class T> wsp::State wsp::MatrixBase<T>::SaveMatrixBase(const char *filename) const{
    u32 x, y, s, index;
    int state;
    IplMatrixBase *img = cvCreateMatrixBase(cvSize((int)column_size_, (int)row_size_), IPL_DEPTH_8U, 3);
    uchar *ptr=(uchar*)data_;
    u32 spp=sizeof(T);
    for(y=0; y<row_size_; y++){
        for(x=0; x<column_size_; x++){
            index=y*column_size_ + x;
            for(s=0; s<spp; s++){
                img->imageData[index*3+s] = ptr[index*spp+s];
            }
        }
    }
    state=cvSaveMatrixBase(filename, img);
    if(state==0){ return WSP_STATE_FAILURE; }
    cvReleaseMatrixBase(&img);
    return WSP_STATE_SUCCESS;
}

#else
template<class T> wsp::State wsp::MatrixBase<T>::LoadMatrixBase(const char *filename, u32 nChannels){
    if(sizeof(T)!=1 && sizeof(T)!=3){
        WSP_COMMON_ERROR_LOG("Number of channels %d is invalid for image format\n", sizeof(T));
        return WSP_STATE_FAILURE;
    }
    u32 i, j, spp, nChannel;
    wsp::State stat;
    uchar *tmp_data=NULL, *data=NULL;
    stat=wsp::img::LoadPNM(tmp_data, column_size_, row_size_, filename);
    if(stat!=WSP_STATE_SUCCESS){
        WSP_COMMON_ERROR_LOG("Loading image failed\n");
        column_size_=0; row_size_=0; length_=0;
        data_=NULL; 
        return stat;
    }
    length_ = column_size_*row_size_;
    spp=sizeof(T);
    //data = WSP_NEW uchar[length_*spp];
    wsp::img::ReallocMatrixBaseData(&data, alpha_data_, state_data_, length_, spp);
    for(i=0; i<length_; i++){
        for(j=0; j<spp; j++){
            if(j<nChannel){
                data[i*spp+j]=tmp_data[i*nChannel+j];
                continue;
            }
            data[i*spp+j]=tmp_data[i*nChannel+nChannel-1];
        }
    }
    data_ = (T*)data;


    delete[] tmp_data;
    return WSP_STATE_SUCCESS;
}
template<class T> wsp::State wsp::MatrixBase<T>::SaveMatrixBase(const char *filename) const{
    if(sizeof(T)!=1 && sizeof(T)!=3){
        WSP_COMMON_ERROR_LOG("Number of channels %d is invalid for image format\n", sizeof(T));
        return WSP_STATE_FAILURE;
    }
    wsp::State state;
    uchar *data=(uchar*)data_;
    int spp=sizeof(T);
    if(redState!=WSP_PIXEL_STATE_NORMAL){
        uchar *tmp_data = WSP_NEW uchar[3*length_];
        //! fill up occluded pixel in red and save as RGB image
        for(int i=0; i<length_; i++){
            for(int j=0; j<3; j++){
                if(j<spp){
                    tmp_data[i*3+j] = data[i*spp+j];
                    continue;
                }
                tmp_data[i*3+j] =data[i*spp+spp-1];
            }
        }
        state = wsp::img::SaveU24AsPPM(tmp_data, column_size_, row_size_, filename);
        delete[] tmp_data;
    }else{
        switch(sizeof(T)){
        case 1:
            state=wsp::img::SaveU8AsPGM(data, column_size_, row_size_, filename);
            break;
        case 3:
            state=wsp::img::SaveU24AsPPM(data, column_size_, row_size_, filename);
            break;
        default:
            WSP_COMMON_ERROR_LOG("Number of channels %d is invalid for image format\n", sizeof(T));
            return WSP_STATE_FAILURE;
        }
    }
    if(state!=WSP_STATE_SUCCESS){ WSP_COMMON_ERROR_LOG("Saving %s failed\n", filename); return state; }
    return WSP_STATE_SUCCESS;
}

#endif

#ifdef USE_LIBTIFF
template<class T> wsp::State wsp::MatrixBase<T>::LoadTiff(const char *filename){
    u32 column_size, row_size;
    u32 srcIndex, c;
    wsp::img::ImgPtr8U tmp_imgRgba;
    uchar *tmp_imgRgb;
    tmp_imgRgba = wsp::img::LoadTiff32(column_size, row_size, filename);
    if(tmp_imgRgba==NULL){
        return WSP_STATE_FAILURE;
    }
    //wsp::img::SaveAsTiff32(tmp_imgRgba, column_size, row_size, "debug_loadtest.tif");
    SetSize(column_size, row_size, 3);
    for(u32 i=0; i<column_size*row_size; i++){
        srcIndex = i*4;
        for(c=0; c<num_of_channels_; c++){
            if(c<3){
                data_[i*num_of_channels_+c] = tmp_imgRgba[srcIndex+c];
                continue;
            }
            data_[i*num_of_channels_+c] = tmp_imgRgba[srcIndex+2];
        }
    }
    wsp::img::ReleaseImg(tmp_imgRgba);
    return WSP_STATE_SUCCESS;
}
template<class T> wsp::State wsp::MatrixBase<T>::SaveAsTiff(const char *filename) const{
    wsp::State state;
    u32 spp = num_of_channels_;
    T range=this->GetMax()-this->GetMin();
    wsp::MatrixBase<T> tmp_img(*this);
    if(range>=(T)255 || range<(T)1){
        tmp_img.Normalize(0, 255);
    }
    uchar *tmp_data = WSP_NEW uchar[3*length_];
    //! fill up occluded pixel in red and save as RGB image
    for(int i=0; i<length_; i++){
        for(int j=0; j<3; j++){
            if(j<spp){
                //tmp_data[i*3+j] = data[i*spp+j];
                tmp_data[i*3+j] = static_cast<uchar>(tmp_img[i*spp+j]);
                continue;
            }
            //tmp_data[i*3+j] =data[i*spp+spp-1];
            tmp_data[i*3+j] = static_cast<uchar>(tmp_img[i*spp+spp-1]);
        }
    }
    state = wsp::img::SaveAsTiff24(tmp_data, NULL, column_size_, row_size_, filename);
    delete[] tmp_data;

    if(state!=WSP_STATE_SUCCESS){ WSP_COMMON_ERROR_LOG("Saving %s failed\n", filename); return state; }
    return WSP_STATE_SUCCESS;
}

#endif

#ifdef USE_QT
template<typename T> inline wsp::State wsp::MatrixBase<T>::SaveMatrixBaseDialog(const char *title) const
{
    QString selectedFilter;
    QString qfilename = QFileDialog::getSaveFileName(NULL, QString(title),
                            QDir::currentPath(), wsp::formatFilterImg, &selectedFilter);
    char currentDir[MAX_LENGTH_OF_PATH];
    wsp::GetDir(currentDir, qfilename.toLatin1().data());
    if(QDir::setCurrent(QString(currentDir))==false){ WSP_COMMON_ERROR_LOG("setCurrent failed\n"); }

    return SaveMatrixBase(qfilename.toLatin1().data());
}


template<typename T> inline wsp::State wsp::MatrixBase<T>::LoadMatrixBaseDialog(u32 nChannels, const char *title)
{
    QString qfilename = QFileDialog::getOpenFileName(NULL, QString(title), QDir::currentPath(), wsp::formatFilterImg);
    char currentDir[MAX_LENGTH_OF_PATH];
    wsp::GetDir(currentDir, qfilename.toLatin1().data());
    if(QDir::setCurrent(QString(currentDir))==false){ WSP_COMMON_ERROR_LOG("setCurrent failed\n"); }
    return LoadMatrixBase(qfilename.toLatin1().data(), nChannels);
}

#endif

template<class T> wsp::State wsp::MatrixBase<T>::SaveRawData(const char *in_fileNameFormat,...){
    wsp::State stat;
    char filename[MAX_LENGTH_OF_NAME];
    va_list ap;
    va_start(ap, in_fileNameFormat);
    vsprintf(filename, in_fileNameFormat, ap);
    va_end(ap);
    return wsp::img::SaveRawData<T>(data_, column_size_, row_size_, num_of_channels_, filename);
}
template<class T> wsp::State wsp::MatrixBase<T>::LoadRawData(const char *fileName){
    wsp::State stat;
    stat=wsp::img::LoadRawData<T>(&data_, column_size_, row_size_, num_of_channels_, fileName);
    length_ = column_size_*row_size_;
    return stat;
}



//! GUI ---------------------------------------------------------------------------------------
#ifdef USE_OPENCV
template<class T> void wsp::MatrixBase<T>::ShowMatrixBase(){
    //cvNamedWindow ("CaptureL", CV_WINDOW_AUTOSIZE);
    //cvNamedWindow ("CaptureR", CV_WINDOW_AUTOSIZE);

    //frameL = cvQueryFrame (captureL);
    //frameR = cvQueryFrame (captureR);
    //imgL.SetSize(frameL->column_size, frameL->row_size, 1);

    //// (3)カメラから画像をキャプチャする
    //while (1) {
    //    wsp::img::cvIplMatrixBase2RgbImg(imgL, frameL, NULL);
    //    wsp::img::cvIplMatrixBase2RgbImg(imgR, frameR, NULL);
    //    wsp::img::StereoBM(depth, imgL, imgR, -20, 20, 3, 9, false);
    //    wsp::img::Median<WspRgb>(imgL, 3);
    //    wsp::img::cvRgbMatrixBase2IplMatrixBase(frameL, NULL, imgL);
    //    wsp::img::cvRgbMatrixBase2IplMatrixBase(frameR, NULL, imgR);

    //    cvShowMatrixBase ("Capture", frameL);
    //    cvShowMatrixBase ("Capture", frameR);
    //    c = cvWaitKey (0);
    //    if (c == '\x1b')
    //        break;
    //    frameL = cvQueryFrame (captureL);
    //    frameR = cvQueryFrame (captureR);
    //    if (frameL==NULL || frameR==NULL){ break; }
    //}

    //cvReleaseCapture (&captureL);
    //cvDestroyWindow ("Capture");
}
#endif

// Operators ---------------------------------------------------------------------------------
template<class T> wsp::MatrixBase<T>& wsp::MatrixBase<T>::operator=(const wsp::MatrixBase<T>& other){
    if(this != &other){
        CopyFrom(other);
    }
    return *this;
}
template<class T> bool wsp::MatrixBase<T>::operator==(const wsp::MatrixBase<T> &other) const{
    if(other.data()==NULL && data_==NULL && column_size_*row_size_==other.column_size()*other.row_size()){
        return true;
    }
    if(other.column_size()*other.row_size()!=column_size_*row_size_ || data_==NULL || other.data()==NULL){
        return false;
    }
    if(wsp::Vector<T>::operator!=(other)){
        return false;
    }
    return true;
}
template<class T> bool wsp::MatrixBase<T>::operator!=(const wsp::MatrixBase<T> &other) const{
    return !(*this==other);
}

//! Qt -----------------------------------------------------------
#ifdef USE_QT
template <class T>
wsp::State wsp::MatrixBase<T>::CopyInto(QMatrixBase &o_qimg, bool withOcclusion) const
{
    int x, y; //! iterators
    int row_size, column_size;
    T minValue, value;

    column_size = (int)column_size_;
    row_size = (int)row_size_;

    o_qimg.~QMatrixBase();
    o_qimg = QMatrixBase(column_size, row_size, QMatrixBase::Format_ARGB32);

    minValue = GetMin();

    for(y=0; y<row_size; y++){
        for(x=0; x<column_size; x++)
        {
            if(withOcclusion!=false && GetState((u32)x, (u32)y)==WSP_PIXEL_STATE_OCCLUDED){
                o_qimg.setPixel(x, y, qRgb(255, 0, 0));
                continue;
            }

            value = (T)(GetElem((u32)x, (u32)y) - minValue);
            if(value<=255){
                o_qimg.setPixel(x, y, qRgb(value, value, value));
            }
        }
    }

    return WSP_STATE_SUCCESS;
}
template <class T>
wsp::State wsp::MatrixBase<T>::CopyFrom(const QMatrixBase &in_src)
{
    int x, y; //! iterators
    int column_size, row_size;
    QColor pix;

    column_size = in_src.column_size();
    row_size = in_src.row_size();

    SetSize((u32)column_size, (u32)row_size);

    for(y=0; y<row_size; y++){
        for(x=0; x<column_size; x++){
            pix.setRgb(in_src.pixel(x,y));
            SetElem((T)pix.red(), (u32)x, (u32)y);
        }
    }
    return WSP_STATE_SUCCESS;
}



#endif

#endif
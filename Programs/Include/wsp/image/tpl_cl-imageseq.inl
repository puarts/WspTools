/**
 * @file tpl_cl-imageseq.inl
 * 
 */

#ifndef WSP_IMAGE_TEMPLATE_CLASS_IMAGESEQ_INL_
#define WSP_IMAGE_TEMPLATE_CLASS_IMAGESEQ_INL_

#include <wsp/common/fn-debug.h>
#include "image_result.h"
#include "tpl_cl-imageseq.h"

/* ------------------------------------------------
    Constructor, Destructor
   ------------------------------------------------*/
#define INIT_WSP_IMAGE_SEQ \
    seq_info_src_(NULL),\
    data_(NULL),\
    has_own_data_(true),\
    width_(0), height_(0), num_channels_(0)

template<class T> 
void wsp::ImageSeq<T>::Init(){
    if(data_==NULL){ data_ = WSP_NEW wsp::Vector<wsp::Image<T>>(); }
    if(seq_info_src_==NULL){ seq_info_src_ = WSP_NEW wsp::SeqInfo(); }
}

template<class T> 
wsp::ImageSeq<T>::ImageSeq()
    :INIT_WSP_IMAGE_SEQ
{ 
    Init(); 
}

template<class T> 
wsp::ImageSeq<T>::ImageSeq( u32 width, u32 height, u32 num_channels, u32 num_frames )
    : INIT_WSP_IMAGE_SEQ
{
    Init(); 
    width_ = width;
    height_ = height;
    num_channels_ = num_channels;
    data_->SetLength( num_frames );
}

template<class T> 
wsp::ImageSeq<T>::ImageSeq(const char *filename)
    :INIT_WSP_IMAGE_SEQ
{
    Init();
    LoadImageSeq(filename);
}

template<class T> 
wsp::ImageSeq<T>::ImageSeq(const wsp::ImageSeq<T> &in_imgseq)
    :INIT_WSP_IMAGE_SEQ
{
    Init();
    Clear();
    this->SetSeqSize(in_imgseq.SeqSize());
    for(int i=0; i<this->SeqSize(); i++){
        (*data_)[i] = in_imgseq[i];
    }
}

template<class T> wsp::ImageSeq<T>::~ImageSeq(){
    if(has_own_data_){
        delete data_;
        delete seq_info_src_;
    }
}

/* ------------------------------------------------
    Getter 
   ------------------------------------------------*/
template<class T> inline int  wsp::ImageSeq<T>::width()               const{ return SeqSize()==0? 0: (*data_)[0].width(); }
template<class T> inline int  wsp::ImageSeq<T>::height()              const{ return SeqSize()==0? 0: (*data_)[0].height(); }
template<class T> inline int  wsp::ImageSeq<T>::num_of_channels()     const{ return SeqSize()==0? 0: (*data_)[0].num_of_channels(); }
template<class T> inline bool wsp::ImageSeq<T>::has_own_data()        const{ return has_own_data_; }
template<class T> inline int  wsp::ImageSeq<T>::SeqSize()             const{ return data_==NULL? 0 : data_->length(); }
template<class T> inline wsp::SeqInfo                      wsp::ImageSeq<T>::SeqInfoSrc() const{ return seq_info_src_==NULL? wsp::SeqInfo(): *seq_info_src_; }
template<class T> inline wsp::Vector<wsp::Image<T>>*       wsp::ImageSeq<T>::data(){ return data_; }
template<class T> inline const wsp::Vector<wsp::Image<T>>* wsp::ImageSeq<T>::data() const{ return data_; }
//template<class T> inline wsp::SeqInfo GetSeqInfoDst() const{ return m_seqInfoDst; }
template<class T> inline wsp::Image<T>* wsp::ImageSeq<T>::GetImageData(int frame){ return (data_->data()+frame); }

/* ------------------------------------------------
    Setter 
   ------------------------------------------------*/

template<class T>
wsp::Result wsp::ImageSeq<T>::Resize( u32 width, u32 height )
{
    width_ = width;
    height_ = height;
    wsp::Image<T>* img_ptr = data_->data();
    wsp::Image<T>* end_ptr = img_ptr + data_->length();
    for( ; img_ptr!=end_ptr; ++img_ptr )
    {
        img_ptr->ScaleImage( width, height );
    }

    return wsp::ResultSuccess();
}

template<class T>
void wsp::ImageSeq<T>::ResizeCanvas( u32 width, u32 height )
{
    width_ = width;
    height_ = height;
    wsp::Image<T>* img_ptr = data_->data();
    wsp::Image<T>* end_ptr = img_ptr + data_->length();
    for( ; img_ptr!=end_ptr; ++img_ptr )
    {
        img_ptr->ResizeCanvas( (int)width, (int)height );
    }
}

template<class T>
inline wsp::State wsp::ImageSeq<T>::Clear()
{ 
    if(has_own_data_==false)
    { 
        WSP_COMMON_ERROR_LOG("You can not reallocate image data because current data is external data\n"); 
        return WSP_STATE_FAILURE; 
    }
    data_->Clear();
    memset( seq_info_src_, 0, sizeof(wsp::SeqInfo ) );
    //delete data_;
    //delete seq_info_src_;
    //data_=NULL;
    //seq_info_src_=NULL;
    return WSP_STATE_SUCCESS;
}

template<class _Type>
template<typename _InputType>
wsp::Result wsp::ImageSeq<_Type>::Append( const wsp::Image<_InputType> &image )
{
    if(has_own_data_==false)
    { 
        return wsp::img::ResultIsExternalData(); 
    }

    wsp::Image<_Type> resized_image( image );
    resized_image.ResizeCanvas( width_, height_ );
    resized_image.ExtendsNumOfChannels( num_channels_ );

    data_->Append( resized_image );
    return wsp::ResultSuccess();
}

template<class T>
template<typename U> 
inline wsp::State wsp::ImageSeq<T>::CopyFrom(const wsp::ImageSeq<U> &img_seq, int dst_num_of_channels){
    if(has_own_data_==false)
    { 
        WSP_COMMON_ERROR_LOG("You can not reallocate image data because current data is external data\n"); 
        return WSP_STATE_FAILURE; 
    }
    SetSeqSize(img_seq.SeqSize());
    int len = SeqSize();
    wsp::State state;
    for(int i=0; i<len; i++)
    {
        wsp::Image<T,50>* image = &(*data_)[i];
        state = image->template CopyFrom<U>(img_seq[i], dst_num_of_channels);
        if(state!=WSP_STATE_SUCCESS){
            WSP_COMMON_ERROR_LOG("Copying is failed\n"); 
            return state;
        }
    }
    return WSP_STATE_SUCCESS;
}

template<class T>
inline wsp::State wsp::ImageSeq<T>::SetSeqSize(int num_of_seq){ 
    if(data_==NULL)
    { 
        data_ = WSP_NEW wsp::Vector<wsp::Image<T>>();
    }
    seq_info_src_->end_frame = seq_info_src_->start_frame + num_of_seq - 1;
    wsp::State state = data_->SetLength(num_of_seq); 
    ResizeCanvas( width_, height_ );
    SetNumOfChannels( num_of_channels() );

    return state;
}
template<class T>
inline wsp::State wsp::ImageSeq<T>::SetNumOfChannels( int num_of_channels )
{
    if(data_==NULL){ return WSP_STATE_FAILURE; }

    {
        wsp::State state;
        wsp::Image<T> *ptr = data_->data();
        wsp::Image<T> *end_ptr = ptr + SeqSize();
        for( ; ptr!=end_ptr; ++ptr)
        {
            state = ptr->SetNumOfChannels( num_of_channels );
            if(state!=WSP_STATE_SUCCESS){ return state; }
        }
        num_channels_ = num_of_channels;
    }
    return WSP_STATE_SUCCESS;
}
template<class T>
inline wsp::State wsp::ImageSeq<T>::ExtendsNumOfChannels( int extended_num_of_channels )
{
    if(data_==NULL){ return WSP_STATE_FAILURE; }

    {
        wsp::State state;
        wsp::Image<T> *ptr = data_->data();
        wsp::Image<T> *end_ptr = ptr + SeqSize();
        for( ; ptr!=end_ptr; ++ptr)
        {
            state = ptr->ExtendsNumOfChannels( extended_num_of_channels );
            if(state!=WSP_STATE_SUCCESS){ return state; }
        }
    }
    return WSP_STATE_SUCCESS;
}
template<class T>
inline void wsp::ImageSeq<T>::SetSeqInfo(wsp::SeqInfo info){
    if(seq_info_src_==NULL){ seq_info_src_ = new wsp::SeqInfo(); }
    *seq_info_src_ = wsp::SeqInfo(info); 
}
template<class T>
inline void wsp::ImageSeq<T>::SetDataFrom(wsp::Vector<wsp::Image<T>> *seq_data){
    delete data_; 
    data_=seq_data; 
    seq_data=NULL;
    has_own_data_=false;
}
template<class T>
inline void wsp::ImageSeq<T>::SetDataInto(wsp::Vector<wsp::Image<T>> *seq_data){
    seq_data = data_; 
    data_=NULL; 
    has_own_data_=true;
}

/* ---------------------------------------------------
    I/O 
   ---------------------------------------------------*/

template<class T>
wsp::State wsp::ImageSeq<T>::LoadRawDataSeq(const char *filename, int end
#ifdef USE_QT
        , QProgressBar *progress_bar=NULL
#endif
        )
{
    if(has_own_data_==false)
    { WSP_COMMON_ERROR_LOG("You can not reallocate image data because current data is external data\n"); return WSP_STATE_FAILURE; }

    int endFrame;
    wsp::State wstat;
    char path[MAX_LENGTH_OF_PATH];

    Init();

    wsp::GetSeqInfo(*seq_info_src_, filename);
    if(end>0 && seq_info_src_->start_frame<=end && end<seq_info_src_->end_frame)
    { seq_info_src_->end_frame = end; }

    //cout<<seq_info_src_<<endl;
    //printf("s=%d, e=%d\n", seq_info_src_->start_frame, end<seq_info_src_->end_frame);

    int i, frame;
    int len = seq_info_src_->end_frame - seq_info_src_->start_frame + 1;
    //if(seq_info_src_->num_padding==0){ len=1; }
    data_->SetLength(len);
    #ifdef USE_QT
    if(progress_bar!=NULL){
        progress_bar->setRange(0, len); progress_bar->setValue(0);
    }
    #endif
    //#ifdef _OPENMP
    //#pragma omp parallel for private(i)
    //#endif
    for(i=0; i<len; i++){
        frame = seq_info_src_->start_frame + i;
        wsp::GetSeqPath(path, *seq_info_src_, frame);
        wstat = (*data_)[i].LoadRawData(path);
        if(wstat != WSP_STATE_SUCCESS){ WSP_COMMON_ERROR_LOG("Loading image failed\n"); return wstat; }
        #ifdef USE_QT
        if(progress_bar!=NULL){ progress_bar->setValue(i); }
        #endif
    }
    #ifdef USE_QT
    if(progress_bar!=NULL){ progress_bar->setValue(0); }
    #endif
    return WSP_STATE_SUCCESS;
}



template<class T>
wsp::State wsp::ImageSeq<T>::LoadImageSeq(const char *filename, int nChannels, int end
#ifdef USE_QT
        , QProgressBar *progress_bar=NULL
#endif
){
    if(has_own_data_==false)
    { 
        WSP_COMMON_ERROR_LOG("You can not reallocate image data because current data is external data\n"); 
        return WSP_STATE_FAILURE; 
    }

    Init();

    printf("extracting sequence info...\r");

    wsp::GetSeqInfo(*seq_info_src_, filename);
    if(end>0 && seq_info_src_->start_frame<=end && end<seq_info_src_->end_frame)
    { seq_info_src_->end_frame = end; }

    printf("successfully finished extracting sequence info...\n");

    //cout<<seq_info_src_<<endl;
    //printf("s=%d, e=%d\n", seq_info_src_->start_frame, end<seq_info_src_->end_frame);
    int len = seq_info_src_->end_frame - seq_info_src_->start_frame + 1;
    //if(seq_info_src_->num_padding==0){ len=1; }
    data_->SetLength(len);
    printf("successfully finished setting sequence size...\n");
    #ifdef USE_QT
    if(progress_bar!=NULL){
        progress_bar->setRange(0, len); progress_bar->setValue(0);
    }
    #endif
    //#ifdef _OPENMP
    //#pragma omp parallel
    //#endif
    {
        char path[MAX_LENGTH_OF_PATH];
        int i, frame;
        wsp::State wstat;
        //#ifdef _OPENMP
        //#pragma omp for
        //#endif
        for(i=0; i<len; i++){
            printf("loading frame %d..\r", i);

            frame = seq_info_src_->start_frame + i;
            wsp::GetSeqPath(path, *seq_info_src_, frame);
            //#ifdef _OPENMP
            //#pragma omp critical
            //#endif
            {
                wstat = (*data_)[i].LoadImageFile(path, nChannels);
                if (wstat != WSP_STATE_SUCCESS)
                {
                    return wstat;
                }
            }
            //if(wstat != WSP_STATE_SUCCESS){ WSP_COMMON_ERROR_LOG("Loading image failed\n"); return wstat; }
            #ifdef USE_QT
            if(progress_bar!=NULL){ progress_bar->setValue(i); }
            qApp->processEvents();
            #endif
        }
    }
    #ifdef USE_QT
    if(progress_bar!=NULL){ progress_bar->setValue(0); }
    #endif

    printf("successfully finished loading image sequence\n");

    return WSP_STATE_SUCCESS;
}


template<class T>
wsp::State wsp::ImageSeq<T>::LoadImageSeqAsAlpha(const char *filename, int end
#ifdef USE_QT
        , QProgressBar *progress_bar=NULL
#endif
        )
{
    if(has_own_data_==false)
    { WSP_COMMON_ERROR_LOG("You can not reallocate image data because current data is external data\n"); return WSP_STATE_FAILURE; }

    int endFrame;
    wsp::State wstat;
    char path[MAX_LENGTH_OF_PATH];

    Init();

    wsp::GetSeqInfo(*seq_info_src_, filename);
    if(end>0 && seq_info_src_->start_frame<=end && end<seq_info_src_->end_frame)
    { seq_info_src_->end_frame = end; }

    //cout<<seq_info_src_<<endl;
    //printf("s=%d, e=%d\n", seq_info_src_->start_frame, end<seq_info_src_->end_frame);

    //int old_len = length_;
    int i, frame;
    int len = seq_info_src_->end_frame - seq_info_src_->start_frame + 1;
    //if(seq_info_src_->num_padding==0){ len=1; }
    data_->SetLength(len);
    #ifdef USE_QT
    if(progress_bar!=NULL){
        progress_bar->setRange(0, len); progress_bar->setValue(0);
    }
    #endif
    //#ifdef _OPENMP
    //#pragma omp parallel for private(i)
    //#endif
    for(i=0; i<len; i++){
        frame = seq_info_src_->start_frame + i;
        wsp::GetSeqPath(path, *seq_info_src_, frame);
        wstat = (*data_)[i].LoadImageFileAsAlpha(path);
        //if(wstat != WSP_STATE_SUCCESS){ WSP_COMMON_ERROR_LOG("Loading image failed\n"); return wstat; }
        #ifdef USE_QT
        if(progress_bar!=NULL){ progress_bar->setValue(i); }
        #endif
    }
    #ifdef USE_QT
    if(progress_bar!=NULL){ progress_bar->setValue(0); }
    #endif
    return WSP_STATE_SUCCESS;
}


template<class T>
wsp::State wsp::ImageSeq<T>::LoadADRISeq(
    const char *filename, 
    const char *filename_alpha, 
    int nChannels, 
    int end, 
    bool normalize, 
    bool normalize_alpha
    #ifdef USE_QT
    , QProgressBar *progress_bar=NULL
    #endif
)
{
    if(has_own_data_==false)
    { 
        WSP_COMMON_ERROR_LOG("You can not reallocate image data because current data is external data\n"); 
        return WSP_STATE_FAILURE; 
    }

    int endFrame;
    wsp::State state;
    char *path=NULL, *path_alpha=NULL;

    Init();

    wsp::SeqInfo alpha_info;
    int len;
    if(filename!=NULL){
        wsp::GetSeqInfo(*seq_info_src_, filename);
        path = WSP_NEW char[MAX_LENGTH_OF_PATH];
        if(end>0 && seq_info_src_->start_frame<=end && end<seq_info_src_->end_frame)
        { seq_info_src_->end_frame = end; }
        len = seq_info_src_->end_frame - seq_info_src_->start_frame + 1;
    }
    if(filename_alpha!=NULL){
        wsp::GetSeqInfo(alpha_info, filename_alpha);
        path_alpha = WSP_NEW char[MAX_LENGTH_OF_PATH];
        if(end>0 && alpha_info.start_frame<=end && end<alpha_info.end_frame)
        { alpha_info.end_frame = end; }
        len = alpha_info.end_frame - alpha_info.start_frame + 1;
    }

    int i, frame;
    data_->SetLength(len);

    #ifdef USE_QT
    if(progress_bar!=NULL){
        progress_bar->setRange(0, len); progress_bar->setValue(0);
    }
    #endif
    //#ifdef _OPENMP
    //#pragma omp parallel for private(i)
    //#endif
    for(i=0; i<len; i++){
        frame = seq_info_src_->start_frame + i;
        if(filename!=NULL){ wsp::GetSeqPath(path, *seq_info_src_, frame); }
        if(filename_alpha!=NULL){ wsp::GetSeqPath(path_alpha, alpha_info, frame); }
        wsp::Image<int> tmp_img((*data_)[i]);
        state = tmp_img.LoadADRI(path, path_alpha, 3);
        if(state!=WSP_STATE_SUCCESS){ WSP_COMMON_ERROR_LOG("Loading ADRI failed\n"); return state; }
        if(normalize){ tmp_img.Normalize(0, 255);}
        if(normalize_alpha){ tmp_img.NormalizeAlpha(0, 255); }
        (*data_)[i].CopyFrom(tmp_img);
        #ifdef USE_QT
        if(progress_bar!=NULL){ progress_bar->setValue(i); }
        #endif
    }
    #ifdef USE_QT
    if(progress_bar!=NULL){ progress_bar->setValue(0); }
    #endif
    if(filename!=NULL){ delete[] path; }
    if(filename_alpha!=NULL){ delete[] path_alpha; }
    if(filename==NULL && filename_alpha!=NULL){
        *seq_info_src_ = alpha_info;
    }
    return WSP_STATE_SUCCESS;
}

template<class T>
wsp::State wsp::ImageSeq<T>::LoadSeq(const char *filename, int nChannels, int end, bool as_alpha
    #ifdef USE_QT
    , QProgressBar *progress_bar
    #endif
){
    char extension[MAX_LENGTH_OF_NAME];

    wsp::GetExtension(extension, filename);
    if(as_alpha){
        if(strcmp(extension, "adri")==0){
            LoadADRISeq(
                NULL, filename, nChannels, end, false, true
                #ifdef USE_QT
                , progress_bar
                #endif
            );
        }else{
            LoadImageSeqAsAlpha(
                filename, end
                #ifdef USE_QT
                , progress_bar
                #endif
            );
        }
    }else{
        if(strcmp(extension, "adri")==0){
            LoadADRISeq(
                filename, NULL, nChannels, end, false, false 
                #ifdef USE_QT
                , progress_bar
                #endif
            );
        }else if(strcmp(extension, "raw")==0 || strcmp(extension, "bin")==0){
            LoadRawDataSeq(
                filename, end
                #ifdef USE_QT
                , progress_bar
                #endif
            );
        }else{
            LoadImageSeq(
                filename, nChannels, end
                #ifdef USE_QT
                , progress_bar
                #endif
            );
        }
    }
    if(SeqSize()==0){ 
        WSP_COMMON_ERROR_LOG("No image sequence loaded\n"); wsp::StopTimer(); return WSP_STATE_FAILURE; 
    }

    return WSP_STATE_SUCCESS;
}

template<class T>
wsp::State wsp::ImageSeq<T>::SaveImageSeq(const char *filename, int start_index, int end_index) const
{
    WSP_COMMON_LOG("filename=%s, start_index=%d, end_index=%d", filename, start_index, end_index);
    if( data_==NULL || seq_info_src_==NULL )
    { 
        WSP_COMMON_ERROR_LOG("data is NULL\n"); 
        return WSP_STATE_FAILURE; 
    }
    if( SeqSize()==0 )
    {
        WSP_COMMON_WARNING_LOG("sequence size is zero, no images are saved.\n"); 
        return WSP_STATE_SUCCESS;
    }

    WSP_ASSERT( end_index < data_->length(), "Invalid argument" );

    char path[MAX_LENGTH_OF_PATH];
    wsp::SeqInfo seqInfo;
    s32 start_frame, len;

    wsp::GetSeqInfo( seqInfo, filename );

    if( seqInfo.num_padding==0 
        && data_->length() > 1 
        && (start_index>=0 && end_index>=0 && start_index!=end_index))
    {
        seqInfo.num_padding = 4;
        sprintf( seqInfo.name, "%s_", seqInfo.name );
    }

    if( start_index>=0 )
    {
        start_frame=start_index;
    }
    else
    {
        start_frame=seqInfo.start_frame;
    }
    //if( end>0 && seq_info_src_->start_frame<=end ){ seqInfo.end_frame = end; }
    //else{ seqInfo.end_frame = (int)data_->length()-1-seqInfo.start_frame; }
    if(end_index>=0)
    { 
        len = end_index+1; 
    }
    else
    {
        len = data_->length(); 
    }
    len -= start_frame;

    int i, index, frame;

    //for(int i=0, frame=seqInfo.start_frame; frame<=seqInfo.end_frame; frame++, i++){
    //#ifdef _OPENMP
    //#pragma omp parallel for private(i)
    //#endif
    for( i=0; i<len; ++i )
    {
        index = start_frame + i;
        frame = seqInfo.start_frame + i;
        wsp::GetSeqPath(path, seqInfo, frame);
        (*data_)[index].SaveImage(path);
    }
    return WSP_STATE_SUCCESS;
}

template<class T>
wsp::State wsp::ImageSeq<T>::SaveAlphaSeq(const char *filename, int start_index, int end_index) const{
    if(data_==NULL || seq_info_src_==NULL){ WSP_COMMON_ERROR_LOG("data is NULL\n"); return WSP_STATE_FAILURE; }
    char path[MAX_LENGTH_OF_PATH];
    wsp::SeqInfo seqInfo;
    wsp::GetSeqInfo(seqInfo, filename);
    
    int start_frame, len;

    if(seqInfo.num_padding==0){
        start_frame=0;
    }else{
        start_frame=seqInfo.start_frame;
    }
    //if( end>0 && seq_info_src_->start_frame<=end ){ seqInfo.end_frame = end; }
    //else{ seqInfo.end_frame = (int)data_->length()-1-seqInfo.start_frame; }
    if(end_index>0){ len = end_index+1; }
    else{ len = data_->length(); }

    int i, frame;

    //for(int i=0, frame=seqInfo.start_frame; frame<=seqInfo.end_frame; frame++, i++){
    //#ifdef _OPENMP
    //#pragma omp parallel for private(i)
    //#endif
    for(i=0; i<len; i++){
        frame = seq_info_src_->start_frame + i;
        wsp::GetSeqPath(path, seqInfo, frame);
        (*data_)[i].SaveAlpha(path);
    }
    return WSP_STATE_SUCCESS;
}

template<class T>
wsp::State wsp::ImageSeq<T>::SaveStateSeq(const char *filename, int start_index, int end_index) const{
    if(data_==NULL || seq_info_src_==NULL){ WSP_COMMON_ERROR_LOG("data is NULL\n"); return WSP_STATE_FAILURE; }
    char path[MAX_LENGTH_OF_PATH];
    wsp::SeqInfo seqInfo;
    int start_frame, len;
    
    wsp::GetSeqInfo(seqInfo, filename);

    if(seqInfo.num_padding==0){
        start_frame=0;
    }else{
        start_frame=seqInfo.start_frame;
    }
    //if( end>0 && seq_info_src_->start_frame<=end ){ seqInfo.end_frame = end; }
    //else{ seqInfo.end_frame = (int)data_->length()-1-seqInfo.start_frame; }
    if(end_index>0){ len = end_index+1; }
    else{ len = data_->length(); }

    int i, frame;

    //for(int i=0, frame=seqInfo.start_frame; frame<=seqInfo.end_frame; frame++, i++){
    //#ifdef _OPENMP
    //#pragma omp parallel for private(i)
    //#endif
    for(i=0; i<len; i++){
        frame = seq_info_src_->start_frame + i;
        wsp::GetSeqPath(path, seqInfo, frame);
        (*data_)[i].SaveState(path);
    }
    return WSP_STATE_SUCCESS;
}


template<class T>
wsp::State wsp::ImageSeq<T>::SaveRawDataSeq(const char *filename, int start_index, int end_index) const{
    if(data_==NULL || seq_info_src_==NULL){ WSP_COMMON_ERROR_LOG("data is NULL\n"); return WSP_STATE_FAILURE; }
    char path[MAX_LENGTH_OF_PATH];
    wsp::SeqInfo seqInfo;
    int start_frame, len;
    
    wsp::GetSeqInfo(seqInfo, filename);

    if(seqInfo.num_padding==0){
        start_frame=0;
    }else{
        start_frame=seqInfo.start_frame;
    }

    int i, frame;
    //if( end>0 && seq_info_src_->start_frame<=end ){ seqInfo.end_frame = end; }
    //else{ seqInfo.end_frame = (int)data_->length()-1-seqInfo.start_frame; }
    if(end_index>0){ len = end_index+1; }
    else{ len = data_->length(); }

    //for(int i=0, frame=seqInfo.start_frame; frame<=seqInfo.end_frame; frame++, i++){

    //#ifdef _OPENMP
    //#pragma omp parallel for private(i)
    //#endif
    for(i=0; i<len; i++){
        frame = seq_info_src_->start_frame + i;
        wsp::GetSeqPath(path, seqInfo, frame);
        (*data_)[i].SaveRawData(path);
    }
    return WSP_STATE_SUCCESS;
}



template<class T>
wsp::State wsp::ImageSeq<T>::SaveImageSeqAsADRI(const char *filename, const char *filename_alpha, int start_index, int end_index) const
{
    if(data_==NULL || seq_info_src_==NULL){ WSP_COMMON_ERROR_LOG("data is NULL\n"); return WSP_STATE_FAILURE; }
    int start_frame, len;
    char *path=NULL, *path_alpha=NULL;

    wsp::SeqInfo alpha_info;
    if(filename!=NULL){
        wsp::GetSeqInfo(*seq_info_src_, filename);
        path = WSP_NEW char[MAX_LENGTH_OF_PATH];
    }
    if(filename_alpha!=NULL){
        wsp::GetSeqInfo(alpha_info, filename_alpha);
        path_alpha = WSP_NEW char[MAX_LENGTH_OF_PATH];
    }

    if(seq_info_src_->num_padding==0){
        start_frame=0;
    }else{
        start_frame=seq_info_src_->start_frame;
    }

    int i, frame;
    //if( end>0 && seq_info_src_->start_frame<=end ){ seqInfo.end_frame = end; }
    //else{ seqInfo.end_frame = (int)data_->length()-1-seqInfo.start_frame; }
    if(end_index>0){ len = end_index+1; }
    else{ len = data_->length(); }

    //for(int i=0, frame=seqInfo.start_frame; frame<=seqInfo.end_frame; frame++, i++){

    //#ifdef _OPENMP
    //#pragma omp parallel for private(i)
    //#endif
    for(i=0; i<len; i++){
        frame = seq_info_src_->start_frame + i;
        wsp::GetSeqPath(path, *seq_info_src_, frame);
        wsp::GetSeqPath(path_alpha, alpha_info, frame);
        (*data_)[i].SaveAsADRI(path, path_alpha);
    }
    if(filename!=NULL){ delete[] path; }
    if(filename_alpha!=NULL){ delete[] path_alpha; }
    return WSP_STATE_SUCCESS;
}



//template<typename T>
//void OpenSeq(wsp::ImageSeq<T> &o_seq, const wsp::SeqInfo &in_seqInfo)
//{
//    char filename[MAX_LENGTH_OF_PATH];
//    o_seq.SetLength(0);
//    for(int i=in_seqInfo.start_frame; i<in_seqInfo.end_frame; i++){
//        wsp::Image<uchar> *img;
//        img = new wsp::Image<uchar>();
//        wsp::GetSeqPath(filename, *in_seqInfo, i);
//        img->LoadImageFile(filename);
//        o_seq.Append(img);
//    }
//}

#ifdef USE_QT
template<class T>
inline wsp::State wsp::ImageSeq<T>::LoadImageSeqDialog(int nChannels, int end, const char *title, QProgressBar *bar)
{
    QString qfilename = QFileDialog::getOpenFileName(NULL, QString(title), QDir::currentPath(), wsp::formatFilterImg);
    if(qfilename.isNull()){ WSP_COMMON_ERROR_LOG("Getting filename is failed\n"); return WSP_STATE_FAILURE; }
    char currentDir[MAX_LENGTH_OF_PATH];
    wsp::GetDir(currentDir, qfilename.toLatin1().data());
    if(QDir::setCurrent(QString(currentDir))==false){ WSP_COMMON_ERROR_LOG("setCurrent failed\n"); }

    char extension[MAX_LENGTH_OF_NAME];    
    wsp::GetExtension(extension, qfilename.toLatin1().data());
    if(strcmp(extension, "adri")==0){
        return LoadADRISeq(qfilename.toLatin1().data(), NULL, nChannels, end, false, false, bar);
    }
    return LoadImageSeq(qfilename.toLatin1().data(), nChannels, end, bar);
}

template<class T>
inline wsp::State wsp::ImageSeq<T>::SaveImageSeqDialog(int start_index, int end_index, const char *title) const
{
    QString selectedFilter;
    QString qfilename = QFileDialog::getSaveFileName(NULL, QString(title),
                            QDir::currentPath(), wsp::formatFilterImg, &selectedFilter);
    if(qfilename.isNull())
    { 
        WSP_COMMON_ERROR_LOG("Getting filename is failed\n"); 
        return WSP_STATE_FAILURE; 
    }
    char currentDir[MAX_LENGTH_OF_PATH];
    wsp::GetDir(currentDir, qfilename.toLatin1().data());
    if(QDir::setCurrent(QString(currentDir))==false)
    { 
        WSP_COMMON_WARNING_LOG("setCurrent failed\n"); 
    }

    char extension[MAX_LENGTH_OF_NAME];    
    wsp::GetExtension(extension, qfilename.toLatin1().data());
    if(strcmp(extension, "adri")==0)
    {
        QString qfilename_alpha = QFileDialog::getSaveFileName(NULL, QString("Save Alpha As ADRI"),
                                QDir::currentPath(), wsp::formatFilterADRI, &selectedFilter);
        if(qfilename_alpha.isNull()){ WSP_COMMON_ERROR_LOG("Getting filename is failed\n"); return WSP_STATE_FAILURE; }
        return SaveImageSeqAsADRI(qfilename.toLatin1().data(), qfilename_alpha.toLatin1().data(), start_index, end_index);
    }
    return SaveImageSeq(qfilename.toLatin1().data(), start_index, end_index);
}

template<class T>
inline wsp::State wsp::ImageSeq<T>::SaveAlphaSeqDialog(int start_index, int end, const char *title) const
{
    QString qfilename = QFileDialog::getSaveFileName(NULL, QString(title),
                            QDir::currentPath(), wsp::formatFilterImg, NULL);
    if(qfilename.isNull()){ WSP_COMMON_ERROR_LOG("Getting filename is failed\n"); return WSP_STATE_FAILURE; }
    char currentDir[MAX_LENGTH_OF_PATH];
    wsp::GetDir(currentDir, qfilename.toLatin1().data());
    if(QDir::setCurrent(QString(currentDir))==false){ WSP_COMMON_ERROR_LOG("setCurrent failed\n"); }

    return SaveAlphaSeq(qfilename.toLatin1().data(), start_index, end_index);
}
template<class T>
inline wsp::State wsp::ImageSeq<T>::SaveStateSeqDialog(int start_index, int end, const char *title) const
{
    QString qfilename = QFileDialog::getSaveFileName(NULL, QString(title),
                            QDir::currentPath(), wsp::formatFilterImg, NULL);
    if(qfilename.isNull()){ WSP_COMMON_ERROR_LOG("Getting filename is failed\n"); return WSP_STATE_FAILURE; }
    char currentDir[MAX_LENGTH_OF_PATH];
    wsp::GetDir(currentDir, qfilename.toLatin1().data());
    if(QDir::setCurrent(QString(currentDir))==false){ WSP_COMMON_ERROR_LOG("setCurrent failed\n"); }

    return SaveStateSeq(qfilename.toLatin1().data(), start_index, end_index);
}

template<class T>
inline wsp::State wsp::ImageSeq<T>::LoadRawDataSeqDialog(int end, const char *title, QProgressBar *bar)
{
    QString qfilename = QFileDialog::getOpenFileName(NULL, QString(title), QDir::currentPath(), wsp::formatFilterWspLibData);
    if(qfilename.isNull()){ WSP_COMMON_ERROR_LOG("Getting filename is failed\n"); return WSP_STATE_FAILURE; }
    char currentDir[MAX_LENGTH_OF_PATH];
    wsp::GetDir(currentDir, qfilename.toLatin1().data());
    if(QDir::setCurrent(QString(currentDir))==false){ WSP_COMMON_ERROR_LOG("setCurrent failed\n"); }
    return LoadRawDataSeq(qfilename.toLatin1().data(), end, bar);
}

template<class T>
inline wsp::State wsp::ImageSeq<T>::SaveRawDataSeqDialog(int start_index, int end_index, const char *title) const
{
    QString selectedFilter;
    QString qfilename = QFileDialog::getSaveFileName(NULL, QString(title),
                            QDir::currentPath(), wsp::formatFilterWspLibData, &selectedFilter);
    if(qfilename.isNull()){ WSP_COMMON_ERROR_LOG("Getting filename is failed\n"); return WSP_STATE_FAILURE; }
    char currentDir[MAX_LENGTH_OF_PATH];
    wsp::GetDir(currentDir, qfilename.toLatin1().data());
    if(QDir::setCurrent(QString(currentDir))==false){ WSP_COMMON_ERROR_LOG("setCurrent failed\n"); }

    return SaveRawDataSeq(qfilename.toLatin1().data(), start_index, end_index);
}

template<class T>
inline wsp::State wsp::ImageSeq<T>::LoadADRISeqDialog(int nChannels, int end, bool normalize, bool normalize_alpha, const char *title
        #ifdef USE_QT
        , QProgressBar *progress_bar
        #endif
        )
{
    QString qfilename = QFileDialog::getOpenFileName(NULL, QString(title), QDir::currentPath(), wsp::formatFilterADRI);
    if(qfilename.isNull()){ WSP_COMMON_ERROR_LOG("Getting filename is failed\n"); return WSP_STATE_FAILURE; }
    char currentDir[MAX_LENGTH_OF_PATH];
    wsp::GetDir(currentDir, qfilename.toLatin1().data());
    if(QDir::setCurrent(QString(currentDir))==false){ WSP_COMMON_ERROR_LOG("setCurrent failed\n"); }

    QString qfilename_alpha = QFileDialog::getOpenFileName(NULL, QString(title), QDir::currentPath(), wsp::formatFilterADRI);
    if(qfilename_alpha.isNull()){ 
        WSP_COMMON_WARNING_LOG("Getting filename of alpha is skipped\n"); 
    }else{
        wsp::GetDir(currentDir, qfilename.toLatin1().data());
        if(QDir::setCurrent(QString(currentDir))==false){ WSP_COMMON_WARNING_LOG("setCurrent failed\n"); }
    }
    return LoadADRISeq(qfilename.toLatin1().data(), qfilename_alpha.toLatin1().data(), nChannels, end, normalize, normalize_alpha
        #ifdef USE_QT
        , progress_bar
        #endif
        );
}

#endif

// Operators ------------------------------------------
template<class T> 
wsp::Image<T>& wsp::ImageSeq<T>::operator[](int index)
{ 
    //if(data_==NULL){ data_ = new wsp::Vector<wsp::Image<T>>(); }
    return (*data_)[index]; 
}
//template<class T> wsp::Image<T>& wsp::ImageSeq<T>::operator[](int index){
//    if(data_==NULL){ data_ = new wsp::Vector<wsp::Image<T>>(); }
//    if(index<0){ return (*data_)[0]; }
//    return operator[](static_cast<int>(index));
//}
template<class T> 
const wsp::Image<T> wsp::ImageSeq<T>::operator[](int index) const
{
    //if(data_==NULL){ data_ = new wsp::Vector<wsp::Image<T>>(); }
    return (*data_)[index];
}
//template<class T> const wsp::Image<T> wsp::ImageSeq<T>::operator[](int index) const{
//    //if(data_==NULL){ data_ = new wsp::Vector<wsp::Image<T>>(); }
//    if(index<0){ return (*data_)[0]; }
//    return operator[](static_cast<int>(index));
//}

template<class T> 
wsp::ImageSeq<T>& wsp::ImageSeq<T>::operator=(const wsp::ImageSeq<T>& rhs){
    if(has_own_data_==false)
    { 
        WSP_COMMON_ERROR_LOG("You can not reallocate image data because current data is external data\n"); 
        return *this; 
    }
    CopyFrom(rhs);
    return *this;
}

template<class T> 
bool wsp::ImageSeq<T>::operator==(const wsp::ImageSeq<T> &rhs)const
{
    return *data_==*rhs.data()
        && *seq_info_src_==rhs.SeqInfoSrc()
        && has_own_data_==rhs.has_own_data();
}

template<class T> 
bool wsp::ImageSeq<T>::operator!=(const wsp::ImageSeq<T> &rhs)const
{
    return !(*this!=rhs);
}


#endif

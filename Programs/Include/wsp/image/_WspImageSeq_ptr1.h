// _WspImageSeq.h
// Author 

#ifndef WspImage_WspImageSeq_H_
#define WspImage_WspImageSeq_H_

#include "_WspImage.h>

#ifdef USE_QT
#include <QtGui/QtGui>
#endif

BEGIN_WSP_CORE_NAMESPACE
template<class T>
class ImageSeq
{
protected:
    wsp::Vector<wsp::Image<T>*> *data_;
    wsp::SeqInfo *seq_info_src_;
    bool has_own_data_;


private:
    inline void Init(){
        if(data_==NULL){ data_ = WSP_NEW wsp::Vector<wsp::Image<T>*>(); }
        if(seq_info_src_==NULL){ seq_info_src_ = WSP_NEW wsp::SeqInfo(); }
    }

public:
    ImageSeq();
    ImageSeq(const char *in_filename);
    ImageSeq(const wsp::ImageSeq<T> &in_imgseq);
    ~ImageSeq();

    // Getter
    inline int width() const{ return SeqSize()==0? 0: (*data_)[0]->width(); }
    inline int height() const{ return SeqSize()==0? 0: (*data_)[0]->height(); }
    inline int num_of_channels() const{ return SeqSize()==0? 0: (*data_)[0]->num_of_channels(); }
    inline int SeqSize() const{ return data_==NULL? 0 : data_->length(); }
    inline wsp::SeqInfo SeqInfoSrc() const{ return seq_info_src_==NULL? wsp::SeqInfo(): *seq_info_src_; }
    inline wsp::Vector<wsp::Image<T>*>* data(){ return data_; }
    inline const wsp::Vector<wsp::Image<T>*>* data() const{ return data_; }
    //inline wsp::SeqInfo GetSeqInfoDst() const{ return m_seqInfoDst; }

    // Setter
    inline wsp::State Clear(){ 
        if(has_own_data_==false)
        { WSP_COMMON_ERROR_LOG("You can not reallocate image data because current data is external data\n"); return WSP_STATE_FAILURE; }

        data_->Clear();
        seq_info_src_->Clear();
        //delete data_;
        //delete seq_info_src_;
        //data_=NULL;
        //seq_info_src_=NULL;
    }
    template<typename U> 
    inline wsp::State CopyFrom(const wsp::ImageSeq<U> &img_seq, u32 dst_num_of_channels=0){
        if(has_own_data_==false)
        { WSP_COMMON_ERROR_LOG("You can not reallocate image data because current data is external data\n"); return WSP_STATE_FAILURE; }
        SetSeqSize(img_seq.SeqSize());
        u32 len = SeqSize();
        wsp::State state;
        for(u32 i=0; i<len; i++){
            state = (*data_)[i]->CopyFrom<U>(img_seq[i], dst_num_of_channels);
            if(state!=WSP_STATE_SUCCESS){
                WSP_COMMON_ERROR_LOG("Copying is failed\n"); return state;
            }
        }
        return WSP_STATE_SUCCESS;
    }

    inline wsp::State SetSeqSize(u32 num_of_seq){ 
        if(data_==NULL){ data_ = WSP_NEW wsp::Vector<wsp::Image<T>*>(); }
        return data_->SetLength(num_of_seq); 
    }
    inline void SetSeqInfo(wsp::SeqInfo info){
        if(seq_info_src_==NULL){ seq_info_src_ = WSP_NEW wsp::SeqInfo(); }
        *seq_info_src_ = wsp::SeqInfo(info); 
    }
    inline void SetDataFrom(wsp::Vector<wsp::Image<T>*> *seq_data){
        delete data_; 
        data_=seq_data; 
        seq_data=NULL;
        has_own_data_=false;
    }
    inline void SetDataInto(wsp::Vector<wsp::Image<T>*> *seq_data){
        seq_data = data_; 
        data_=NULL; 
        has_own_data_=true;
    }

    //wsp::State SetSeqInfoDst();
    //inline void setDestDir(const char* destDir){ strcpy(m_destDir, destDir); }
    //inline void setDestName(const char* destName){ strcpy(m_destName, destName); }

    // I/O
    //inline void WspImageHandler::printSeqInfoRgb() const{ cout<<m_seqInfoRgb; }
    //inline void WspImageHandler::printSeqInfoGray() const{ cout<<m_seqInfoGray; }

    wsp::State LoadImageSeq(const char *filename, u32 nChannels=0, int end=-1 
        #ifdef USE_QT
        , QProgressBar *progress_bar=NULL
        #endif
        );
    wsp::State LoadImageSeqAsAlpha(const char *filename, int end_index=-1
        #ifdef USE_QT
        , QProgressBar *progress_bar=NULL
        #endif
        ); 
    wsp::State LoadRawDataSeq(const char *filename, int end=-1
        #ifdef USE_QT
        , QProgressBar *progress_bar=NULL
        #endif
        );
    wsp::State LoadADRISeq(const char *filename, const char *filename_alpha=NULL, u32 nChannels=0, int end=-1, bool normalize=false, bool normalize_alpha=false
        #ifdef USE_QT
        , QProgressBar *progress_bar=NULL
        #endif
        );


    // end=-1 means that end depends on length of sequence
    wsp::State SaveImageSeq(const char *filename, int start_index=0, int end=-1) const;
    wsp::State SaveAlphaSeq(const char *filename, int start_index=0, int end=-1) const;
    wsp::State SaveStateSeq(const char *filename, int start_index=0, int end=-1) const;
    wsp::State SaveRawDataSeq(const char *filename, int start_index=0, int end=-1) const;
    wsp::State SaveImageSeqAsADRI(const char *filename, const char *filename_alpha, int start_index=0, int end=-1) const;

    #ifdef USE_QT
    inline wsp::State LoadImageSeqDialog(u32 nChannels=0, int end=-1, const char *title="Open Sequence", QProgressBar *progress_bar=NULL);
    inline wsp::State SaveImageSeqDialog(int start_index=0, int end=-1, const char *title="Save Sequence As") const;
    inline wsp::State SaveAlphaSeqDialog(int start_index=0, int end=-1, const char *title="Save Alpha Sequence As") const;
    inline wsp::State SaveStateSeqDialog(int start_index=0, int end=-1, const char *title="Save State Sequence As") const;
    inline wsp::State LoadRawDataSeqDialog(int end=-1, const char *title="Open Raw Data Sequence", QProgressBar *progress_bar=NULL);
    inline wsp::State SaveRawDataSeqDialog(int start_index=0, int end=-1, const char *title="Save Raw Data Sequence As") const;
    inline wsp::State LoadADRISeqDialog(u32 nChannels=0, int end=-1, bool normalize=true, bool normalize_alpha=false, const char *title="Open ADRI Sequence"
        #ifdef USE_QT
        , QProgressBar *progress_bar=NULL
        #endif
        );
    #endif

    // Operators --------------------------
    wsp::Image<T>& operator[](u32 index);
    wsp::Image<T>& operator[](int index);
    const wsp::Image<T> operator[](u32 index) const;
    const wsp::Image<T> operator[](int index) const;

    inline wsp::ImageSeq<T>& operator=(const wsp::ImageSeq<T>& other){
        if(has_own_data_==false)
        { WSP_COMMON_ERROR_LOG("You can not reallocate image data because current data is external data\n"); return *this; }
        CopyFrom(other);
        return *this;
    }

    
};

END_WSP_CORE_NAMESPACE


// Constructor, Destructor -------------------------------
#define INIT_WSP_IMAGE_SEQ seq_info_src_(NULL),data_(NULL),has_own_data_(true)
template<class T> wsp::ImageSeq<T>::ImageSeq()
    :INIT_WSP_IMAGE_SEQ
{ Init(); }
template<class T> wsp::ImageSeq<T>::ImageSeq(const char *filename)
    :INIT_WSP_IMAGE_SEQ
{
    Init();
    LoadImageSeq(filename);
}
template<class T> wsp::ImageSeq<T>::ImageSeq(const wsp::ImageSeq<T> &in_imgseq)
    :INIT_WSP_IMAGE_SEQ
{
    Init();
    Clear();
    this->SetSeqSize(in_imgseq.SeqSize());
    for(u32 i=0; i<this->SeqSize(); i++){
        *((*data_)[i]) = in_imgseq[i];
    }
}
template<class T> wsp::ImageSeq<T>::~ImageSeq(){
    if(has_own_data_){
        delete data_;
        delete seq_info_src_;
    }
}

// Getter ------------------------------------------------


// Setter ------------------------------------------------


// I/O ---------------------------------------------------
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
        wstat = (*data_)[i]->LoadRawData(path);
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
wsp::State wsp::ImageSeq<T>::LoadImageSeq(const char *filename, u32 nChannels, int end
#ifdef USE_QT
        , QProgressBar *progress_bar=NULL
#endif
        )
{
    if(has_own_data_==false)
    { WSP_COMMON_ERROR_LOG("You can not reallocate image data because current data is external data\n"); return WSP_STATE_FAILURE; }

    int endFrame;

    Init();

    wsp::GetSeqInfo(*seq_info_src_, filename);
    if(end>0 && seq_info_src_->start_frame<=end && end<seq_info_src_->end_frame)
    { seq_info_src_->end_frame = end; }

    //cout<<seq_info_src_<<endl;
    //printf("s=%d, e=%d\n", seq_info_src_->start_frame, end<seq_info_src_->end_frame);
    int len = seq_info_src_->end_frame - seq_info_src_->start_frame + 1;
    //if(seq_info_src_->num_padding==0){ len=1; }
    data_->SetLength(len);
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
            frame = seq_info_src_->start_frame + i;
            wsp::GetSeqPath(path, *seq_info_src_, frame);
            //#ifdef _OPENMP
            //#pragma omp critical
            //#endif
            {
                wstat = (*data_)[i]->LoadImageFile(path, nChannels);
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

    //u32 old_len = length_;
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
        wstat = (*data_)[i]->LoadImageAsAlpha(path);
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
wsp::State wsp::ImageSeq<T>::SaveImageSeq(const char *filename, int start_index, int end_index) const{
    if(data_==NULL || seq_info_src_==NULL){ WSP_COMMON_ERROR_LOG("data is NULL\n"); return WSP_STATE_FAILURE; }
    char path[MAX_LENGTH_OF_PATH];
    wsp::SeqInfo seqInfo(filename);
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
        (*data_)[i]->SaveImage(path);
    }
    return WSP_STATE_SUCCESS;
}

template<class T>
wsp::State wsp::ImageSeq<T>::SaveAlphaSeq(const char *filename, int start_index, int end_index) const{
    if(data_==NULL || seq_info_src_==NULL){ WSP_COMMON_ERROR_LOG("data is NULL\n"); return WSP_STATE_FAILURE; }
    char path[MAX_LENGTH_OF_PATH];
    wsp::SeqInfo seqInfo(filename);
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
        (*data_)[i]->SaveAlpha(path);
    }
    return WSP_STATE_SUCCESS;
}

template<class T>
wsp::State wsp::ImageSeq<T>::SaveStateSeq(const char *filename, int start_index, int end_index) const{
    if(data_==NULL || seq_info_src_==NULL){ WSP_COMMON_ERROR_LOG("data is NULL\n"); return WSP_STATE_FAILURE; }
    char path[MAX_LENGTH_OF_PATH];
    wsp::SeqInfo seqInfo(filename);
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
        (*data_)[i]->SaveState(path);
    }
    return WSP_STATE_SUCCESS;
}


template<class T>
wsp::State wsp::ImageSeq<T>::SaveRawDataSeq(const char *filename, int start_index, int end_index) const{
    if(data_==NULL || seq_info_src_==NULL){ WSP_COMMON_ERROR_LOG("data is NULL\n"); return WSP_STATE_FAILURE; }
    char path[MAX_LENGTH_OF_PATH];
    wsp::SeqInfo seqInfo(filename);
    int start_frame, len;

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
        (*data_)[i]->SaveRawData(path);
    }
    return WSP_STATE_SUCCESS;
}


template<class T>
wsp::State wsp::ImageSeq<T>::LoadADRISeq(const char *filename, const char *filename_alpha, u32 nChannels, int end, bool normalize, bool normalize_alpha
        #ifdef USE_QT
        , QProgressBar *progress_bar=NULL
        #endif
        )
{
    if(has_own_data_==false)
    { WSP_COMMON_ERROR_LOG("You can not reallocate image data because current data is external data\n"); return WSP_STATE_FAILURE; }

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
        wsp::Image<int> tmp_img(*((*data_)[i]));
        state = tmp_img.LoadADRI(path, path_alpha, 3);
        if(state!=WSP_STATE_SUCCESS){ WSP_COMMON_ERROR_LOG("Loading ADRI failed\n"); return state; }
        if(normalize){ tmp_img.Normalize(0, 255);}
        if(normalize_alpha){ tmp_img.NormalizeAlpha(0, 255); }
        (*data_)[i]->CopyFrom(tmp_img);
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
        (*data_)[i]->SaveAsADRI(path, path_alpha);
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
inline wsp::State wsp::ImageSeq<T>::LoadImageSeqDialog(u32 nChannels, int end, const char *title, QProgressBar *bar)
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
    if(qfilename.isNull()){ WSP_COMMON_ERROR_LOG("Getting filename is failed\n"); return WSP_STATE_FAILURE; }
    char currentDir[MAX_LENGTH_OF_PATH];
    wsp::GetDir(currentDir, qfilename.toLatin1().data());
    if(QDir::setCurrent(QString(currentDir))==false){ WSP_COMMON_ERROR_LOG("setCurrent failed\n"); }

    char extension[MAX_LENGTH_OF_NAME];    
    wsp::GetExtension(extension, qfilename.toLatin1().data());
    if(strcmp(extension, "adri")==0){
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
inline wsp::State wsp::ImageSeq<T>::LoadADRISeqDialog(u32 nChannels, int end, bool normalize, bool normalize_alpha, const char *title
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
template<class T> wsp::Image<T>& wsp::ImageSeq<T>::operator[](u32 index){ 
    if(data_==NULL){ data_ = WSP_NEW wsp::Vector<wsp::Image<T>*>(); }
    return *((*data_)[index]); 
}
template<class T> wsp::Image<T>& wsp::ImageSeq<T>::operator[](int index){
    if(data_==NULL){ data_ = WSP_NEW wsp::Vector<wsp::Image<T>*>(); }
    if(index<0){ return *((*data_)[0]); }
    return operator[](static_cast<u32>(index));
}
template<class T> const wsp::Image<T> wsp::ImageSeq<T>::operator[](u32 index) const{
    //if(data_==NULL){ data_ = WSP_NEW wsp::Vector<wsp::Image<T>*>(); }
    return *((*data_)[index]);
}
template<class T> const wsp::Image<T> wsp::ImageSeq<T>::operator[](int index) const{
    //if(data_==NULL){ data_ = WSP_NEW wsp::Vector<wsp::Image<T>*>(); }
    if(index<0){ return *((*data_)[0]); }
    return operator[](static_cast<u32>(index));
}

#endif
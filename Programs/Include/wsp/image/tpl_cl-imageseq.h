/**
 * @file tpl_cl-imageseq.h
 * 
 */

#ifndef WSP_IMAGE_TEMPLATE_CLASS_IMAGESEQ_H_
#define WSP_IMAGE_TEMPLATE_CLASS_IMAGESEQ_H_

#ifndef __WSP_COMMONCORE_RESULT_H__
#include <wsp/common/result.h>
#endif
#include "tpl_cl-image.h"

#ifdef USE_QT
#include <QtGui/QtGui>
#endif

namespace wsp{
    //! \ingroup WspImage
    //! \brief Image sequence manipulation 
    /*!
      wsp::ImageSeq is a class which provides access to some of image sequence
      manipulation functionality. 
    */
    template<class T>
    class ImageSeq
    {
    public:
        ImageSeq();
        explicit ImageSeq( u32 width, u32 height, u32 num_channels, u32 num_frames );
        explicit ImageSeq( const char *in_filename );
        ImageSeq( const wsp::ImageSeq<T> &in_imgseq );
        ~ImageSeq();

        /* ---------------------
            Getters 
           ---------------------*/
        inline int                               width()           const;
        inline int                               height()          const;
        inline int                               num_of_channels() const;
        inline int                               SeqSize()         const;
        inline wsp::SeqInfo                      SeqInfoSrc() const;
        inline bool                              has_own_data() const;
        inline wsp::Vector<wsp::Image<T>>*       data();
        inline const wsp::Vector<wsp::Image<T>>* data() const;
        inline wsp::Image<T>*                    GetRawData() { return data_->data(); }
        inline const wsp::Image<T>*              GetRawData() const{ return data_->data(); }
        inline wsp::Image<T>*                    GetImageData(int frame);
        //inline wsp::SeqInfo GetSeqInfoDst() const;

        /* ---------------------
            Setters
           ---------------------*/
        wsp::Result     Resize( u32 width, u32 height );
        /**
         * @brief Resize canvas of all images in sequence.
         */
        void              ResizeCanvas( u32 width, u32 height );
        /**
         * @brief Clear sequence, the size of sequence will be zero.
         */
        inline wsp::State Clear();
        /**
         * @brief Append copied image to the tail of sequence.
         */
        template<typename _InputType>
        wsp::Result       Append( const wsp::Image<_InputType> &image );

        template<typename U> 
        inline wsp::State CopyFrom(const wsp::ImageSeq<U> &img_seq, int dst_num_of_channels=0);

        inline wsp::State SetSeqSize(int num_of_seq);
        inline void       SetSeqInfo(wsp::SeqInfo info);
        inline void       SetDataFrom(wsp::Vector<wsp::Image<T>> *seq_data);
        inline void       SetDataInto(wsp::Vector<wsp::Image<T>> *seq_data);
        inline wsp::State SetNumOfChannels( int num_of_channels );
        inline wsp::State ExtendsNumOfChannels( int extended_num_of_channels );


        wsp::State LoadSeq(const char *filename, int nChannels=0, int end=-1, bool as_alpha=false
            #ifdef USE_QT
            , QProgressBar *progress_bar=NULL
            #endif
        );
        wsp::State LoadImageSeq(const char *filename, int nChannels=0, int end=-1 
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
        wsp::State LoadADRISeq(const char *filename, const char *filename_alpha=NULL, int nChannels=0, int end=-1, bool normalize=false, bool normalize_alpha=false
            #ifdef USE_QT
            , QProgressBar *progress_bar=NULL
            #endif
        );
        wsp::State LoadVideo(const char *filename);


        // end=-1 means that end depends on length of sequence
        wsp::State SaveImageSeq(const char *filename, int start_index=-1, int end=-1) const;
        wsp::State SaveAlphaSeq(const char *filename, int start_index=-1, int end=-1) const;
        wsp::State SaveStateSeq(const char *filename, int start_index=-1, int end=-1) const;
        wsp::State SaveRawDataSeq(const char *filename, int start_index=-1, int end=-1) const;
        wsp::State SaveImageSeqAsADRI(const char *filename, const char *filename_alpha, int start_index=-1, int end=-1) const;

        #ifdef USE_QT
        inline wsp::State LoadImageSeqDialog(int nChannels=0, int end=-1, const char *title="Open Sequence", QProgressBar *progress_bar=NULL);
        inline wsp::State SaveImageSeqDialog(int start_index=0, int end=-1, const char *title="Save Sequence As") const;
        inline wsp::State SaveAlphaSeqDialog(int start_index=0, int end=-1, const char *title="Save Alpha Sequence As") const;
        inline wsp::State SaveStateSeqDialog(int start_index=0, int end=-1, const char *title="Save State Sequence As") const;
        inline wsp::State LoadRawDataSeqDialog(int end=-1, const char *title="Open Raw Data Sequence", QProgressBar *progress_bar=NULL);
        inline wsp::State SaveRawDataSeqDialog(int start_index=0, int end=-1, const char *title="Save Raw Data Sequence As") const;
        inline wsp::State LoadADRISeqDialog(int nChannels=0, int end=-1, bool normalize=true, bool normalize_alpha=false, const char *title="Open ADRI Sequence"
            #ifdef USE_QT
            , QProgressBar *progress_bar=NULL
            #endif
            );
        #endif

        // Operators --------------------------
        wsp::Image<T>& operator[](int index);
        const wsp::Image<T> operator[](int index) const;

        inline wsp::ImageSeq<T>& operator=(const wsp::ImageSeq<T>& rhs);
        inline bool operator==(const wsp::ImageSeq<T> &rhs)const;
        inline bool operator!=(const wsp::ImageSeq<T> &rhs)const;

    private:
        u32 width_, height_, num_channels_;
        wsp::Vector<wsp::Image<T>> *data_;
        wsp::SeqInfo *seq_info_src_;
        bool has_own_data_;


    private:
        inline void Init();
    
    };


    template<typename T>
    wsp::State OpenSequence(wsp::ImageSeq<T> &io_imgseq, const char *filename, bool as_alpha)
    {
        char extension[MAX_LENGTH_OF_NAME];
        wsp::GetExtension(extension, filename);

        printf("OpenSequence\n");
        wsp::StartTimer();
        if(as_alpha){
            if(strcmp(extension, "adri")==0){
                io_imgseq.LoadADRISeq(NULL, filename, 0U, -1, false, true);
            }else{
                io_imgseq.LoadImageSeqAsAlpha(filename, -1);
            }
        }else{
            if(strcmp(extension, "adri")==0){
                io_imgseq.LoadADRISeq(filename, NULL, 0U, -1, false, false);
            }else{
                io_imgseq.LoadImageSeq(filename, 0U, -1);
            }
        }
        wsp::StopTimer();
        if(io_imgseq.SeqSize()==0){ WSP_COMMON_ERROR_LOG("No image sequence loaded\n"); return WSP_STATE_FAILURE; }

        return WSP_STATE_SUCCESS;
    }
}

#include "tpl_cl-imageseq.inl"

#endif

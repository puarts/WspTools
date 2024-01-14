/**
 * @file cl-qt_image_seq_view.hpp
 * @author Junichi Nishikata
 */

#ifndef __WSP_GUI_IMAGE_VIEWER_H__
#define __WSP_GUI_IMAGE_VIEWER_H__

#include <string>

#include <QtGui/QtGui>

#include <wsp/image/tpl_cl-imageseq.h>
#include <wsp/sound/tpl_cl-sound.h>
#include "cl-qt_paint_area.hpp"
#include "cl-qt_abstract_mdisubwin.hpp"



namespace wsp{


    //! \ingroup WspQtGui
    //! \brief Image viewer
    /*!
        wsp::QtImageSeqView is a widget which can view image or image sequence.
        You need to call SetExternalImageSeq() to set image sequence data before use wsp::QtImageSeqView.
    */
    class WSP_DLL_EXPORT QtImageSeqView
        : public QWidget
    {
        Q_OBJECT
    public:
        enum Channel{
            CHANNEL_COLOR,
            CHANNEL_ALPHA,
            CHANNEL_STATE,
            CHANNEL_UNKNOWN
        };
    public:
        QtImageSeqView(QWidget *parent=NULL);
        QtImageSeqView(wsp::ImageSeq<SIGNED_IMG_DATA_TYPE> *image_seq, QWidget *parent=NULL);
        QtImageSeqView(wsp::Image<SIGNED_IMG_DATA_TYPE> *image, QWidget *parent=NULL);
        QtImageSeqView(wsp::Sound<SIGNED_IMG_DATA_TYPE> *sound, QWidget *parent=NULL);
        ~QtImageSeqView();

        // accessors
        //int id(){ return id_; }
        wsp::QtPaintArea*                    display_label(){ return display_label_; }
        wsp::ImageSeq<SIGNED_IMG_DATA_TYPE>* image_seq(){ return image_seq_; }
        const char*                          data_path(){ return data_path_.c_str(); }
        QSlider*                             time_slider(){ return time_slider_; }
        wsp::QtImageSeqView::Channel         GetCurrentChannel() const
        {
            if(color_button_->isEnabled()==false){      return wsp::QtImageSeqView::CHANNEL_COLOR; }
            else if(alpha_button_->isEnabled()==false){ return wsp::QtImageSeqView::CHANNEL_ALPHA; }
            else if(state_button_->isEnabled()==false){ return wsp::QtImageSeqView::CHANNEL_STATE; }
            return wsp::QtImageSeqView::CHANNEL_UNKNOWN; 
        }
        bool                                 NormalizeIsChecked(){ return normalize_checkbox_->isChecked(); }

        std::string data_path() const{ return data_path_; }

        /** @brief Return true if the image is already loaded, otherwise return false. */
        bool HasImage();

        /** @brief Hide state button. */
        void HideStateButton();

        // mutators
        //void set_id(int id){ id_ = id; }
        void SetExternalImageSeq(wsp::ImageSeq<SIGNED_IMG_DATA_TYPE> *image_seq);
        void SetImage( wsp::Image<SIGNED_IMG_DATA_TYPE> *image);
        void SetDisplayLabel(wsp::QtPaintArea*display_label){
            display_label_ = display_label;
        }

        void updateView(){
            if( color_button_->isEnabled()==false ){ slot_showColor(); }
            if( alpha_button_->isEnabled()==false ){ slot_showAlpha(); }
            if( state_button_->isEnabled()==false ){ slot_showState(); }
        }
        void set_data_path( char *path ){ data_path_ = std::string(path); }

        void SetProgressBar( QProgressBar *progress_bar ){ progress_bar_ = progress_bar; }

        virtual void dragEnterEvent(QDragEnterEvent *event);
        virtual void dragMoveEvent(QDragMoveEvent *event);
        virtual void dragLeaveEvent(QDragLeaveEvent *event);
        virtual void dropEvent(QDropEvent *event);



        wsp::State OpenSequence(const char *filename, bool as_alpha);
        wsp::State SaveSequence();


    private slots:
        void slot_changeFrame(int frame);
        void slot_PlayStop();
        void slot_showColor();
        void slot_showAlpha();
        void slot_showState();

    private:
        void InitGui();
        void SynchGuiWithImageSeq();


    private:
        class PlayThread;
        friend class PlayThread;

        // data -------------
        // data
        //int  id_;
        bool has_own_data_;
        std::string data_path_;

        // shared data
        wsp::ImageSeq<SIGNED_IMG_DATA_TYPE> *image_seq_;

        PlayThread *play_thread_;

        // gui --------------
        // indivisual gui
        wsp::QtPaintArea *display_label_;
        QSlider          *time_slider_;
        QPushButton      *play_button_;//, *stop_button_;
        QPushButton      *color_button_, *alpha_button_, *state_button_;
        QCheckBox        *red_checkbox_, *green_checkbox_, *blue_checkbox_;
        QCheckBox        *alpha_mask_checkbox_, *normalize_checkbox_;
        QLCDNumber       *frame_display_;
        QVBoxLayout      *image_layout_;
        QScrollArea      *image_scroll_area_;

        // shared gui
        QProgressBar *progress_bar_;

        // layout
        QVBoxLayout *main_vblayout_;
        QHBoxLayout *mode_btn_layout_;
        QHBoxLayout *display_mode_layout_;
        QHBoxLayout *time_slider_layout_;

    };




    typedef wsp::QtMdiSubWindowWapper<QtImageSeqView, wsp::gui::WINDOW_TYPE_IMAGE_VIEW> QtImageSeqViewMdiSubWin;
    typedef wsp::QtDialogWapper      <QtImageSeqView, wsp::gui::WINDOW_TYPE_IMAGE_VIEW> QtImageSeqViewDialog;
}

#endif

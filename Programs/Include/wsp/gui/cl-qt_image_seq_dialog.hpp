/**
 * @file cl-qt_image_seq_dialog.hpp
 * @author Junichi Nishikata
 */

#ifndef WSP_GUI_CLASS_QT_IMAGE_SEQ_DIALOG_HPP__
#define WSP_GUI_CLASS_QT_IMAGE_SEQ_DIALOG_HPP__

#include "_define_gui.h"

#include <QtGui/QtGui>

#include <wsp/image/tpl_cl-imageseq.h>
#include <wsp/sound/tpl_cl-sound.h>

namespace wsp{
    class QtImageSeqView;

    class WSP_DLL_EXPORT QtImageSeqDialogViewer
        : public QDialog
    {
    public:
        QtImageSeqDialogViewer(wsp::ImageSeq<SIGNED_IMG_DATA_TYPE> *_image_seq, QWidget *_parent=NULL);
        QtImageSeqDialogViewer(wsp::Image<SIGNED_IMG_DATA_TYPE> *_image, QWidget *_parent=NULL);
        QtImageSeqDialogViewer(wsp::Sound<SIGNED_IMG_DATA_TYPE> *_sound, QWidget *_parent=NULL);
        ~QtImageSeqDialogViewer();

        /** @brief @brief-----------------------
              accessors 
            -----------------------*/
        wsp::QtImageSeqView* widget_data(){ return image_seq_viewer_; }

    private:
        wsp::QtImageSeqView *image_seq_viewer_;
        QVBoxLayout *main_layout_;

    private:
        void Init();

    };
}

#endif

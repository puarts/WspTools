/**
 * @file cl-qt_image_seq_dialog.cpp
 * @author Junichi Nishikata
 */

#include "cl-qt_image_seq_dialog.hpp"

#include "cl-qt_image_seq_view.hpp"


void wsp::QtImageSeqDialogViewer::Init()
{
    this->setLayout(main_layout_);
    setWindowTitle(tr("Image View"));
}

#define INIT_WSP_QtImageSeqDialogViewer \
    image_seq_viewer_(NULL)\
    ,main_layout_(new QVBoxLayout)

wsp::QtImageSeqDialogViewer::QtImageSeqDialogViewer(
    wsp::ImageSeq<SIGNED_IMG_DATA_TYPE> *_image_seq, 
    QWidget *_parent
)
    : QDialog(_parent), INIT_WSP_QtImageSeqDialogViewer
{
    Init();
    image_seq_viewer_ = new wsp::QtImageSeqView(_image_seq);
    main_layout_->addWidget(image_seq_viewer_);
}

wsp::QtImageSeqDialogViewer::QtImageSeqDialogViewer(
    wsp::Image<SIGNED_IMG_DATA_TYPE> *_image, 
    QWidget *_parent
)
    : QDialog(_parent), INIT_WSP_QtImageSeqDialogViewer
{
    Init();
    image_seq_viewer_ = new wsp::QtImageSeqView(_image);
    main_layout_->addWidget(image_seq_viewer_);
}

wsp::QtImageSeqDialogViewer::QtImageSeqDialogViewer(
    wsp::Sound<SIGNED_IMG_DATA_TYPE> *_sound, 
    QWidget *_parent
)
    : QDialog(_parent), INIT_WSP_QtImageSeqDialogViewer
{
    Init();
    image_seq_viewer_ = new wsp::QtImageSeqView(_sound);
    main_layout_->addWidget(image_seq_viewer_);
}

wsp::QtImageSeqDialogViewer::~QtImageSeqDialogViewer()
{
    delete image_seq_viewer_;
    delete main_layout_;
}

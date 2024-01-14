/* 
 * @file cl-qt_image_seq_viewer.cpp
 * @author Junichi Nishikata
 */

#include "_define_gui.h"

#include <wsp/common/fn-debug.h>
#include <wsp/common/_define_global.h>
#include <wsp/image/_tpl_fn-imgproc.hpp>
#include "cl-qt_image_seq_view.hpp"


#define ENABLE_WSP_GUI_LOG
#include "fn-gui_io.h"


#define BIN_DIR ""

#define COLOR_ICON (BIN_DIR "icons/btn-color.png")
#define ALPHA_ICON (BIN_DIR "icons/btn-alpha.png")
#define WSP_STATE_ICON (BIN_DIR "icons/btn-state.png")

#define R_ICON  (BIN_DIR "icons/cb-red.png")
#define G_ICON  (BIN_DIR "icons/cb-green.png")
#define B_ICON  (BIN_DIR "icons/cb-blue.png")
#define A_ICON  (BIN_DIR "icons/cb-alpha.png")

#define R_STR ""
#define G_STR ""
#define B_STR ""
#define A_STR ""

#define PLAY_ICON  (BIN_DIR "icons/btn-play.png")
#define STOP_ICON  (BIN_DIR "icons/btn-pause.png")

#define PLAY_STR  ""
#define STOP_STR  ""


class wsp::QtImageSeqView::PlayThread : public QThread
{
public:
    explicit PlayThread();//コンストラクタ
    virtual void stop();//スレッドを止める
 
protected:
    virtual void run();//スレッドを始める関数（再実装）
 
private:
    friend class wsp::QtImageSeqView;
    volatile bool is_playing_;//volatileは処理系の最適化の抑制の意味
    wsp::QtImageSeqView *img_seq_view_;
};


wsp::QtImageSeqView::PlayThread::PlayThread()
    : img_seq_view_(NULL), is_playing_(false)
{
}
 
void wsp::QtImageSeqView::PlayThread::run()//QThreadのrunの再実装
{
    is_playing_ = true;


    float fps = 30.0f;
    s32 wait_ms = 0;
    for( s32 frame=img_seq_view_->time_slider_->value(); is_playing_; ++frame)
    {
        wsp::StartTimer();
        img_seq_view_->time_slider_->setValue(frame);
        qApp->processEvents();
        msleep(33);
        if(frame==img_seq_view_->image_seq_->SeqInfoSrc().end_frame)
        {
            frame = img_seq_view_->image_seq_->SeqInfoSrc().start_frame-1;
        }
        fps = 1.0f/wsp::StopTimer();
        //wait_ms = fps - 30.0f;
        //Sleep(wait_ms<0? 0: wait_ms);
        WSP_GUI_LOG("%f fps", fps);
    }
}
 
void wsp::QtImageSeqView::PlayThread::stop()
{
    is_playing_ = false;
}






void wsp::QtImageSeqView::HideStateButton()
{
    state_button_->hide();
}

void wsp::QtImageSeqView::SynchGuiWithImageSeq()
{
    WSP_GUI_DEBUG_LOG("start");
    if(image_seq_==NULL)
    { 
        return; 
    }
    time_slider_->setRange(image_seq_->SeqInfoSrc().start_frame, image_seq_->SeqInfoSrc().end_frame);
    if(image_seq_->SeqSize()!=0){
        slot_showColor();
    }
}

void wsp::QtImageSeqView::SetImage( wsp::Image<SIGNED_IMG_DATA_TYPE> *image )
{
    WSP_GUI_DEBUG_LOG("start");
    if (image_seq_==NULL)
    {
        image_seq_ = new wsp::ImageSeq<SIGNED_IMG_DATA_TYPE>();
    }
    image_seq_->SetSeqSize(1);
    (*image_seq_)[0] = *image;
    has_own_data_ = true;

    SynchGuiWithImageSeq();
}

void wsp::QtImageSeqView::SetExternalImageSeq( wsp::ImageSeq<SIGNED_IMG_DATA_TYPE> *image_seq )
{ 
    WSP_GUI_DEBUG_LOG("start");
    if (image_seq_!=NULL)
    {
        delete image_seq_;
    }
    image_seq_ = image_seq; 
    has_own_data_ = false;

    SynchGuiWithImageSeq();
}


void wsp::QtImageSeqView::slot_showColor()
{
    color_button_->setEnabled(false);
    alpha_button_->setEnabled(true);
    state_button_->setEnabled(true);

    if( image_seq_ == NULL || image_seq_->SeqSize()==0 ){ return; }

    int index = time_slider_->value() - image_seq_->SeqInfoSrc().start_frame;
    wsp::Image<SIGNED_IMG_DATA_TYPE> *image = &(*image_seq_)[index];
    int w = image->width();
    int h = image->height();
    int src_nc= image->num_of_channels();
    int dst_nc = 4;
    int npix = w*h;
    int src_len = npix*src_nc;
    int dst_len = npix*dst_nc;
    bool mask_alpha = alpha_mask_checkbox_->isChecked();
    //display_label_->pixmap()->scaled(w, h);
    QImage tmp_img(w, h, QImage::Format_RGB32);
    bool show_r = red_checkbox_->isChecked();
    bool show_g = green_checkbox_->isChecked();
    bool show_b = blue_checkbox_->isChecked();
    bool normalize = normalize_checkbox_->isChecked();
    SIGNED_IMG_DATA_TYPE *src_ptr = image->data();
    uchar *dst_ptr = const_cast<uchar*>(tmp_img.bits());
    uchar *alpha_ptr = image->alpha_data();
    //! just copy
    memset(dst_ptr, 0, dst_len);
    if(show_r){
        wsp::img::CopyOneChannel(src_ptr, alpha_ptr, dst_ptr, npix, src_nc, dst_nc, 0, 2, mask_alpha, normalize);
    }
    if(show_g){
        wsp::img::CopyOneChannel(src_ptr, alpha_ptr, dst_ptr, npix, src_nc, dst_nc, 1, 1, mask_alpha, normalize);
    }
    if(show_b){
        wsp::img::CopyOneChannel(src_ptr, alpha_ptr, dst_ptr, npix, src_nc, dst_nc, 2, 0, mask_alpha, normalize);
    }
    display_label_->setImage(tmp_img);
}

void wsp::QtImageSeqView::slot_showAlpha()
{
    color_button_->setEnabled(true);
    alpha_button_->setEnabled(false);
    state_button_->setEnabled(true);

    if( image_seq_ == NULL || image_seq_->SeqSize()==0 ){ return; }

    int index = time_slider_->value() - (*image_seq_).SeqInfoSrc().start_frame;

    wsp::Image<SIGNED_IMG_DATA_TYPE> *image = &(*image_seq_)[index];
    int w = image->width();
    int h = image->height();
    int src_nc= 1;
    int dst_nc = 4;
    int npix = w*h;
    bool normalize = normalize_checkbox_->isChecked();
    QImage tmp_img(w, h, QImage::Format_RGB32);
    uchar *src_ptr = (*image_seq_)[index].alpha_data();
    uchar *dst_ptr = const_cast<uchar*>(tmp_img.bits());
    wsp::img::CopyOneChannel(src_ptr, NULL, dst_ptr, npix, src_nc, dst_nc, 0, 0, false, normalize);
    wsp::img::CopyOneChannel(src_ptr, NULL, dst_ptr, npix, src_nc, dst_nc, 0, 1, false, normalize);
    wsp::img::CopyOneChannel(src_ptr, NULL, dst_ptr, npix, src_nc, dst_nc, 0, 2, false, normalize);

    display_label_->setImage(tmp_img);
}

void wsp::QtImageSeqView::slot_showState()
{
    color_button_->setEnabled(true);
    alpha_button_->setEnabled(true);
    state_button_->setEnabled(false);

    if( image_seq_ == NULL || image_seq_->SeqSize()==0 ){ return; }
    int index = time_slider_->value() - (*image_seq_).SeqInfoSrc().start_frame;
    wsp::Image<SIGNED_IMG_DATA_TYPE> *image = &(*image_seq_)[index];
    int w = image->width();
    int h = image->height();
    QImage tmp_img(w, h, QImage::Format_Indexed8);
    uchar *dst_ptr = const_cast<uchar*>(tmp_img.bits());
    wsp::CopyArray(dst_ptr, (*image_seq_)[index].state_data(), (*image_seq_)[index].length());
    display_label_->setImage(tmp_img);
}

void wsp::QtImageSeqView::slot_changeFrame(int frame)
{
    if( image_seq_ == NULL )
    {
        return;
    }

    if(color_button_->isEnabled()==false){
        slot_showColor();
    }else if(alpha_button_->isEnabled()==false){
        slot_showAlpha();
    }else if(state_button_->isEnabled()==false){
        slot_showState();
    }
}

void wsp::QtImageSeqView::slot_PlayStop()
{
    WSP_GUI_DEBUG_LOG("start");

    if( image_seq_ == NULL )
    {
        return;
    }

    QString exe_dir_path = QString( WSP_EXE_DIR_PATH );
    if( play_thread_->isFinished() )
    {
        play_button_->setIcon( QIcon( exe_dir_path + QString(STOP_ICON) ) );
        play_thread_->start();
        return;
    }
    else
    {
        play_button_->setIcon( QIcon( exe_dir_path + QString(PLAY_ICON) ) );
        play_thread_->stop();
        play_thread_->wait();
        return;
    }

}

bool wsp::QtImageSeqView::HasImage()
{
    WSP_ASSERT( image_seq_ != NULL, "image sequence data must be set before use %s", __FUNCTION__);

    return image_seq_->SeqSize() > 0;
}

wsp::State wsp::QtImageSeqView::OpenSequence(const char *filename, bool as_alpha)
{
    WSP_ASSERT( image_seq_ != NULL, "image sequence data must be set before use %s", __FUNCTION__);

    data_path_ = filename;

    char extension[MAX_LENGTH_OF_NAME];
    wsp::GetExtension(extension, filename);

    wsp::StartTimer();
    if(as_alpha){
        if(strcmp(extension, "adri")==0){
            image_seq_->LoadADRISeq(NULL, filename, 0U, -1, false, true, progress_bar_);
        }else{
            image_seq_->LoadImageSeqAsAlpha(filename, -1, progress_bar_);
        }
    }else{
        if(strcmp(extension, "adri")==0){
            image_seq_->LoadADRISeq(filename, NULL, 0U, -1, false, false, progress_bar_);
        }else{
            image_seq_->LoadImageSeq(filename, 0U, -1, progress_bar_);
        }
    }
    wsp::StopTimer();
    if(image_seq_->SeqSize()==0)
    { 
        WSP_COMMON_ERROR_LOG("No image sequence loaded\n"); 
        return WSP_STATE_FAILURE; 
    }
    time_slider_->setRange(image_seq_->SeqInfoSrc().start_frame, image_seq_->SeqInfoSrc().end_frame);
    connect(time_slider_, SIGNAL(valueChanged(int)), this, SLOT(slot_changeFrame(int)));

    this->setWindowFilePath(QString(filename));

    setBackgroundRole(QPalette::Dark);

    slot_changeFrame(0);

    return WSP_STATE_SUCCESS;
}

wsp::State wsp::QtImageSeqView::SaveSequence()
{
    WSP_ASSERT( image_seq_ != NULL, "image sequece data must be set before use %s", __FUNCTION__ );
    s32 frame = time_slider_->value() - time_slider_->minimum();
    WSP_GUI_DEBUG_LOG( "current_frame = %d", frame );
    s32 start_index = frame;
    s32 end_index = frame;
    {
        QString selectedFilter;
        QString qfilename;
        {
            QFileDialog dialog;
            char default_name[128];
            dialog.setFilter( wsp::kDevIlSavingSupportedFmt );
            dialog.selectNameFilter ("BMP (*.bmp)");
            dialog.setWindowTitle("Save As");
            dialog.setLabelText(QFileDialog::Accept, "Save");
            wsp::GetName( default_name, data_path_.c_str() );
            printf("default_name = %s\n", default_name);
            dialog.selectFile( default_name );
            dialog.setDirectory( QDir::currentPath() );

            dialog.exec();
            QStringList file_list = dialog.selectedFiles();
            if( file_list.size() > 0)
            {
                qfilename = file_list[0];
            }
        }
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
            return image_seq_->SaveImageSeqAsADRI(qfilename.toLatin1().data(), qfilename_alpha.toLatin1().data(), start_index, end_index);
        }
        return image_seq_->SaveImageSeq(qfilename.toLatin1().data(), start_index, end_index);
    }
}

void wsp::QtImageSeqView::dropEvent(QDropEvent *event)
{
    WSP_GUI_DEBUG_LOG("start");

    WSP_ASSERT( image_seq_ != NULL, "image sequece data must be set before use %s", __FUNCTION__ );

    const QMimeData *mimeData = event->mimeData();
    char filename[MAX_LENGTH_OF_NAME];
    //char *filename_ptr;

    if(mimeData->hasUrls()==NULL)
    { 
        WSP_COMMON_ERROR_LOG("URL is NULL\n"); 
        return; 
    }
    QList<QUrl> urlList = mimeData->urls();
    QString urlText = urlList.at(0).path();
    //printf("%s\n", urlText.toLatin1().data());
    //filename_ptr = urlText.toLatin1().data()+1;
    strcpy(filename, urlText.toLatin1().data()+1);
    printf("%s\n", filename);
    char currentDir[MAX_LENGTH_OF_NAME];
    wsp::GetDir(currentDir, filename);
    if(QDir::setCurrent(QString(currentDir))==false)
    { 
        WSP_COMMON_ERROR_LOG("setCurrent failed\n"); 
    }


    //char extension[MAX_LENGTH_OF_NAME];
    //wsp::GetExtension(extension, filename);
    if(color_button_->isEnabled()){
        OpenSequence(filename, true);
    }else{
        OpenSequence(filename, false);
    }

    event->acceptProposedAction();
}

void wsp::QtImageSeqView::dragLeaveEvent(QDragLeaveEvent *event)
{
    WSP_GUI_DEBUG_LOG("start");

    event->accept();
}

void wsp::QtImageSeqView::dragEnterEvent(QDragEnterEvent *event)
{
    WSP_GUI_DEBUG_LOG("start");

    //setText(tr("<drop content>"));
    setBackgroundRole(QPalette::Highlight);

    event->acceptProposedAction();
    //emit changed(event->mimeData());
}

void wsp::QtImageSeqView::dragMoveEvent(QDragMoveEvent *event)
{
    WSP_GUI_DEBUG_LOG("start");

    event->acceptProposedAction();
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

#define INIT_MEMBERS_QtImageSeqView\
    QWidget(parent),\
    data_path_   ("Untitled"),\
    /*id_          (-1),*/\
    image_seq_   (NULL),\
    has_own_data_(false),\
    play_thread_(new PlayThread()),\
    main_vblayout_      (new QVBoxLayout()),\
    mode_btn_layout_    (new QHBoxLayout()),\
    display_mode_layout_(new QHBoxLayout()),\
    time_slider_layout_ (new QHBoxLayout()),\
    image_layout_       (new QVBoxLayout()),\
    display_label_      (new wsp::QtPaintArea()),\
    image_scroll_area_  (new QScrollArea()),\
    color_button_       (new QPushButton(QIcon(COLOR_ICON), QString("Color"))),\
    alpha_button_       (new QPushButton(QIcon(ALPHA_ICON), QString("Alpha"))),\
    state_button_       (new QPushButton(QIcon(WSP_STATE_ICON), QString("State"))),\
    red_checkbox_       (new QCheckBox(QString(R_STR))),\
    green_checkbox_     (new QCheckBox(QString(G_STR))),\
    blue_checkbox_      (new QCheckBox(QString(B_STR))),\
    alpha_mask_checkbox_(new QCheckBox(QString(A_STR))),\
    normalize_checkbox_ (new QCheckBox(QString("Normalize"))),\
    play_button_        (new QPushButton(QIcon(PLAY_ICON), QString(PLAY_STR))),\
    /*stop_button_        (new QPushButton(QIcon(STOP_ICON), QString(STOP_STR))),*/\
    frame_display_      (new QLCDNumber()),\
    time_slider_        (new QSlider()),\
    progress_bar_       (NULL)

void wsp::QtImageSeqView::InitGui()
{
    QString exe_dir_path = QString( WSP_EXE_DIR_PATH );

    color_button_       ->setIcon( QIcon( exe_dir_path + QString( COLOR_ICON ) ) );
    alpha_button_       ->setIcon( QIcon( exe_dir_path + QString( ALPHA_ICON ) ) );
    state_button_       ->setIcon( QIcon( exe_dir_path + QString( WSP_STATE_ICON ) ) );
    play_button_        ->setIcon( QIcon( exe_dir_path + QString(PLAY_ICON) ) );
    //stop_button_        ->setIcon( QIcon( exe_dir_path + QString(STOP_ICON) ) );
    red_checkbox_       ->setIcon( QIcon( exe_dir_path + QString( R_ICON ) ) );
    green_checkbox_     ->setIcon( QIcon( exe_dir_path + QString( G_ICON ) ) );
    blue_checkbox_      ->setIcon( QIcon( exe_dir_path + QString( B_ICON ) ) );
    alpha_mask_checkbox_->setIcon( QIcon( exe_dir_path + QString( A_ICON ) ) );

    play_thread_->img_seq_view_ = this;


    // set image label --------------------------
    display_label_->setBackgroundRole(QPalette::Base);
    display_label_->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    display_label_->setScaledContents(true);
    image_scroll_area_->setBackgroundRole(QPalette::Dark);
    image_scroll_area_->setWidget(display_label_);
    // ------------------------------

    // set time slider --------------------------
    frame_display_->setNumDigits(4);
    time_slider_->setOrientation(Qt::Horizontal);
    time_slider_->setValue(0);
    connect(time_slider_, SIGNAL(valueChanged(int)), frame_display_, SLOT(display(int)));
    connect(time_slider_, SIGNAL(valueChanged(int)), this, SLOT(slot_changeFrame(int)));
    connect(play_button_, SIGNAL(clicked()), this, SLOT(slot_PlayStop()));
    //connect(stop_button_, SIGNAL(clicked()), this, SLOT(slot_stop()));
    // ------------------------------


    // set button and checkbox ------------------------------
    color_button_->setEnabled(false);
    alpha_button_->setEnabled(true);
    state_button_->setEnabled(true);
    connect(color_button_, SIGNAL(clicked()), this, SLOT(slot_showColor()));
    connect(alpha_button_, SIGNAL(clicked()), this, SLOT(slot_showAlpha()));
    connect(state_button_, SIGNAL(clicked()), this, SLOT(slot_showState()));

    red_checkbox_->setChecked(true);
    green_checkbox_->setChecked(true);
    blue_checkbox_->setChecked(true);
    alpha_mask_checkbox_->setChecked(true);
    normalize_checkbox_->setChecked(false);
    connect(red_checkbox_, SIGNAL(clicked()), this, SLOT(slot_showColor()));
    connect(green_checkbox_, SIGNAL(clicked()), this, SLOT(slot_showColor()));
    connect(blue_checkbox_, SIGNAL(clicked()), this, SLOT(slot_showColor()));
    connect(alpha_mask_checkbox_, SIGNAL(clicked()), this, SLOT(slot_showColor()));
    connect(normalize_checkbox_, SIGNAL(clicked()), this, SLOT(slot_showColor()));

    // ------------------------------


    // set gui to layout
    mode_btn_layout_->addWidget(color_button_);
    mode_btn_layout_->addWidget(alpha_button_);
    mode_btn_layout_->addWidget(state_button_);
    mode_btn_layout_->addWidget(red_checkbox_);
    mode_btn_layout_->addWidget(green_checkbox_);
    mode_btn_layout_->addWidget(blue_checkbox_);
    mode_btn_layout_->addWidget(alpha_mask_checkbox_);
    mode_btn_layout_->addWidget(normalize_checkbox_);

    time_slider_layout_->addWidget(frame_display_);
    time_slider_layout_->addWidget(time_slider_);
    time_slider_layout_->addWidget(play_button_);
    //time_slider_layout_->addWidget(stop_button_);

    image_layout_->addWidget(image_scroll_area_);
    image_layout_->addLayout(mode_btn_layout_);
    image_layout_->addLayout(time_slider_layout_);

    main_vblayout_->addLayout(image_layout_);

    this->setLayout(main_vblayout_);


    SynchGuiWithImageSeq();
}

wsp::QtImageSeqView::QtImageSeqView( QWidget *parent )
    : INIT_MEMBERS_QtImageSeqView
{
    WSP_GUI_DEBUG_LOG("start");

    InitGui();
}

wsp::QtImageSeqView::QtImageSeqView(wsp::ImageSeq<SIGNED_IMG_DATA_TYPE> *image_seq, QWidget *parent)
    : INIT_MEMBERS_QtImageSeqView
{
    WSP_GUI_DEBUG_LOG("start");

    //setParent(parent);
    // init data
    image_seq_ = image_seq;
    InitGui();
}

wsp::QtImageSeqView::QtImageSeqView(wsp::Image<SIGNED_IMG_DATA_TYPE> *image, QWidget *parent)
    :INIT_MEMBERS_QtImageSeqView
{
    WSP_GUI_DEBUG_LOG("start");

    image_seq_ = new wsp::ImageSeq<SIGNED_IMG_DATA_TYPE>();
    image_seq_->SetSeqSize(1);
    (*image_seq_)[0] = *image;
    has_own_data_ = true;
    InitGui();
}
wsp::QtImageSeqView::QtImageSeqView(wsp::Sound<SIGNED_IMG_DATA_TYPE> *sound, QWidget *parent)
    : INIT_MEMBERS_QtImageSeqView
{
    WSP_GUI_DEBUG_LOG("start");

    image_seq_ = new wsp::ImageSeq<SIGNED_IMG_DATA_TYPE>();
    image_seq_->SetSeqSize(1);
    (*image_seq_)[0].SetSize(320, 240);
    has_own_data_ = true;

    InitGui();
}
    


wsp::QtImageSeqView::~QtImageSeqView()
{
    WSP_GUI_DEBUG_LOG("start");

    if(has_own_data_)
    {
        delete image_seq_;
    }
    // free gui
    delete display_label_;
    delete image_scroll_area_;

    delete alpha_button_;
    delete color_button_;
    delete state_button_;
    delete red_checkbox_;
    delete green_checkbox_;
    delete blue_checkbox_;
    delete alpha_mask_checkbox_;
    delete normalize_checkbox_;

    delete frame_display_;
    delete time_slider_;
    delete play_button_;
    //delete stop_button_;

    delete display_mode_layout_;
    delete mode_btn_layout_;
    delete time_slider_layout_;
    delete image_layout_;
    delete main_vblayout_;

    delete play_thread_;
}

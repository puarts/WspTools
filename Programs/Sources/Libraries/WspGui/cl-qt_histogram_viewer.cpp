/**
 * @file cl-qt_histogram_viewer.cpp
 * @author Jun-ichi Nishikata
 */

#include <GL/glew.h>
#include "cl-qt_histogram_viewer.hpp"

#define COLOR_NUM 256
#define HISTOGRAM_H 256

#define INIT_WSP_HISTOGRAM_VIEWER \
    img_label_(new QLabel()), \
    channel_label_(new QLabel(QString("channel"))), \
    channel_select_combobox_(new QComboBox()), \
    display_label_(new QLabel(QString("display"))), \
    display_type_combobox_(new QComboBox()), \
    min_label(new QLabel(QString().sprintf("<b>range min</b>: %d", 0))), \
    max_label(new QLabel(QString().sprintf("range max: %d", 255))), \
    graph_max_label_(new QLabel(QString().sprintf("histogram max: %d", 0))), \
    main_glayout_(new QGridLayout()), \
    channel_select_layout_(new QHBoxLayout()), \
    display_type_layout_(new QHBoxLayout()), \
    total_pix_label_(new QLabel(QString().sprintf("total number of pixels: %d", 0)))
    
void wsp::QtHistogramViewer::Init()
{
    setWindowTitle("Histogram");
    this->setFixedSize(QSize(COLOR_NUM+25, HISTOGRAM_H+128));

    // set gui
    const int label_width = 100;
    channel_label_->setFixedWidth(label_width);
    display_label_->setFixedWidth(label_width);

    channel_select_combobox_->addItem(tr("Intensity"));
    channel_select_combobox_->addItem(QString("Red"));
    channel_select_combobox_->addItem(QString("Green"));
    channel_select_combobox_->addItem(QString("Blue"));

    display_type_combobox_->addItem(QString("upper is max"));
    display_type_combobox_->addItem(tr("upper is average"));


    connect(channel_select_combobox_, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_updateHistogram(int)));
    connect(display_type_combobox_, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_updateHistogram(int)));

    // set laytout ---------------------
    channel_select_layout_->addWidget(channel_label_);
    channel_select_layout_->addWidget(channel_select_combobox_);

    display_type_layout_->addWidget(display_label_);
    display_type_layout_->addWidget(display_type_combobox_);

    main_glayout_->addLayout(channel_select_layout_, 0, 0, 1, 2);
    main_glayout_->addLayout(display_type_layout_, 1, 0, 1, 2);
    main_glayout_->addWidget(img_label_, 2, 0, 1, 2);
    main_glayout_->addWidget(min_label, 3, 0, 1, 1, Qt::AlignLeft);
    main_glayout_->addWidget(max_label, 3, 1, 1, 1, Qt::AlignRight);
    main_glayout_->addWidget(total_pix_label_, 4, 0, 1, 1);
    main_glayout_->addWidget(graph_max_label_, 5, 0, 1, 1);
    setLayout(main_glayout_);
}

wsp::QtHistogramViewer::QtHistogramViewer(QWidget *parent)
    : INIT_WSP_HISTOGRAM_VIEWER, QWidget(parent)
{
    Init();
}

wsp::QtHistogramViewer::QtHistogramViewer(wsp::Image<SIGNED_IMG_DATA_TYPE> &image, QWidget *parent)
    : INIT_WSP_HISTOGRAM_VIEWER, QWidget(parent)
{
    Init();
    SetImage(image);
}

wsp::QtHistogramViewer::~QtHistogramViewer()
{
    delete channel_label_;
    delete display_label_;
    delete channel_select_combobox_;
    delete display_type_combobox_;
    delete img_label_;
    delete max_label;
    delete min_label;

    // free layout
    delete channel_select_layout_;
    delete display_type_layout_;
    delete main_glayout_;
}

void wsp::QtHistogramViewer::SetImage(wsp::Image<SIGNED_IMG_DATA_TYPE> &image)
{
    SIGNED_IMG_DATA_TYPE min, max;
    wsp::GetMinMax(image.data(), min, max, image.length()*image.num_of_channels());
    //printf("max=%d, min=%d\n", min, max);
    histogram_data_.SetData(image, min, max);

    min_label->setText(QString().sprintf("range min: %3f", static_cast<float>(min)));
    max_label->setText(QString().sprintf("range max: %3f", static_cast<float>(max)));
    total_pix_label_->setText(QString().sprintf("total number of pixels: %d", image.length()));
    graph_max_label_->setText(QString().sprintf("histogram max: %d", histogram_data_.GetMax()));

    slot_updateHistogram(0);
}

void wsp::QtHistogramViewer::slot_updateHistogram(int)
{
    int channel_combobox_index = channel_select_combobox_->currentIndex();
    int display_combobox_index = display_type_combobox_->currentIndex();
    printf("combobox_index = %d\n", channel_combobox_index);
    int i, j, c, index;
    int dst_nc = 4;
    int src_nc = histogram_data_.num_of_channels();

    wsp::img::ChannelType channel=WSP_CHANNEL_TYPE_SUM_OF_RGB;
    int channel_index = -1;
    switch(channel_combobox_index){
    case 1: channel_index=0; channel=WSP_CHANNEL_TYPE_RED; break;
    case 2: channel_index=1; channel=WSP_CHANNEL_TYPE_GREEN; break;
    case 3: channel_index=2; channel=WSP_CHANNEL_TYPE_BLUE; break;
    default: break;
    }
    if(channel_index>=src_nc){ channel_index=src_nc-1; }

    QImage qimage(COLOR_NUM, HISTOGRAM_H, QImage::Format_RGB32);
    uchar *data_ptr = qimage.bits();
    memset(data_ptr, 0xff, qimage.numBytes()); //qimage.byteCount());
    int max, sum;
    int amount, end;
    int range =static_cast<int>(histogram_data_.range_max() - histogram_data_.range_min());

    // calculate division value for normalization of histogram
    int div;
    switch(display_combobox_index){
    case 1: // upper threshold is average ----------------------
        div = histogram_data_.GetAverage();
        break;
    case 0: // upper threshold is max --------------------------
        if(channel == WSP_CHANNEL_TYPE_SUM_OF_RGB){
            // find max value from sum of value in all channels for each pixels
            sum=0;
            for(c=0; c<src_nc; ++c){
                sum+=histogram_data_[c];
            }
            max=sum;
            for(i=1; i<range; ++i){
                sum=0;
                index = i*src_nc;
                for(c=0; c<src_nc; ++c){
                    sum+=histogram_data_[index+c];
                }
                if(sum>max){ max=sum; }
            }
        }else{
            max = histogram_data_.GetMax(channel_index);
        }
        div = max;
        printf("max value in histogram is %d\n", max);
        break;
    }
    if(div==0){
        WSP_COMMON_ERROR_LOG("max or average is zero\n");
        return;
    }
    
    // draw histogram -----------------------------
    if(channel==WSP_CHANNEL_TYPE_SUM_OF_RGB){
        for(i=0; i<=range; ++i){
            sum = 0;
            index = i*src_nc;
            for(c=0; c<src_nc; ++c){
                sum += histogram_data_[index+c];
            }
            amount = sum*HISTOGRAM_H/div;
            //printf("%d: real data=%d, amount=%d\n", i, sum, amount);
            if(amount<0){
                WSP_COMMON_WARNING_LOG("amount is negative, drawing histogram of intensity %d skipped\n", i);
                continue;
            }
            end = HISTOGRAM_H-1-amount;
            if(end<0){
                //WSP_COMMON_WARNING_LOG("end must be larger than or equal to zero: %d\n", end);
                end=0;
            }
            for(j=HISTOGRAM_H-1; j>end; --j){
                index = (j*COLOR_NUM+i) * dst_nc;
                for(c=0; c<dst_nc; ++c){
                    data_ptr[index+c] = 0;
                }
            }
        }
    }else{
        for(i=0; i<=range; ++i){
            amount = histogram_data_[i*src_nc+channel_index]*HISTOGRAM_H/div;
            //printf("%d: real data=%d, amount=%d\n", i, histogram_data_[i*src_nc+channel_index], amount);
            end = HISTOGRAM_H-1-amount;
            if(end<0){
                //WSP_COMMON_WARNING_LOG("end must be larger than or equal to zero: %d\n", end);
                end=0;
            }
            for(j=HISTOGRAM_H-1; j>end; --j){
                index = (j*COLOR_NUM+i) * dst_nc;
                for(c=0; c<dst_nc; ++c){
                    data_ptr[index+c] = 0;
                }
            }
        }
    }
    printf("nc=%d, channel=%d\n", src_nc, channel_index);
    img_label_->setPixmap(QPixmap().fromImage(qimage.convertToFormat(QImage::Format_ARGB32)));
    // ----------------------------

}

void wsp::QtHistogramViewer::closeEvent( QCloseEvent *e ){ e->accept(); }

/**
 * @file cl-qt_histogram_viewer.hpp
 * @author Jun-ichi Nishikata
 */

#ifndef __WSP_GUI_CLASS_QT_HISTOGRAM_VIEWER_H__
#define __WSP_GUI_CLASS_QT_HISTOGRAM_VIEWER_H__

#include "_define_gui.h"

#include <QtGui/QtGui>

#include <wsp/image/tpl_cl-histogram.h>
#include <wsp/image/tpl_cl-image.h>

#include "cl-qt_abstract_mdisubwin.hpp"



namespace wsp{
    class WSP_DLL_EXPORT QtHistogramViewer
        : public QWidget
    {
        Q_OBJECT
    public:
        QtHistogramViewer(QWidget *parent=NULL);
        QtHistogramViewer(wsp::Image<SIGNED_IMG_DATA_TYPE> &img, QWidget *parent=NULL);
        ~QtHistogramViewer();

        void SetImage(wsp::Image<SIGNED_IMG_DATA_TYPE> &image);

    public slots:
        void slot_updateHistogram(int);

    private:
        virtual void closeEvent(QCloseEvent *e);
        void Init();

    private:
        // data -------------
        wsp::Histogram<SIGNED_IMG_DATA_TYPE> histogram_data_;

        // gui --------------
        // gui data
        QLabel *img_label_;
        QLabel *channel_label_;
        QComboBox *channel_select_combobox_;
        QLabel *display_label_;
        QComboBox *display_type_combobox_;
        QLabel *min_label, *max_label, *total_pix_label_;
        QLabel *graph_max_label_;

        // layout
        QHBoxLayout *channel_select_layout_;
        QHBoxLayout *display_type_layout_;
        QGridLayout *main_glayout_;

    };

    DEFINE_MDI_SUB_WIN_CLASS( QtHistogramViewer );
}

#endif
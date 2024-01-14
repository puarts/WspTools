/**
 * @file cl-qt_image_filter_plugin_action.hpp
 * @author Junichi Nishikata
 */

#ifndef __WSP_GUI_QT_IMAGE_FILTER_PLUGIN_ACTION_H__
#define __WSP_GUI_QT_IMAGE_FILTER_PLUGIN_ACTION_H__


#include "_define_gui.h"

#include <QtGui/QtGui>
#include <wsp/common/_cl-noncopyable.hpp>

#include <wsp/image/tpl_cl-imageseq.h>
#include <wsp/image/tpl_cl-image_filter_plugin_interface.hpp>

namespace{
    template<typename T> class MediaObject;
}

namespace wsp{
    class WSP_DLL_EXPORT QtImageFilterPluginAction
        : public QAction, public wsp::NonCopyable
    {
        Q_OBJECT
    public:
        QtImageFilterPluginAction(QObject *parent=NULL);
        QtImageFilterPluginAction(const QString &text, QObject *parent=NULL);
        ~QtImageFilterPluginAction();

        /* ============================= 
                      Getter 
           =============================*/
        inline int index(){ return index_; }
        inline wsp::ImageFilterPluginInterface<SIGNED_IMG_DATA_TYPE>* plugin_interface(){ 
            return plugin_interface_; 
        }

        /* ============================= 
                      Setter 
           =============================*/
        inline void setImgLabel(QLabel *img_label){
            plugin_interface_->SetImageLabelFrom(img_label);
        }
        inline void setProgressBar(QProgressBar *bar){ progress_bar_ = bar; }
        inline void setImgSeq(wsp::ImageSeq<SIGNED_IMG_DATA_TYPE> *img_seq){ 
            plugin_interface_->SetImageSeqFrom(img_seq);
        }
        inline void setIndex(int index){ index_ = index; }
        inline void setPluginHandler(wsp::ImageFilterPluginInterface<SIGNED_IMG_DATA_TYPE> *plugin_interface){
            plugin_interface_ = plugin_interface;
        }
        inline void setImagePath( const char *src_image_path ){
            plugin_interface_->SetSrcImagePath( src_image_path );
        }
    

    public slots:
        //void slot_applyFilter(wsp::ImageSeq<uchar> &io_img, QLabel &io_img_label);
        void slot_applyFilter();
        void slot_emitIndex();

    signals:
        void signal_emitIndex(int action_index);

    private:
        int index_;
        wsp::ImageFilterPluginInterface<SIGNED_IMG_DATA_TYPE>* plugin_interface_;
        wsp::ImageSeq<SIGNED_IMG_DATA_TYPE>*                   img_seq_;
        QLabel*                                                img_label_;
        QProgressBar*                                          progress_bar_;
    };
}

#endif

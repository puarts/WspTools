/**
 * @file cl-qt_paint_area.hpp
 * @author Jun-ichi Nishikata
 */


#ifndef WSP_GUI_CLASS_QT_PAINT_AREA_H_
#define WSP_GUI_CLASS_QT_PAINT_AREA_H_

#ifndef __WSP_COMMONCORE_DEFINE_COMMONCORE_H__
#include <wsp/common/_define_commoncore.h>
#endif

#include <QtGui/QColor>
#include <QtGui/QImage>
#include <QtGui/QPainterPath>
#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QScrollArea>

#include <wsp/image/tpl_cl-image.h>

class BrushInterface;

namespace wsp{
    class WSP_DLL_EXPORT QtPaintArea
        : public QLabel
    {
        Q_OBJECT
    private:
        wsp::Image<SIGNED_IMG_DATA_TYPE> *image_;
        //QImage theImage;
        QColor color;
        int thickness_;

        BrushInterface *brushInterface;
        QString brush;
        QRect rect_;
        QPoint last_pos_;

        QPainterPath pendingPath;

    private:
        void setupPainter(QPainter &painter);

    public:
        // Constructor -------------------------------
        //wsp::QtPaintArea();
        QtPaintArea(QWidget *parent=NULL);

        // Accessors ---------------------------------
        //QImage image() const { return theImage; }
        QColor brushColor() const { return color; }
        int brushWidth() const { return thickness_; }
        //QSize sizeHint() const;

        // Mutators ----------------------------------
        inline void SetImage(wsp::Image<SIGNED_IMG_DATA_TYPE> *image){
            image_ = image;
        }
        void setImage(const QImage &image);
        void insertShape(const QPainterPath &path);
        void setBrushColor(const QColor &color);
        void setBrushWidth(int width);
        void setBrush(BrushInterface *brushInterface, const QString &brush);


        // I/O ---------------------------------------
        bool openImage(const QString &fileName);
        bool saveImage(const QString &fileName, const char *fileFormat);


    protected:
        void paintEvent(QPaintEvent *event);
        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);


    };


    class QtScrollPaintArea
        : public QScrollArea
    {
    protected:
        wsp::QtPaintArea*image_area_;
        bool load_as_alpha_;

        //QVBoxLayout *main_layout_;

    protected:
        void dragEnterEvent(QDragEnterEvent *event);
        void dragMoveEvent(QDragMoveEvent *event);
        void dragLeaveEvent(QDragLeaveEvent *event);
        void dropEvent(QDropEvent *event);

    public:
        inline QtScrollPaintArea(QWidget *parent=NULL)
            : QScrollArea(parent), load_as_alpha_(false)
        {
            image_area_ = new wsp::QtPaintArea(this);
        }
        inline ~QtScrollPaintArea(){
            delete image_area_;
            //delete main_layout_;
        }

        // accessor
        inline wsp::QtPaintArea* image_area(){
            return image_area_;
        }
    };
}


#endif

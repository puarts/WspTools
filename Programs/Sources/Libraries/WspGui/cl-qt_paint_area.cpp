//! wsp::QtPaintArea.cpp
// @author Jun-ichi Nishikata

#include "_define_gui.h"

#include <wsp/image/gpl_tpl_fn-imgproc.h>
#include "cl-qt_paint_area.hpp"
//#include "interfaces.h>

#include <wsp/image/tpl_cl-painter.h>
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>

#define INIT_WSP_PAINT_AREA \
    image_(NULL),\
    color(Qt::blue),\
    thickness_(3),\
    brushInterface(0),\
    last_pos_(-1, -1)
    //,theImage(500, 400, QImage::Format_RGB32)

//wsp::QtPaintArea::wsp::QtPaintArea() 
//    : QLabel(NULL), INIT_WSP_PAINT_AREA
//{ this->setPixmap(QPixmap(1, 1)); }

wsp::QtPaintArea::QtPaintArea(QWidget *parent) 
    : QLabel(parent), INIT_WSP_PAINT_AREA
{
    //setAttribute(Qt::WA_StaticContents);
    //setAttribute(Qt::WA_NoBackground);

    //theImage.fill(qRgb(255, 255, 255));
    this->setPixmap(QPixmap(1, 1));
    //this->pixmap()->fill(qRgb(255, 255, 255));


}

bool wsp::QtPaintArea::openImage(const QString &fileName)
{
    QImage image;
    if (!image.load(fileName))
        return false;

    //setImage(image);
    this->setPixmap(QPixmap().fromImage(image));
    return true;
}

bool wsp::QtPaintArea::saveImage(const QString &fileName, const char *fileFormat)
{
    return this->pixmap()->toImage().save(fileName, fileFormat);
    //return theImage.save(fileName, fileFormat);
}

void wsp::QtPaintArea::setImage(const QImage &image)
{
    //theImage = image.convertToFormat(QImage::Format_RGB32);
    this->resize(image.width(), image.height());
    this->setPixmap(QPixmap().fromImage(image.convertToFormat(QImage::Format_ARGB32)));
    update();
    updateGeometry();
}

void wsp::QtPaintArea::insertShape(const QPainterPath &path)
{
    pendingPath = path;
#ifndef QT_NO_CURSOR
    setCursor(Qt::CrossCursor);
#endif
}

void wsp::QtPaintArea::setBrushColor(const QColor &color)
{
    this->color = color;
}

void wsp::QtPaintArea::setBrushWidth(int width)
{
    thickness_ = width;
}

//! [0]
void wsp::QtPaintArea::setBrush(BrushInterface *brushInterface, const QString &brush)
{
    this->brushInterface = brushInterface;
    this->brush = brush;
}
//! [0]

//QSize wsp::QtPaintArea::sizeHint() const
//{
//    return theImage.size();
//}

void wsp::QtPaintArea::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);
    painter.drawImage(QPoint(0, 0), (this->pixmap()->toImage()));
}

void wsp::QtPaintArea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        //if(image_!=NULL){
            printf("left button clicked\n");
            //wsp::Painter painter(image_);
            //painter.DrawEclipse(100, 100, 10, 10);
            //QImage img;
            //wsp::img::qtWspImageToQImage<uchar>(img, NULL, *image_);
            //this->setPixmap(QPixmap().fromImage(img));

            QImage paint_img(this->pixmap()->toImage());
            QPainter painter(&paint_img);
            setupPainter(painter);
            painter.drawEllipse (event->pos().x(), event->pos().y(), thickness_, thickness_ );
            this->setPixmap(QPixmap().fromImage(paint_img));

            //update();

            last_pos_ = event->pos();
        //}

//        if (!pendingPath.isEmpty()) {
//            printf("left button clicked\n");
//            QPainter painter(&(this->pixmap()->toImage()));
//            //setupPainter(painter);
//
//            const QRectF boundingRect = pendingPath.boundingRect();
//            QLinearGradient gradient(boundingRect.topRight(),
//                boundingRect.bottomLeft());
//            gradient.setColorAt(0.0, QColor(color.red(), color.green(),
//                color.blue(), 63));
//            gradient.setColorAt(1.0, QColor(color.red(), color.green(),
//                color.blue(), 191));
//            painter.setBrush(gradient);
//            painter.translate(event->pos() - boundingRect.center());
//            painter.drawPath(pendingPath);
//
//            pendingPath = QPainterPath();
//#ifndef QT_NO_CURSOR
//            unsetCursor();
//#endif
//            update();
//        } else {
//            QPainter painter(&(this->pixmap()->toImage()));
//            //setupPainter(painter);
//            //const QRect rect = brushInterface->mousePress(brush, painter,
//            //    event->pos());
//            const QRect rect = QRect(event->pos().x(), event->pos().y(), 3, 3);
//            update(rect);
//
//            last_pos_ = event->pos();
//        }
    }
}

//! [1]
void wsp::QtPaintArea::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && last_pos_ != QPoint(-1, -1)) {
        printf("mouse moved\n");
        QImage paint_img(this->pixmap()->toImage());
        QPainter painter(&paint_img);
        setupPainter(painter);
    //    //const QRect rect = brushInterface->mouseMove(brush, painter, last_pos_,
    //    //    event->pos());
        painter.drawEllipse (event->pos().x(), event->pos().y(), thickness_, thickness_ );
        this->setPixmap(QPixmap().fromImage(paint_img));
        //update(rect);

        last_pos_ = event->pos();
    }
}
//! [1]

void wsp::QtPaintArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && last_pos_ != QPoint(-1, -1)) {
        printf("left button released\n");
        QImage paint_img(this->pixmap()->toImage());
        QPainter painter(&paint_img);
        setupPainter(painter);
    //    //QRect rect = brushInterface->mouseRelease(brush, painter,
    //    //    event->pos());
        painter.drawEllipse (event->pos().x(), event->pos().y(), thickness_, thickness_ );
        this->setPixmap(QPixmap().fromImage(paint_img));
        //update(rect);

        last_pos_ = QPoint(-1, -1);
    }
}

void wsp::QtPaintArea::setupPainter(QPainter &painter)
{
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(color, thickness_, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}


// =================================
// wsp::QtScrollPaintArea
// =================================




void wsp::QtScrollPaintArea::dragEnterEvent(QDragEnterEvent *event){
    //setText(tr("<drop content>"));
    setBackgroundRole(QPalette::Highlight);

    event->acceptProposedAction();
    //emit changed(event->mimeData());
}
void wsp::QtScrollPaintArea::dragMoveEvent(QDragMoveEvent *event){
    event->acceptProposedAction();
}
void wsp::QtScrollPaintArea::dragLeaveEvent(QDragLeaveEvent *event){
    event->accept();
}
void wsp::QtScrollPaintArea::dropEvent(QDropEvent *event){
    const QMimeData *mimeData = event->mimeData();
    char filename[MAX_LENGTH_OF_NAME];
    //char *filename_ptr;

    if(mimeData->hasUrls()==NULL){ WSP_COMMON_ERROR_LOG("URL is NULL\n"); return; }
    QList<QUrl> urlList = mimeData->urls();
    QString urlText = urlList.at(0).path();
    //printf("%s\n", urlText.toLatin1().data());
    //filename_ptr = urlText.toLatin1().data()+1;
    strcpy(filename, urlText.toLatin1().data()+1);
    printf("%s\n", filename);
    char currentDir[MAX_LENGTH_OF_NAME];
    wsp::GetDir(currentDir, filename);
    if(QDir::setCurrent(QString(currentDir))==false){ WSP_COMMON_ERROR_LOG("setCurrent failed\n"); }


    //char extension[MAX_LENGTH_OF_NAME];
    //wsp::GetExtension(extension, filename);
    char extension[MAX_LENGTH_OF_NAME];
    wsp::GetExtension(extension, filename);
    //if(load_as_alpha_==false){
    //    OpenSequence(filename, true);
    //    slot_showAlpha();
    //}else{
    //    if(strcmp(extension, "adri")==0){
    //        img8u_seq_[tabIndex].LoadADRISeq(NULL, filename, 0U, -1, false, true, progress_bar_);
    //    }else{
    //        img8u_seq_[tabIndex].LoadImageSeqAsAlpha(filename, -1, progress_bar_);
    //    }
    //    slot_showColor();
    //}
    event->acceptProposedAction();
}


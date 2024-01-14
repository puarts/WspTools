/**
 * @file cl-qt_render_area.hpp
 * @author Junichi Nisihkata
 */

#ifndef __WSP_GUI_CLASS_QT_RENDERAREA_HPP__
#define __WSP_GUI_CLASS_QT_RENDERAREA_HPP__

#include <QtGui/QBrush>
#include <QtGui/QPen>
#include <QtGui/QPixmap>
#include <QtGui/QWidget>

#include <QtOpenGL/qgl.h>

#ifndef __WSP_COMMONCORE_DEFINE_COMMONCORE_H__
#include <wsp/common/_define_commoncore.h>
#endif

namespace wsp{

    class WSP_DLL_EXPORT QtRenderArea 
        : public QWidget
    {
        Q_OBJECT

    public:
        enum Shape { Line, Points, Polyline, Polygon, Rect, RoundedRect, Ellipse, Arc,
                     Chord, Pie, Path, Text, Pixmap };

        QtRenderArea(QWidget *parent = 0);

        QSize minimumSizeHint() const;
        QSize sizeHint() const;

    public slots:
        void setShape(Shape shape);
        void setPen(const QPen &pen);
        void setBrush(const QBrush &brush);
        void setAntialiased(bool antialiased);
        void setTransformed(bool transformed);

    protected:
        virtual void paintEvent(QPaintEvent *event);

    private:
        Shape shape;
        QPen pen;
        QBrush brush;
        bool antialiased;
        bool transformed;
        QPixmap pixmap;
    };

}

#endif
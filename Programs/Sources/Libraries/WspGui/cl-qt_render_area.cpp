/**
 * @file cl-qt_render_area.cpp
 * @author Junichi Nisihkata
 */

#include <time.h>

#include <QtGui/QtGui>

#include <wsp/math/cl-perlin_noise.h>

#include "cl-qt_render_area.hpp"

#define HEIGHT 2000
#define WIDTH  2000

wsp::QtRenderArea::QtRenderArea(QWidget *parent)
    : QWidget(parent)
{
    shape = Polygon;
    antialiased = false;
    transformed = false;
    pixmap.load(":/images/qt-logo.png");

    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

QSize wsp::QtRenderArea::minimumSizeHint() const
{
    return QSize(100, 100);
}

QSize wsp::QtRenderArea::sizeHint() const
{
    return QSize(WIDTH, HEIGHT);
}

void wsp::QtRenderArea::setShape(Shape shape)
{
    this->shape = shape;
    update();
}

void wsp::QtRenderArea::setPen(const QPen &pen)
{
    this->pen = pen;
    update();
}

void wsp::QtRenderArea::setBrush(const QBrush &brush)
{
    this->brush = brush;
    update();
}

void wsp::QtRenderArea::setAntialiased(bool antialiased)
{
    this->antialiased = antialiased;
    update();
}

void wsp::QtRenderArea::setTransformed(bool transformed)
{
    this->transformed = transformed;
    update();
}

void wsp::QtRenderArea::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);
    painter.setPen(pen);
    painter.setBrush(brush);
    if (antialiased)
    {
        painter.setRenderHint(QPainter::Antialiasing, true);
    }

    {
        painter.setBackground( QBrush(QColor(150, 150, 150 )) );

        wsp::PerlinNoise pn( time(NULL) );
        srand(time(NULL));
        float xstart = (rand()/(float)RAND_MAX) * 10.0f;
        float ynoise = (rand()/(float)RAND_MAX) * 10.0f;

        painter.translate( WIDTH/2, HEIGHT/2 );

        for( float y = -(HEIGHT/8); y<=(HEIGHT/8); y+=3 )
        {
            ynoise += 0.02;
            float xnoise = xstart;
            for( float x = -(WIDTH/8); x<=(WIDTH/8); x+=3 )
            {
                xnoise += 0.02;

                /* draw point */
                {
                    float noise_factor = pn.noise( xnoise, ynoise, 0.0 );
                    //printf("noise_factor = %f\n", noise_factor );
                    painter.translate( x*noise_factor*4, y*noise_factor*4 );
                    float edge_size = noise_factor * 26;
                    painter.scale( ((HEIGHT/4)-y)/(HEIGHT/4), ((HEIGHT/4)-y)/(HEIGHT/4));
                    painter.drawEllipse( 0, 0, edge_size, edge_size );

                }
            }
            painter.drawEllipse( QPoint(50, 50), 10, 10 );
            printf("%f %%\n", (y+(HEIGHT/8))/(float)(HEIGHT/4));
            fflush(stdout);
        }
    }

    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setPen(palette().dark().color());
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));
}

/**
 * @file cl-qt_comp_layer_view.cpp
 * @author Junichi Nishikata
 */

#include "cl-qt_slider_delegate.hpp"






wsp::QtSliderDelegate::QtSliderDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}


void wsp::QtSliderDelegate::paint(
    QPainter *painter, 
    const QStyleOptionViewItem &option,
    const QModelIndex &index
) const
{
    wsp::LayerTimeline timeline = qVariantValue<wsp::LayerTimeline>(index.data());

    if (option.state & QStyle::State_Selected)
    {
        painter->fillRect(option.rect, option.palette.highlight());
    }

    timeline.paint( painter, option.rect, option.palette, wsp::LayerTimeline::ReadOnly );
}

QSize wsp::QtSliderDelegate::sizeHint(
    const QStyleOptionViewItem &option,
    const QModelIndex &index) const
{
    wsp::LayerTimeline timeline = qVariantValue<wsp::LayerTimeline>(index.data());
    return timeline.sizeHint();
}

QWidget* wsp::QtSliderDelegate::createEditor(
    QWidget *parent,
    const QStyleOptionViewItem &option,
    const QModelIndex &index 
) const
{
    wsp::TimeLineEditor *editor = new wsp::TimeLineEditor(parent);
    connect(editor, SIGNAL(editingFinished()),
            this,   SLOT(slot_commitAndCloseEditor()));
    connect(editor, SIGNAL( signal_valueChanged( int )),
            this,   SLOT( slot_valueChanged( int ) ) );
    return editor;

}

void wsp::QtSliderDelegate::setEditorData(
    QWidget *editor, 
    const QModelIndex &index
) const
{
    wsp::LayerTimeline timeline = qVariantValue<wsp::LayerTimeline>(index.data());
    wsp::TimeLineEditor *starEditor = qobject_cast<wsp::TimeLineEditor *>(editor);
    starEditor->setLayerTimeline(timeline);
}

void wsp::QtSliderDelegate::setModelData(
    QWidget *editor, 
    QAbstractItemModel *model,
    const QModelIndex &index
) const
{
    wsp::TimeLineEditor *starEditor = qobject_cast<wsp::TimeLineEditor *>(editor);
    model->setData(index, qVariantFromValue(starEditor->timeline()));
}


void wsp::QtSliderDelegate::slot_commitAndCloseEditor()
{
    WSP_COMMON_DEBUG_LOG("\n");
    wsp::TimeLineEditor *editor = qobject_cast<wsp::TimeLineEditor *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);

}

void wsp::QtSliderDelegate::slot_valueChanged( int value )
{
    WSP_COMMON_DEBUG_LOG("value=%d\n", value);
    emit signal_valueChanged( value );
}





wsp::TimeLineEditor::TimeLineEditor(QWidget *parent)
    : QSlider(parent)
{
    setMouseTracking(true);
    setAutoFillBackground(true);
}

QSize wsp::TimeLineEditor::sizeHint() const
{
    return timeline_.sizeHint();
}

void wsp::TimeLineEditor::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    timeline_.paint(&painter, rect(), this->palette(), wsp::LayerTimeline::Editable);
    emit signal_valueChanged( timeline_.value() );
}

void wsp::TimeLineEditor::mouseMoveEvent(QMouseEvent *event)
{
    printf("event->x=%d\n", event->x());
    int value = valueAtPosition(event->x());

    if (value != timeline_.value() && value != -1) 
    {
        timeline_.set_value(value);
        //timeline_.set_value( frame_offset );
        update();
    }
}

void wsp::TimeLineEditor::mouseReleaseEvent(QMouseEvent * /* event */)
{
    emit editingFinished();
}

int wsp::TimeLineEditor::valueAtPosition(int x)
{
    s32 width = timeline_.sizeHint().width();
    s32 range = timeline_.max_value();
    int value = ( 10 * x / ( width/range )) ;
    if (value <= 0 || value > timeline_.max_value())
    {
        return -1;
    }

    return value;
}




const int PaintingScaleFactor = 20;


wsp::LayerTimeline::LayerTimeline(int value, int maxStarCount, QWidget *parent)
{
    value_ = value;
    max_value_ = maxStarCount;

    //starPolygon << QPointF(1.0, 0.5);
    //for (int i = 1; i < 5; ++i)
    //{
    //    starPolygon << QPointF(0.5 + 0.5 * cos(0.8 * i * 3.14),
    //                        0.5 + 0.5 * sin(0.8 * i * 3.14));
    //}
    float rect_w = 0.5;
    starPolygon << QPointF(0.0, 0.0) << QPointF(0.0, 1.0) << QPointF(rect_w, 1.0) << QPointF(0.0, 0.0)
                 << QPointF(rect_w, 0.0) << QPointF(rect_w, 1.0) << QPointF(0.0, 0.0);

    diamondPolygon << QPointF(0.4, 0.5) << QPointF(0.5, 0.4)
                << QPointF(0.6, 0.5) << QPointF(0.5, 0.6)
                << QPointF(0.4, 0.5);
}

QSize wsp::LayerTimeline::sizeHint() const
{
    return PaintingScaleFactor * QSize(max_value_, 1);
}

void wsp::LayerTimeline::paint(
    QPainter *painter, 
    const QRect &rect,
    const QPalette &palette, 
    EditMode mode
) const
{
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(Qt::NoPen);

    if (mode == Editable) 
    {
        painter->setBrush(palette.highlight());
    } 
    else 
    {
        painter->setBrush(palette.foreground());
    }

    int yOffset = (rect.height()-PaintingScaleFactor) / 2;
    painter->translate(rect.x() - 5.0, rect.y() + yOffset);
    painter->scale(PaintingScaleFactor, PaintingScaleFactor);

    for (int i = 0; i < max_value_; ++i) 
    {
        if ( i == value_ ) 
        {
            painter->drawPolygon(starPolygon, Qt::WindingFill);
        } 
        else
        {
            painter->drawPolygon(diamondPolygon, Qt::WindingFill);
        }
        painter->translate(0.1, 0.0);
    }

    painter->restore();
}





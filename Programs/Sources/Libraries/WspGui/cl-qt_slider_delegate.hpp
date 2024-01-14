/**
 * @file cl-qt_slider_delegate.hpp
 * @author Junichi Nishikata
 */

#ifndef WSP_GUI_CLASS_QT_SLIDER_DELEGATE_HPP__
#define WSP_GUI_CLASS_QT_SLIDER_DELEGATE_HPP__

#include <QtGui/QtGui>
#include "_define_gui.h"


namespace wsp{

    class LayerTimeline
    {
    public:
        enum EditMode { Editable, ReadOnly };

        LayerTimeline(int value = 1, int max_value = 100, QWidget *parent=NULL);

        void paint(QPainter *painter, const QRect &rect, const QPalette &palette, EditMode mode) const;
        QSize sizeHint() const;

        int  value()    const              { return value_; }
        int  max_value() const              { return max_value_; }
        void set_value(int value)       { value_ = value; }
        void set_max_value(int max_value) { max_value_ = max_value; }


    private:
        QPolygonF starPolygon;
        QPolygonF diamondPolygon;
        int value_;
        int max_value_;
    };

    Q_DECLARE_METATYPE(LayerTimeline)




    class TimeLineEditor 
        : public QSlider
    {
        Q_OBJECT

    public:
        TimeLineEditor(QWidget *parent = 0);

        QSize sizeHint() const;
        void setLayerTimeline(const LayerTimeline &timeline) {
            timeline_ = timeline;
        }
        LayerTimeline timeline() { return timeline_; }

    signals:
        void editingFinished();
        void signal_valueChanged( int value );


    protected:
        void paintEvent(QPaintEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);

    private:
        int valueAtPosition(int x);

        LayerTimeline timeline_;
    };



    class WSP_DLL_EXPORT QtSliderDelegate 
        : public QStyledItemDelegate
    {
        Q_OBJECT

    public:
        QtSliderDelegate(QObject *parent = 0);


        QWidget *createEditor(
                QWidget *parent, 
                const QStyleOptionViewItem &option,
                const QModelIndex &index
            ) const;

        void setEditorData(QWidget *editor, const QModelIndex &index) const;
        void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
        QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;


    signals:
        void signal_valueChanged( int value );

    private slots:
        void slot_commitAndCloseEditor();
        void slot_valueChanged( int value );
    };


}







#endif

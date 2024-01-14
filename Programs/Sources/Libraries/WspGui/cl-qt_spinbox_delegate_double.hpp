/**
 * @file cl-qt_comp_layer_view.hpp
 * @author Junichi Nishikata
 */

#ifndef WSP_GUI_CLASS_QT_SPINBOX_DELEGATE_DOUBLE_HPP__
#define WSP_GUI_CLASS_QT_SPINBOX_DELEGATE_DOUBLE_HPP__

#include <QtGui/QtGui>
#include "_define_gui.h"

namespace wsp{

    class WSP_DLL_EXPORT QtSpinBoxDelegateDouble 
        : public QItemDelegate
    {
        Q_OBJECT

    public:
        QtSpinBoxDelegateDouble( QObject *parent = 0 );
        QtSpinBoxDelegateDouble( const QtSpinBoxDelegateDouble &item_delegate );

        /* getter */
        double min_value() const{ return min_value_; }
        double max_value() const{ return max_value_; }

        /* setter */
        void set_min_value( double min_value ){ min_value_ = min_value; }
        void set_max_value( double max_value ){ max_value_ = max_value; }

        QtSpinBoxDelegateDouble& operator=(const QtSpinBoxDelegateDouble& other)
        {
            min_value_ = other.min_value();
            max_value_ = other.max_value();
            return *this;
        }

        /* inherited methods */
        virtual QWidget *createEditor(
                            QWidget *parent, 
                            const QStyleOptionViewItem &option,
                            const QModelIndex &index
                        ) const;
        virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
        virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
        virtual void updateEditorGeometry(
                            QWidget *editor,
                            const QStyleOptionViewItem &option, 
                            const QModelIndex &index
                        ) const;

    signals:
        void signal_valueChanged( double value );

    private slots:
        void slot_valueChanged( double value );

    private:
        double min_value_;
        double max_value_;
    };


}


#endif

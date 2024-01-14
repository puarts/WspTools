/**
 * @file cl-qt_comp_layer_view.hpp
 * @author Junichi Nishikata
 */

#ifndef WSP_GUI_CLASS_QT_SPINBOX_DELEGATE_HPP__
#define WSP_GUI_CLASS_QT_SPINBOX_DELEGATE_HPP__

#include <QtGui/QtGui>
#include "_define_gui.h"

namespace wsp{

    class WSP_DLL_EXPORT QtSpinBoxDelegateInt 
        : public QItemDelegate
    {
        Q_OBJECT

    public:
        QtSpinBoxDelegateInt(QObject *parent = 0);
        QtSpinBoxDelegateInt( const QtSpinBoxDelegateInt &item_delegate );

        /* getter */
        int min_value() const{ return min_value_; }
        int max_value() const{ return max_value_; }

        /* setter */
        void set_min_value( int min_value ){ min_value_ = min_value; }
        void set_max_value( int max_value ){ max_value_ = max_value; }

        QtSpinBoxDelegateInt& operator=(const QtSpinBoxDelegateInt& other)
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
        void signal_valueChanged( int value );

    private slots:
        void slot_valueChanged( int value );

    private:
        int min_value_;
        int max_value_;
    };


}


#endif

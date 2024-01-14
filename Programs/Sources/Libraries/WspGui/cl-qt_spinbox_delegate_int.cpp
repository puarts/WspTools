/**
 * @file cl-qt_comp_layer_view.cpp
 * @author Junichi Nishikata
 */

#include "cl-qt_spinbox_delegate_int.hpp"


wsp::QtSpinBoxDelegateInt::QtSpinBoxDelegateInt(QObject *parent)
    : QItemDelegate(parent)
{
}

wsp::QtSpinBoxDelegateInt::QtSpinBoxDelegateInt( const QtSpinBoxDelegateInt &item_delegate )
{
    min_value_ = item_delegate.min_value();
    max_value_ = item_delegate.max_value();
}

QWidget *wsp::QtSpinBoxDelegateInt::createEditor(
    QWidget *parent,
    const QStyleOptionViewItem &option,
    const QModelIndex &index 
) const
{
    WSP_COMMON_DEBUG_LOG( "\n" );
    QSpinBox *editor = new QSpinBox(parent);
    editor->setMinimum(min_value_);
    editor->setMaximum(max_value_);

    connect(
        editor, SIGNAL( valueChanged( int )),
        this,   SLOT( slot_valueChanged( int ) ) 
    );

    return editor;
}

void wsp::QtSpinBoxDelegateInt::setEditorData(
    QWidget *editor, 
    const QModelIndex &index
) const
{
    WSP_COMMON_DEBUG_LOG( "\n" );
    int value = index.model()->data(index, Qt::EditRole).toInt();

    QSpinBox *spinbox = static_cast<QSpinBox*>(editor);
    spinbox->setValue(value);
}

void wsp::QtSpinBoxDelegateInt::setModelData(
    QWidget *editor, 
    QAbstractItemModel *model,
    const QModelIndex &index
) const
{
    WSP_COMMON_DEBUG_LOG( "\n" );
    QSpinBox *spinbox = static_cast<QSpinBox*>(editor);
    spinbox->interpretText();
    int value = spinbox->value();

    model->setData(index, value, Qt::EditRole);
}

void wsp::QtSpinBoxDelegateInt::updateEditorGeometry(
    QWidget *editor,
    const QStyleOptionViewItem &option, 
    const QModelIndex &/* index */
) const
{
    WSP_COMMON_DEBUG_LOG( "\n" );
    editor->setGeometry(option.rect);
}

void wsp::QtSpinBoxDelegateInt::slot_valueChanged( int value )
{
    WSP_COMMON_DEBUG_LOG("value=%d\n", value);
    emit signal_valueChanged( value );
}

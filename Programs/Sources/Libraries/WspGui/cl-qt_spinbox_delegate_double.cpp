/**
 * @file cl-qt_comp_layer_view.cpp
 * @author Junichi Nishikata
 */

#include "cl-qt_spinbox_delegate_double.hpp"


wsp::QtSpinBoxDelegateDouble::QtSpinBoxDelegateDouble(QObject *parent)
    : QItemDelegate(parent)
{
}

wsp::QtSpinBoxDelegateDouble::QtSpinBoxDelegateDouble( const QtSpinBoxDelegateDouble &item_delegate )
{
    min_value_ = item_delegate.min_value();
    max_value_ = item_delegate.max_value();
}


QWidget *wsp::QtSpinBoxDelegateDouble::createEditor(
    QWidget *parent,
    const QStyleOptionViewItem &option,
    const QModelIndex &index 
) const
{
    WSP_COMMON_DEBUG_LOG( 
        "\n"
        "address %08x\n"
        "    min_value_ = %f\n" 
        "    max_value_ = %f\n" 
        , this, min_value_, max_value_
    );
    QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
    editor->setMinimum(min_value_);
    editor->setMaximum(max_value_);

    connect(
        editor, SIGNAL( valueChanged( double )),
        this,   SLOT( slot_valueChanged( double ) ) 
    );

    return editor;
}

void wsp::QtSpinBoxDelegateDouble::setEditorData(
    QWidget *editor, 
    const QModelIndex &index
) const
{
    WSP_COMMON_DEBUG_LOG( "\n" );
    int value = index.model()->data(index, Qt::EditRole).toInt();

    QDoubleSpinBox *spinbox = static_cast<QDoubleSpinBox*>(editor);
    spinbox->setValue(value);
}

void wsp::QtSpinBoxDelegateDouble::setModelData(
    QWidget *editor, 
    QAbstractItemModel *model,
    const QModelIndex &index
) const
{
    WSP_COMMON_DEBUG_LOG( "\n" );
    QDoubleSpinBox *spinbox = static_cast<QDoubleSpinBox*>(editor);
    spinbox->interpretText();
    double value = spinbox->value();

    model->setData(index, value, Qt::EditRole);
}

void wsp::QtSpinBoxDelegateDouble::updateEditorGeometry(
    QWidget *editor,
    const QStyleOptionViewItem &option, 
    const QModelIndex &/* index */
) const
{
    WSP_COMMON_DEBUG_LOG( "\n" );
    editor->setGeometry(option.rect);
}

void wsp::QtSpinBoxDelegateDouble::slot_valueChanged( double value )
{
    WSP_COMMON_DEBUG_LOG("value=%f\n", value);
    emit signal_valueChanged( value );
}

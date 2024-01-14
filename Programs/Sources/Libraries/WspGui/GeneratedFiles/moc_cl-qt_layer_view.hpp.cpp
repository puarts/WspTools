/****************************************************************************
** Meta object code from reading C++ file 'cl-qt_layer_view.hpp'
**
** Created: Thu Oct 23 01:04:55 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../cl-qt_layer_view.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cl-qt_layer_view.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_WspLayerView[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_WspLayerView[] = {
    "WspLayerView\0"
};

const QMetaObject WspLayerView::staticMetaObject = {
    { &QTreeWidget::staticMetaObject, qt_meta_stringdata_WspLayerView,
      qt_meta_data_WspLayerView, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &WspLayerView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *WspLayerView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *WspLayerView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_WspLayerView))
        return static_cast<void*>(const_cast< WspLayerView*>(this));
    return QTreeWidget::qt_metacast(_clname);
}

int WspLayerView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTreeWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE

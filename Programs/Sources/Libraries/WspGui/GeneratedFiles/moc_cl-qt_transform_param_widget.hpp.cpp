/****************************************************************************
** Meta object code from reading C++ file 'cl-qt_transform_param_widget.hpp'
**
** Created: Thu Oct 23 01:04:56 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../cl-qt_transform_param_widget.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cl-qt_transform_param_widget.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_wsp__QtTransformParamWidget[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      29,   28,   28,   28, 0x05,
      51,   28,   28,   28, 0x05,

 // slots: signature, parameters, type, tag, flags
      78,   72,   28,   28, 0x08,
     108,   72,   28,   28, 0x08,
     138,   72,   28,   28, 0x08,
     168,   28,   28,   28, 0x08,
     191,  187,   28,   28, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_wsp__QtTransformParamWidget[] = {
    "wsp::QtTransformParamWidget\0\0"
    "signal_valueUpdated()\0signal_keySelected()\0"
    "value\0slot_updateDestValue0(double)\0"
    "slot_updateDestValue1(double)\0"
    "slot_updateDestValue2(double)\0"
    "slot_keySelected()\0pos\0"
    "slot_showContextMenu(QPoint)\0"
};

const QMetaObject wsp::QtTransformParamWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_wsp__QtTransformParamWidget,
      qt_meta_data_wsp__QtTransformParamWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &wsp::QtTransformParamWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *wsp::QtTransformParamWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *wsp::QtTransformParamWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_wsp__QtTransformParamWidget))
        return static_cast<void*>(const_cast< QtTransformParamWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int wsp::QtTransformParamWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: signal_valueUpdated(); break;
        case 1: signal_keySelected(); break;
        case 2: slot_updateDestValue0((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: slot_updateDestValue1((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: slot_updateDestValue2((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 5: slot_keySelected(); break;
        case 6: slot_showContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void wsp::QtTransformParamWidget::signal_valueUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void wsp::QtTransformParamWidget::signal_keySelected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE

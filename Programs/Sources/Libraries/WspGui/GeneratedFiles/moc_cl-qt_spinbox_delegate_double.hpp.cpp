/****************************************************************************
** Meta object code from reading C++ file 'cl-qt_spinbox_delegate_double.hpp'
**
** Created: Thu Oct 23 01:04:56 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../cl-qt_spinbox_delegate_double.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cl-qt_spinbox_delegate_double.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_wsp__QtSpinBoxDelegateDouble[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      36,   30,   29,   29, 0x05,

 // slots: signature, parameters, type, tag, flags
      64,   30,   29,   29, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_wsp__QtSpinBoxDelegateDouble[] = {
    "wsp::QtSpinBoxDelegateDouble\0\0value\0"
    "signal_valueChanged(double)\0"
    "slot_valueChanged(double)\0"
};

const QMetaObject wsp::QtSpinBoxDelegateDouble::staticMetaObject = {
    { &QItemDelegate::staticMetaObject, qt_meta_stringdata_wsp__QtSpinBoxDelegateDouble,
      qt_meta_data_wsp__QtSpinBoxDelegateDouble, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &wsp::QtSpinBoxDelegateDouble::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *wsp::QtSpinBoxDelegateDouble::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *wsp::QtSpinBoxDelegateDouble::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_wsp__QtSpinBoxDelegateDouble))
        return static_cast<void*>(const_cast< QtSpinBoxDelegateDouble*>(this));
    return QItemDelegate::qt_metacast(_clname);
}

int wsp::QtSpinBoxDelegateDouble::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QItemDelegate::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: signal_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: slot_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void wsp::QtSpinBoxDelegateDouble::signal_valueChanged(double _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE

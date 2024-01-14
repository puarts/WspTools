/****************************************************************************
** Meta object code from reading C++ file 'cl-qt_unit_param_widget.hpp'
**
** Created: Thu Oct 23 01:04:56 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../cl-qt_unit_param_widget.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cl-qt_unit_param_widget.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_wsp__QtUnitParamWidget[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      57,   24,   23,   23, 0x05,
     122,   90,   23,   23, 0x05,
     191,  158,   23,   23, 0x05,

 // slots: signature, parameters, type, tag, flags
     233,  227,   23,   23, 0x0a,
     260,   23,   23,   23, 0x0a,
     289,  283,   23,   23, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_wsp__QtUnitParamWidget[] = {
    "wsp::QtUnitParamWidget\0\0"
    "value,param_id,parent_process_id\0"
    "signal_valueChanged(int,int,int)\0"
    "text,param_id,parent_process_id\0"
    "signal_textChanged(QString,int,int)\0"
    "index,param_id,parent_process_id\0"
    "signal_comboBoxChanged(int,int,int)\0"
    "value\0slot_emitValueChanged(int)\0"
    "slot_emitTextChanged()\0index\0"
    "slot_emitComboBoxChanged(int)\0"
};

const QMetaObject wsp::QtUnitParamWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_wsp__QtUnitParamWidget,
      qt_meta_data_wsp__QtUnitParamWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &wsp::QtUnitParamWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *wsp::QtUnitParamWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *wsp::QtUnitParamWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_wsp__QtUnitParamWidget))
        return static_cast<void*>(const_cast< QtUnitParamWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int wsp::QtUnitParamWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: signal_valueChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: signal_textChanged((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 2: signal_comboBoxChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 3: slot_emitValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: slot_emitTextChanged(); break;
        case 5: slot_emitComboBoxChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void wsp::QtUnitParamWidget::signal_valueChanged(int _t1, int _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void wsp::QtUnitParamWidget::signal_textChanged(QString _t1, int _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void wsp::QtUnitParamWidget::signal_comboBoxChanged(int _t1, int _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE

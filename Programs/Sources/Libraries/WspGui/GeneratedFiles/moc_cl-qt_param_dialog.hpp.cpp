/****************************************************************************
** Meta object code from reading C++ file 'cl-qt_param_dialog.hpp'
**
** Created: Thu Oct 23 01:04:55 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../cl-qt_param_dialog.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cl-qt_param_dialog.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_wsp__QtParamDialog[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      20,   19,   19,   19, 0x09,
      33,   19,   19,   19, 0x09,
      48,   19,   19,   19, 0x09,
      63,   19,   19,   19, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_wsp__QtParamDialog[] = {
    "wsp::QtParamDialog\0\0slot_start()\0"
    "slot_loadXml()\0slot_saveXml()\0"
    "slot_estimateParams()\0"
};

const QMetaObject wsp::QtParamDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_wsp__QtParamDialog,
      qt_meta_data_wsp__QtParamDialog, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &wsp::QtParamDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *wsp::QtParamDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *wsp::QtParamDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_wsp__QtParamDialog))
        return static_cast<void*>(const_cast< QtParamDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int wsp::QtParamDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: slot_start(); break;
        case 1: slot_loadXml(); break;
        case 2: slot_saveXml(); break;
        case 3: slot_estimateParams(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'cl-qt_abstract_mdisubwin.hpp'
**
** Created: Thu Oct 23 01:04:54 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../cl-qt_abstract_mdisubwin.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cl-qt_abstract_mdisubwin.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_wsp__QtAbstractMdiSubWin[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      26,   25,   25,   25, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_wsp__QtAbstractMdiSubWin[] = {
    "wsp::QtAbstractMdiSubWin\0\0"
    "signal_destroyed(s32)\0"
};

const QMetaObject wsp::QtAbstractMdiSubWin::staticMetaObject = {
    { &QMdiSubWindow::staticMetaObject, qt_meta_stringdata_wsp__QtAbstractMdiSubWin,
      qt_meta_data_wsp__QtAbstractMdiSubWin, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &wsp::QtAbstractMdiSubWin::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *wsp::QtAbstractMdiSubWin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *wsp::QtAbstractMdiSubWin::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_wsp__QtAbstractMdiSubWin))
        return static_cast<void*>(const_cast< QtAbstractMdiSubWin*>(this));
    return QMdiSubWindow::qt_metacast(_clname);
}

int wsp::QtAbstractMdiSubWin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMdiSubWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: signal_destroyed((*reinterpret_cast< s32(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void wsp::QtAbstractMdiSubWin::signal_destroyed(s32 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_wsp__QtAbstractDialog[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      23,   22,   22,   22, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_wsp__QtAbstractDialog[] = {
    "wsp::QtAbstractDialog\0\0signal_destroyed(s32)\0"
};

const QMetaObject wsp::QtAbstractDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_wsp__QtAbstractDialog,
      qt_meta_data_wsp__QtAbstractDialog, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &wsp::QtAbstractDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *wsp::QtAbstractDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *wsp::QtAbstractDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_wsp__QtAbstractDialog))
        return static_cast<void*>(const_cast< QtAbstractDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int wsp::QtAbstractDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: signal_destroyed((*reinterpret_cast< s32(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void wsp::QtAbstractDialog::signal_destroyed(s32 _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE

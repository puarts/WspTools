/****************************************************************************
** Meta object code from reading C++ file 'cl-qt_filepath_line.hpp'
**
** Created: Thu Oct 23 01:04:54 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../cl-qt_filepath_line.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cl-qt_filepath_line.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_wsp__QtFilepathLine[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      21,   20,   20,   20, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_wsp__QtFilepathLine[] = {
    "wsp::QtFilepathLine\0\0browse()\0"
};

const QMetaObject wsp::QtFilepathLine::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_wsp__QtFilepathLine,
      qt_meta_data_wsp__QtFilepathLine, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &wsp::QtFilepathLine::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *wsp::QtFilepathLine::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *wsp::QtFilepathLine::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_wsp__QtFilepathLine))
        return static_cast<void*>(const_cast< QtFilepathLine*>(this));
    return QWidget::qt_metacast(_clname);
}

int wsp::QtFilepathLine::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: browse(); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'cl-qt_render_area.hpp'
**
** Created: Thu Oct 23 01:04:55 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../cl-qt_render_area.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cl-qt_render_area.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_wsp__QtRenderArea[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      25,   19,   18,   18, 0x0a,
      45,   41,   18,   18, 0x0a,
      64,   58,   18,   18, 0x0a,
      93,   81,   18,   18, 0x0a,
     126,  114,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_wsp__QtRenderArea[] = {
    "wsp::QtRenderArea\0\0shape\0setShape(Shape)\0"
    "pen\0setPen(QPen)\0brush\0setBrush(QBrush)\0"
    "antialiased\0setAntialiased(bool)\0"
    "transformed\0setTransformed(bool)\0"
};

const QMetaObject wsp::QtRenderArea::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_wsp__QtRenderArea,
      qt_meta_data_wsp__QtRenderArea, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &wsp::QtRenderArea::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *wsp::QtRenderArea::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *wsp::QtRenderArea::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_wsp__QtRenderArea))
        return static_cast<void*>(const_cast< QtRenderArea*>(this));
    return QWidget::qt_metacast(_clname);
}

int wsp::QtRenderArea::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setShape((*reinterpret_cast< Shape(*)>(_a[1]))); break;
        case 1: setPen((*reinterpret_cast< const QPen(*)>(_a[1]))); break;
        case 2: setBrush((*reinterpret_cast< const QBrush(*)>(_a[1]))); break;
        case 3: setAntialiased((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: setTransformed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

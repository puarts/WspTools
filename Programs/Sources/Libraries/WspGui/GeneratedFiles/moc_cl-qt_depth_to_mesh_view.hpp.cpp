/****************************************************************************
** Meta object code from reading C++ file 'cl-qt_depth_to_mesh_view.hpp'
**
** Created: Thu Oct 23 01:04:54 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../cl-qt_depth_to_mesh_view.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cl-qt_depth_to_mesh_view.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_wsp__QtDepthToMeshView[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      30,   24,   23,   23, 0x08,
      47,   24,   23,   23, 0x08,
      69,   24,   23,   23, 0x08,
      88,   24,   23,   23, 0x08,
     115,   24,   23,   23, 0x08,
     138,   24,   23,   23, 0x08,
     164,   24,   23,   23, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_wsp__QtDepthToMeshView[] = {
    "wsp::QtDepthToMeshView\0\0value\0"
    "slot_setFov(int)\0slot_setNearClip(int)\0"
    "slot_setScale(int)\0slot_setFaceDepthThre(int)\0"
    "slot_setAlphaThre(int)\0slot_setSamplingStep(int)\0"
    "slot_setInvert(int)\0"
};

const QMetaObject wsp::QtDepthToMeshView::staticMetaObject = {
    { &wsp::QtGLView::staticMetaObject, qt_meta_stringdata_wsp__QtDepthToMeshView,
      qt_meta_data_wsp__QtDepthToMeshView, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &wsp::QtDepthToMeshView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *wsp::QtDepthToMeshView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *wsp::QtDepthToMeshView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_wsp__QtDepthToMeshView))
        return static_cast<void*>(const_cast< QtDepthToMeshView*>(this));
    typedef wsp::QtGLView QMocSuperClass;
    return QMocSuperClass::qt_metacast(_clname);
}

int wsp::QtDepthToMeshView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    typedef wsp::QtGLView QMocSuperClass;
    _id = QMocSuperClass::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: slot_setFov((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: slot_setNearClip((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: slot_setScale((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: slot_setFaceDepthThre((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: slot_setAlphaThre((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: slot_setSamplingStep((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: slot_setInvert((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

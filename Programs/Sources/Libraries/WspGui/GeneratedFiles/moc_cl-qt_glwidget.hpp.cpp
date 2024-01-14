/****************************************************************************
** Meta object code from reading C++ file 'cl-qt_glwidget.hpp'
**
** Created: Thu Oct 23 01:04:54 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../cl-qt_glwidget.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cl-qt_glwidget.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_wsp__QtMaterialView[] = {

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

static const char qt_meta_stringdata_wsp__QtMaterialView[] = {
    "wsp::QtMaterialView\0"
};

const QMetaObject wsp::QtMaterialView::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_wsp__QtMaterialView,
      qt_meta_data_wsp__QtMaterialView, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &wsp::QtMaterialView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *wsp::QtMaterialView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *wsp::QtMaterialView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_wsp__QtMaterialView))
        return static_cast<void*>(const_cast< QtMaterialView*>(this));
    return QWidget::qt_metacast(_clname);
}

int wsp::QtMaterialView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_wsp__QtGLWidget[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      25,   17,   16,   16, 0x0a,
      47,   17,   16,   16, 0x0a,
      67,   16,   16,   16, 0x0a,
      83,   16,   16,   16, 0x0a,
     109,  104,   16,   16, 0x0a,
     137,   16,   16,   16, 0x0a,
     154,   16,   16,   16, 0x0a,
     174,   16,   16,   16, 0x0a,
     193,   16,   16,   16, 0x0a,
     211,   16,   16,   16, 0x0a,
     229,   16,   16,   16, 0x0a,
     250,   16,   16,   16, 0x0a,
     270,   16,   16,   16, 0x0a,
     291,   16,   16,   16, 0x0a,
     318,   16,   16,   16, 0x0a,
     337,   16,   16,   16, 0x0a,
     359,   16,   16,   16, 0x0a,
     381,   16,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_wsp__QtGLWidget[] = {
    "wsp::QtGLWidget\0\0enabled\0enableWireframe(bool)\0"
    "enableNormals(bool)\0setModelColor()\0"
    "setBackgroundColor()\0type\0"
    "slot_changeDisplayType(int)\0"
    "slot_loadModel()\0slot_loadDepthMap()\0"
    "slot_loadTexture()\0slot_loadShader()\0"
    "slot_vertexDisp()\0slot_wireframeDisp()\0"
    "slot_triangleDisp()\0slot_disableEnvmap()\0"
    "slot_enableEnvmapCubemap()\0"
    "slot_testDrawOff()\0slot_testDrawSphere()\0"
    "slot_testDrawTeapot()\0slot_setEnvmapRatio(int)\0"
};

const QMetaObject wsp::QtGLWidget::staticMetaObject = {
    { &::QGLWidget::staticMetaObject, qt_meta_stringdata_wsp__QtGLWidget,
      qt_meta_data_wsp__QtGLWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &wsp::QtGLWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *wsp::QtGLWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *wsp::QtGLWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_wsp__QtGLWidget))
        return static_cast<void*>(const_cast< QtGLWidget*>(this));
    typedef ::QGLWidget QMocSuperClass;
    return QMocSuperClass::qt_metacast(_clname);
}

int wsp::QtGLWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    typedef ::QGLWidget QMocSuperClass;
    _id = QMocSuperClass::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: enableWireframe((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: enableNormals((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: setModelColor(); break;
        case 3: setBackgroundColor(); break;
        case 4: slot_changeDisplayType((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: slot_loadModel(); break;
        case 6: slot_loadDepthMap(); break;
        case 7: slot_loadTexture(); break;
        case 8: slot_loadShader(); break;
        case 9: slot_vertexDisp(); break;
        case 10: slot_wireframeDisp(); break;
        case 11: slot_triangleDisp(); break;
        case 12: slot_disableEnvmap(); break;
        case 13: slot_enableEnvmapCubemap(); break;
        case 14: slot_testDrawOff(); break;
        case 15: slot_testDrawSphere(); break;
        case 16: slot_testDrawTeapot(); break;
        case 17: slot_setEnvmapRatio((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 18;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

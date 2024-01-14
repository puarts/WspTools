/****************************************************************************
** Meta object code from reading C++ file 'cl-qt_image_filter_plugin_action.hpp'
**
** Created: Thu Oct 23 01:04:54 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../cl-qt_image_filter_plugin_action.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cl-qt_image_filter_plugin_action.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_wsp__QtImageFilterPluginAction[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      45,   32,   31,   31, 0x05,

 // slots: signature, parameters, type, tag, flags
      67,   31,   31,   31, 0x0a,
      86,   31,   31,   31, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_wsp__QtImageFilterPluginAction[] = {
    "wsp::QtImageFilterPluginAction\0\0"
    "action_index\0signal_emitIndex(int)\0"
    "slot_applyFilter()\0slot_emitIndex()\0"
};

const QMetaObject wsp::QtImageFilterPluginAction::staticMetaObject = {
    { &QAction::staticMetaObject, qt_meta_stringdata_wsp__QtImageFilterPluginAction,
      qt_meta_data_wsp__QtImageFilterPluginAction, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &wsp::QtImageFilterPluginAction::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *wsp::QtImageFilterPluginAction::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *wsp::QtImageFilterPluginAction::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_wsp__QtImageFilterPluginAction))
        return static_cast<void*>(const_cast< QtImageFilterPluginAction*>(this));
    if (!strcmp(_clname, "wsp::NonCopyable"))
        return static_cast< wsp::NonCopyable*>(const_cast< QtImageFilterPluginAction*>(this));
    return QAction::qt_metacast(_clname);
}

int wsp::QtImageFilterPluginAction::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAction::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: signal_emitIndex((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: slot_applyFilter(); break;
        case 2: slot_emitIndex(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void wsp::QtImageFilterPluginAction::signal_emitIndex(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE

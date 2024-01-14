/****************************************************************************
** Meta object code from reading C++ file 'cl-qt_slider_delegate.hpp'
**
** Created: Thu Oct 23 01:04:55 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../cl-qt_slider_delegate.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cl-qt_slider_delegate.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_wsp__TimeLineEditor[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   20,   20,   20, 0x05,
      45,   39,   20,   20, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_wsp__TimeLineEditor[] = {
    "wsp::TimeLineEditor\0\0editingFinished()\0"
    "value\0signal_valueChanged(int)\0"
};

const QMetaObject wsp::TimeLineEditor::staticMetaObject = {
    { &QSlider::staticMetaObject, qt_meta_stringdata_wsp__TimeLineEditor,
      qt_meta_data_wsp__TimeLineEditor, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &wsp::TimeLineEditor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *wsp::TimeLineEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *wsp::TimeLineEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_wsp__TimeLineEditor))
        return static_cast<void*>(const_cast< TimeLineEditor*>(this));
    return QSlider::qt_metacast(_clname);
}

int wsp::TimeLineEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSlider::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: editingFinished(); break;
        case 1: signal_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void wsp::TimeLineEditor::editingFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void wsp::TimeLineEditor::signal_valueChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
static const uint qt_meta_data_wsp__QtSliderDelegate[] = {

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
      29,   23,   22,   22, 0x05,

 // slots: signature, parameters, type, tag, flags
      54,   22,   22,   22, 0x08,
      82,   23,   22,   22, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_wsp__QtSliderDelegate[] = {
    "wsp::QtSliderDelegate\0\0value\0"
    "signal_valueChanged(int)\0"
    "slot_commitAndCloseEditor()\0"
    "slot_valueChanged(int)\0"
};

const QMetaObject wsp::QtSliderDelegate::staticMetaObject = {
    { &QStyledItemDelegate::staticMetaObject, qt_meta_stringdata_wsp__QtSliderDelegate,
      qt_meta_data_wsp__QtSliderDelegate, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &wsp::QtSliderDelegate::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *wsp::QtSliderDelegate::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *wsp::QtSliderDelegate::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_wsp__QtSliderDelegate))
        return static_cast<void*>(const_cast< QtSliderDelegate*>(this));
    return QStyledItemDelegate::qt_metacast(_clname);
}

int wsp::QtSliderDelegate::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QStyledItemDelegate::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: signal_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: slot_commitAndCloseEditor(); break;
        case 2: slot_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void wsp::QtSliderDelegate::signal_valueChanged(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE

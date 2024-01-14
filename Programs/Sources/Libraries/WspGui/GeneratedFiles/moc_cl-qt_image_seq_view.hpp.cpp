/****************************************************************************
** Meta object code from reading C++ file 'cl-qt_image_seq_view.hpp'
**
** Created: Thu Oct 23 01:04:55 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../cl-qt_image_seq_view.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cl-qt_image_seq_view.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_wsp__QtImageSeqView[] = {

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
      27,   21,   20,   20, 0x08,
      49,   20,   20,   20, 0x08,
      65,   20,   20,   20, 0x08,
      82,   20,   20,   20, 0x08,
      99,   20,   20,   20, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_wsp__QtImageSeqView[] = {
    "wsp::QtImageSeqView\0\0frame\0"
    "slot_changeFrame(int)\0slot_PlayStop()\0"
    "slot_showColor()\0slot_showAlpha()\0"
    "slot_showState()\0"
};

const QMetaObject wsp::QtImageSeqView::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_wsp__QtImageSeqView,
      qt_meta_data_wsp__QtImageSeqView, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &wsp::QtImageSeqView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *wsp::QtImageSeqView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *wsp::QtImageSeqView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_wsp__QtImageSeqView))
        return static_cast<void*>(const_cast< QtImageSeqView*>(this));
    return QWidget::qt_metacast(_clname);
}

int wsp::QtImageSeqView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: slot_changeFrame((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: slot_PlayStop(); break;
        case 2: slot_showColor(); break;
        case 3: slot_showAlpha(); break;
        case 4: slot_showState(); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

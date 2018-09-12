/****************************************************************************
** Meta object code from reading C++ file 'analysisprogressview.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../p502_GUI/analysisProgressWidget/analysisprogressview.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'analysisprogressview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_analysisProgressView_t {
    QByteArrayData data[7];
    char stringdata0[87];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_analysisProgressView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_analysisProgressView_t qt_meta_stringdata_analysisProgressView = {
    {
QT_MOC_LITERAL(0, 0, 20), // "analysisProgressView"
QT_MOC_LITERAL(1, 21, 16), // "analysisCanceled"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 14), // "updateProgress"
QT_MOC_LITERAL(4, 54, 10), // "progressIN"
QT_MOC_LITERAL(5, 65, 12), // "updateStatus"
QT_MOC_LITERAL(6, 78, 8) // "statusIN"

    },
    "analysisProgressView\0analysisCanceled\0"
    "\0updateProgress\0progressIN\0updateStatus\0"
    "statusIN"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_analysisProgressView[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   30,    2, 0x0a /* Public */,
       5,    1,   33,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::QString,    6,

       0        // eod
};

void analysisProgressView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        analysisProgressView *_t = static_cast<analysisProgressView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->analysisCanceled(); break;
        case 1: _t->updateProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->updateStatus((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (analysisProgressView::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&analysisProgressView::analysisCanceled)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject analysisProgressView::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_analysisProgressView.data,
      qt_meta_data_analysisProgressView,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *analysisProgressView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *analysisProgressView::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_analysisProgressView.stringdata0))
        return static_cast<void*>(const_cast< analysisProgressView*>(this));
    return QWidget::qt_metacast(_clname);
}

int analysisProgressView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void analysisProgressView::analysisCanceled()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE

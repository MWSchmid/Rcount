/****************************************************************************
** Meta object code from reading C++ file 'bamhandler.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../p502_SOURCE/bamHandler/bamhandler.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bamhandler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_bamHandler_t {
    QByteArrayData data[10];
    char stringdata0[116];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_bamHandler_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_bamHandler_t qt_meta_stringdata_bamHandler = {
    {
QT_MOC_LITERAL(0, 0, 10), // "bamHandler"
QT_MOC_LITERAL(1, 11, 13), // "detectedError"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 12), // "errorMessage"
QT_MOC_LITERAL(4, 39, 15), // "xLinesProcessed"
QT_MOC_LITERAL(5, 55, 5), // "lines"
QT_MOC_LITERAL(6, 61, 13), // "statsObtained"
QT_MOC_LITERAL(7, 75, 11), // "statsString"
QT_MOC_LITERAL(8, 87, 16), // "producerFinished"
QT_MOC_LITERAL(9, 104, 11) // "runCanceled"

    },
    "bamHandler\0detectedError\0\0errorMessage\0"
    "xLinesProcessed\0lines\0statsObtained\0"
    "statsString\0producerFinished\0runCanceled"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_bamHandler[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,
       4,    1,   42,    2, 0x06 /* Public */,
       6,    1,   45,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   48,    2, 0x0a /* Public */,
       9,    0,   49,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::QString,    7,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void bamHandler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        bamHandler *_t = static_cast<bamHandler *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->detectedError((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->xLinesProcessed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->statsObtained((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->producerFinished(); break;
        case 4: _t->runCanceled(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (bamHandler::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&bamHandler::detectedError)) {
                *result = 0;
            }
        }
        {
            typedef void (bamHandler::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&bamHandler::xLinesProcessed)) {
                *result = 1;
            }
        }
        {
            typedef void (bamHandler::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&bamHandler::statsObtained)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject bamHandler::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_bamHandler.data,
      qt_meta_data_bamHandler,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *bamHandler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *bamHandler::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_bamHandler.stringdata0))
        return static_cast<void*>(const_cast< bamHandler*>(this));
    return QThread::qt_metacast(_clname);
}

int bamHandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void bamHandler::detectedError(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void bamHandler::xLinesProcessed(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void bamHandler::statsObtained(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE

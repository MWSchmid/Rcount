/****************************************************************************
** Meta object code from reading C++ file 'dataanalysis.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../p502_SOURCE/dataAnalysis/dataanalysis.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dataanalysis.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_dataAnalysis_t {
    QByteArrayData data[22];
    char stringdata0[284];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_dataAnalysis_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_dataAnalysis_t qt_meta_stringdata_dataAnalysis = {
    {
QT_MOC_LITERAL(0, 0, 12), // "dataAnalysis"
QT_MOC_LITERAL(1, 13, 12), // "errorMessage"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 7), // "message"
QT_MOC_LITERAL(4, 35, 11), // "stopThreads"
QT_MOC_LITERAL(5, 47, 14), // "analysisStatus"
QT_MOC_LITERAL(6, 62, 6), // "status"
QT_MOC_LITERAL(7, 69, 16), // "analysisProgress"
QT_MOC_LITERAL(8, 86, 8), // "progress"
QT_MOC_LITERAL(9, 95, 21), // "analysisCountsWritten"
QT_MOC_LITERAL(10, 117, 8), // "fileName"
QT_MOC_LITERAL(11, 126, 19), // "everythingProcessed"
QT_MOC_LITERAL(12, 146, 9), // "idleAgain"
QT_MOC_LITERAL(13, 156, 19), // "readerStatsObtained"
QT_MOC_LITERAL(14, 176, 11), // "statsString"
QT_MOC_LITERAL(15, 188, 19), // "writerStatsObtained"
QT_MOC_LITERAL(16, 208, 11), // "readerLines"
QT_MOC_LITERAL(17, 220, 8), // "numLines"
QT_MOC_LITERAL(18, 229, 11), // "writerLines"
QT_MOC_LITERAL(19, 241, 11), // "readerStats"
QT_MOC_LITERAL(20, 253, 11), // "writerStats"
QT_MOC_LITERAL(21, 265, 18) // "cancelDataAnalysis"

    },
    "dataAnalysis\0errorMessage\0\0message\0"
    "stopThreads\0analysisStatus\0status\0"
    "analysisProgress\0progress\0"
    "analysisCountsWritten\0fileName\0"
    "everythingProcessed\0idleAgain\0"
    "readerStatsObtained\0statsString\0"
    "writerStatsObtained\0readerLines\0"
    "numLines\0writerLines\0readerStats\0"
    "writerStats\0cancelDataAnalysis"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_dataAnalysis[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   84,    2, 0x06 /* Public */,
       4,    0,   87,    2, 0x06 /* Public */,
       5,    1,   88,    2, 0x06 /* Public */,
       7,    1,   91,    2, 0x06 /* Public */,
       9,    1,   94,    2, 0x06 /* Public */,
      11,    0,   97,    2, 0x06 /* Public */,
      12,    0,   98,    2, 0x06 /* Public */,
      13,    1,   99,    2, 0x06 /* Public */,
      15,    1,  102,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      16,    1,  105,    2, 0x08 /* Private */,
      18,    1,  108,    2, 0x08 /* Private */,
      19,    1,  111,    2, 0x08 /* Private */,
      20,    1,  114,    2, 0x08 /* Private */,
      21,    0,  117,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void,

       0        // eod
};

void dataAnalysis::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        dataAnalysis *_t = static_cast<dataAnalysis *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->errorMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->stopThreads(); break;
        case 2: _t->analysisStatus((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->analysisProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->analysisCountsWritten((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->everythingProcessed(); break;
        case 6: _t->idleAgain(); break;
        case 7: _t->readerStatsObtained((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->writerStatsObtained((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: _t->readerLines((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->writerLines((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->readerStats((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 12: _t->writerStats((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 13: _t->cancelDataAnalysis(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (dataAnalysis::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&dataAnalysis::errorMessage)) {
                *result = 0;
            }
        }
        {
            typedef void (dataAnalysis::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&dataAnalysis::stopThreads)) {
                *result = 1;
            }
        }
        {
            typedef void (dataAnalysis::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&dataAnalysis::analysisStatus)) {
                *result = 2;
            }
        }
        {
            typedef void (dataAnalysis::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&dataAnalysis::analysisProgress)) {
                *result = 3;
            }
        }
        {
            typedef void (dataAnalysis::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&dataAnalysis::analysisCountsWritten)) {
                *result = 4;
            }
        }
        {
            typedef void (dataAnalysis::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&dataAnalysis::everythingProcessed)) {
                *result = 5;
            }
        }
        {
            typedef void (dataAnalysis::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&dataAnalysis::idleAgain)) {
                *result = 6;
            }
        }
        {
            typedef void (dataAnalysis::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&dataAnalysis::readerStatsObtained)) {
                *result = 7;
            }
        }
        {
            typedef void (dataAnalysis::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&dataAnalysis::writerStatsObtained)) {
                *result = 8;
            }
        }
    }
}

const QMetaObject dataAnalysis::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_dataAnalysis.data,
      qt_meta_data_dataAnalysis,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *dataAnalysis::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *dataAnalysis::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_dataAnalysis.stringdata0))
        return static_cast<void*>(const_cast< dataAnalysis*>(this));
    return QThread::qt_metacast(_clname);
}

int dataAnalysis::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void dataAnalysis::errorMessage(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void dataAnalysis::stopThreads()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void dataAnalysis::analysisStatus(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void dataAnalysis::analysisProgress(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void dataAnalysis::analysisCountsWritten(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void dataAnalysis::everythingProcessed()
{
    QMetaObject::activate(this, &staticMetaObject, 5, Q_NULLPTR);
}

// SIGNAL 6
void dataAnalysis::idleAgain()
{
    QMetaObject::activate(this, &staticMetaObject, 6, Q_NULLPTR);
}

// SIGNAL 7
void dataAnalysis::readerStatsObtained(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void dataAnalysis::writerStatsObtained(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}
QT_END_MOC_NAMESPACE

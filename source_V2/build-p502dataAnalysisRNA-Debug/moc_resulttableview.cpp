/****************************************************************************
** Meta object code from reading C++ file 'resulttableview.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../p502_GUI/resultTableViewWidget/resulttableview.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'resulttableview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_resultTableView_t {
    QByteArrayData data[7];
    char stringdata0[74];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_resultTableView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_resultTableView_t qt_meta_stringdata_resultTableView = {
    {
QT_MOC_LITERAL(0, 0, 15), // "resultTableView"
QT_MOC_LITERAL(1, 16, 7), // "findRow"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 13), // "displayCounts"
QT_MOC_LITERAL(4, 39, 8), // "fileName"
QT_MOC_LITERAL(5, 48, 12), // "updateCounts"
QT_MOC_LITERAL(6, 61, 12) // "removeCounts"

    },
    "resultTableView\0findRow\0\0displayCounts\0"
    "fileName\0updateCounts\0removeCounts"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_resultTableView[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x08 /* Private */,
       3,    1,   35,    2, 0x0a /* Public */,
       5,    1,   38,    2, 0x0a /* Public */,
       6,    1,   41,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, QMetaType::QString,    4,

       0        // eod
};

void resultTableView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        resultTableView *_t = static_cast<resultTableView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->findRow(); break;
        case 1: _t->displayCounts((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->updateCounts((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->removeCounts((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject resultTableView::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_resultTableView.data,
      qt_meta_data_resultTableView,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *resultTableView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *resultTableView::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_resultTableView.stringdata0))
        return static_cast<void*>(const_cast< resultTableView*>(this));
    return QWidget::qt_metacast(_clname);
}

int resultTableView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

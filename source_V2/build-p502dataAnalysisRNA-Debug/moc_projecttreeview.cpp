/****************************************************************************
** Meta object code from reading C++ file 'projecttreeview.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../p502_GUI/projectTreeViewWidget/projecttreeview.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'projecttreeview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_projectTreeView_t {
    QByteArrayData data[17];
    char stringdata0[206];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_projectTreeView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_projectTreeView_t qt_meta_stringdata_projectTreeView = {
    {
QT_MOC_LITERAL(0, 0, 15), // "projectTreeView"
QT_MOC_LITERAL(1, 16, 10), // "newClicked"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 11), // "openClicked"
QT_MOC_LITERAL(4, 40, 14), // "runClickedFile"
QT_MOC_LITERAL(5, 55, 13), // "closedProject"
QT_MOC_LITERAL(6, 69, 14), // "currentProject"
QT_MOC_LITERAL(7, 84, 15), // "switchedProject"
QT_MOC_LITERAL(8, 100, 5), // "index"
QT_MOC_LITERAL(9, 106, 13), // "resizeColumns"
QT_MOC_LITERAL(10, 120, 10), // "runClicked"
QT_MOC_LITERAL(11, 131, 13), // "runAllClicked"
QT_MOC_LITERAL(12, 145, 19), // "closeCurrentProject"
QT_MOC_LITERAL(13, 165, 10), // "addProject"
QT_MOC_LITERAL(14, 176, 8), // "fileName"
QT_MOC_LITERAL(15, 185, 8), // "addStats"
QT_MOC_LITERAL(16, 194, 11) // "statsString"

    },
    "projectTreeView\0newClicked\0\0openClicked\0"
    "runClickedFile\0closedProject\0"
    "currentProject\0switchedProject\0index\0"
    "resizeColumns\0runClicked\0runAllClicked\0"
    "closeCurrentProject\0addProject\0fileName\0"
    "addStats\0statsString"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_projectTreeView[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x06 /* Public */,
       3,    0,   75,    2, 0x06 /* Public */,
       4,    1,   76,    2, 0x06 /* Public */,
       5,    1,   79,    2, 0x06 /* Public */,
       6,    1,   82,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    1,   85,    2, 0x08 /* Private */,
       9,    0,   88,    2, 0x08 /* Private */,
      10,    0,   89,    2, 0x08 /* Private */,
      11,    0,   90,    2, 0x08 /* Private */,
      12,    0,   91,    2, 0x0a /* Public */,
      13,    1,   92,    2, 0x0a /* Public */,
      15,    1,   95,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::QModelIndex,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   16,

       0        // eod
};

void projectTreeView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        projectTreeView *_t = static_cast<projectTreeView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->newClicked(); break;
        case 1: _t->openClicked(); break;
        case 2: _t->runClickedFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->closedProject((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->currentProject((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->switchedProject((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 6: _t->resizeColumns(); break;
        case 7: _t->runClicked(); break;
        case 8: _t->runAllClicked(); break;
        case 9: _t->closeCurrentProject(); break;
        case 10: _t->addProject((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 11: _t->addStats((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (projectTreeView::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&projectTreeView::newClicked)) {
                *result = 0;
            }
        }
        {
            typedef void (projectTreeView::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&projectTreeView::openClicked)) {
                *result = 1;
            }
        }
        {
            typedef void (projectTreeView::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&projectTreeView::runClickedFile)) {
                *result = 2;
            }
        }
        {
            typedef void (projectTreeView::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&projectTreeView::closedProject)) {
                *result = 3;
            }
        }
        {
            typedef void (projectTreeView::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&projectTreeView::currentProject)) {
                *result = 4;
            }
        }
    }
}

const QMetaObject projectTreeView::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_projectTreeView.data,
      qt_meta_data_projectTreeView,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *projectTreeView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *projectTreeView::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_projectTreeView.stringdata0))
        return static_cast<void*>(const_cast< projectTreeView*>(this));
    return QWidget::qt_metacast(_clname);
}

int projectTreeView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void projectTreeView::newClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void projectTreeView::openClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void projectTreeView::runClickedFile(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void projectTreeView::closedProject(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void projectTreeView::currentProject(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE

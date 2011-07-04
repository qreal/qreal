/****************************************************************************
** Meta object code from reading C++ file 'logicalModelView.h'
**
** Created: Fri Jul 1 16:40:41 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../models/details/logicalModelView.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'logicalModelView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_qReal__models__details__LogicalModelView[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      62,   42,   41,   41, 0x09,
     116,   99,   41,   41, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_qReal__models__details__LogicalModelView[] = {
    "qReal::models::details::LogicalModelView\0"
    "\0topLeft,bottomRight\0"
    "dataChanged(QModelIndex,QModelIndex)\0"
    "parent,start,end\0"
    "rowsAboutToBeRemoved(QModelIndex,int,int)\0"
};

const QMetaObject qReal::models::details::LogicalModelView::staticMetaObject = {
    { &modelsImplementation::AbstractView::staticMetaObject, qt_meta_stringdata_qReal__models__details__LogicalModelView,
      qt_meta_data_qReal__models__details__LogicalModelView, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &qReal::models::details::LogicalModelView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *qReal::models::details::LogicalModelView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *qReal::models::details::LogicalModelView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_qReal__models__details__LogicalModelView))
        return static_cast<void*>(const_cast< LogicalModelView*>(this));
    typedef modelsImplementation::AbstractView QMocSuperClass;
    return QMocSuperClass::qt_metacast(_clname);
}

int qReal::models::details::LogicalModelView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    typedef modelsImplementation::AbstractView QMocSuperClass;
    _id = QMocSuperClass::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: dataChanged((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< const QModelIndex(*)>(_a[2]))); break;
        case 1: rowsAboutToBeRemoved((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

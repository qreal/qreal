/****************************************************************************
** Meta object code from reading C++ file 'abstractView.h'
**
** Created: Fri Jul 1 16:40:43 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../models/details/modelsImplementation/abstractView.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'abstractView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_qReal__models__details__modelsImplementation__AbstractView[] = {

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
     128,   60,   59,   59, 0x0a,
     184,   60,   59,   59, 0x0a,
     251,  231,   59,   59, 0x09,
     305,  288,   59,   59, 0x09,
     347,  288,   59,   59, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_qReal__models__details__modelsImplementation__AbstractView[] = {
    "qReal::models::details::modelsImplementation::AbstractView\0"
    "\0sourceParent,sourceStart,sourceEnd,destinationParent,destinationRow\0"
    "rowsAboutToBeMoved(QModelIndex,int,int,QModelIndex,int)\0"
    "rowsMoved(QModelIndex,int,int,QModelIndex,int)\0"
    "topLeft,bottomRight\0"
    "dataChanged(QModelIndex,QModelIndex)\0"
    "parent,start,end\0"
    "rowsAboutToBeRemoved(QModelIndex,int,int)\0"
    "rowsInserted(QModelIndex,int,int)\0"
};

const QMetaObject qReal::models::details::modelsImplementation::AbstractView::staticMetaObject = {
    { &QAbstractItemView::staticMetaObject, qt_meta_stringdata_qReal__models__details__modelsImplementation__AbstractView,
      qt_meta_data_qReal__models__details__modelsImplementation__AbstractView, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &qReal::models::details::modelsImplementation::AbstractView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *qReal::models::details::modelsImplementation::AbstractView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *qReal::models::details::modelsImplementation::AbstractView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_qReal__models__details__modelsImplementation__AbstractView))
        return static_cast<void*>(const_cast< AbstractView*>(this));
    return QAbstractItemView::qt_metacast(_clname);
}

int qReal::models::details::modelsImplementation::AbstractView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractItemView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: rowsAboutToBeMoved((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< const QModelIndex(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 1: rowsMoved((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< const QModelIndex(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 2: dataChanged((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< const QModelIndex(*)>(_a[2]))); break;
        case 3: rowsAboutToBeRemoved((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 4: rowsInserted((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

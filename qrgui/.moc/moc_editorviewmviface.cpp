/****************************************************************************
** Meta object code from reading C++ file 'editorviewmviface.h'
**
** Created: Fri Jul 1 16:40:34 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../view/editorviewmviface.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'editorviewmviface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_qReal__EditorViewMViface[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      26,   25,   25,   25, 0x0a,
      40,   34,   25,   25, 0x0a,
     134,   66,   25,   25, 0x0a,
     190,   66,   25,   25, 0x0a,
     257,  237,   25,   25, 0x08,
     311,  294,   25,   25, 0x08,
     353,  294,   25,   25, 0x08,
     387,  237,   25,   25, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_qReal__EditorViewMViface[] = {
    "qReal::EditorViewMViface\0\0reset()\0"
    "index\0setRootIndex(QModelIndex)\0"
    "sourceParent,sourceStart,sourceEnd,destinationParent,destinationRow\0"
    "rowsAboutToBeMoved(QModelIndex,int,int,QModelIndex,int)\0"
    "rowsMoved(QModelIndex,int,int,QModelIndex,int)\0"
    "topLeft,bottomRight\0"
    "dataChanged(QModelIndex,QModelIndex)\0"
    "parent,start,end\0"
    "rowsAboutToBeRemoved(QModelIndex,int,int)\0"
    "rowsInserted(QModelIndex,int,int)\0"
    "logicalDataChanged(QModelIndex,QModelIndex)\0"
};

const QMetaObject qReal::EditorViewMViface::staticMetaObject = {
    { &QAbstractItemView::staticMetaObject, qt_meta_stringdata_qReal__EditorViewMViface,
      qt_meta_data_qReal__EditorViewMViface, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &qReal::EditorViewMViface::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *qReal::EditorViewMViface::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *qReal::EditorViewMViface::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_qReal__EditorViewMViface))
        return static_cast<void*>(const_cast< EditorViewMViface*>(this));
    return QAbstractItemView::qt_metacast(_clname);
}

int qReal::EditorViewMViface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractItemView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: reset(); break;
        case 1: setRootIndex((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 2: rowsAboutToBeMoved((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< const QModelIndex(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 3: rowsMoved((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< const QModelIndex(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5]))); break;
        case 4: dataChanged((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< const QModelIndex(*)>(_a[2]))); break;
        case 5: rowsAboutToBeRemoved((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 6: rowsInserted((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 7: logicalDataChanged((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< const QModelIndex(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 8;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

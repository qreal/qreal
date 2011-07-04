/****************************************************************************
** Meta object code from reading C++ file 'graphicalModelAssistApi.h'
**
** Created: Fri Jul 1 16:40:45 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../models/graphicalModelAssistApi.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'graphicalModelAssistApi.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_qReal__models__GraphicalModelAssistApi[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      43,   40,   39,   39, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_qReal__models__GraphicalModelAssistApi[] = {
    "qReal::models::GraphicalModelAssistApi\0"
    "\0id\0nameChanged(Id)\0"
};

const QMetaObject qReal::models::GraphicalModelAssistApi::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_qReal__models__GraphicalModelAssistApi,
      qt_meta_data_qReal__models__GraphicalModelAssistApi, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &qReal::models::GraphicalModelAssistApi::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *qReal::models::GraphicalModelAssistApi::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *qReal::models::GraphicalModelAssistApi::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_qReal__models__GraphicalModelAssistApi))
        return static_cast<void*>(const_cast< GraphicalModelAssistApi*>(this));
    if (!strcmp(_clname, "details::ModelsAssistApi"))
        return static_cast< details::ModelsAssistApi*>(const_cast< GraphicalModelAssistApi*>(this));
    return QObject::qt_metacast(_clname);
}

int qReal::models::GraphicalModelAssistApi::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: nameChanged((*reinterpret_cast< const Id(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void qReal::models::GraphicalModelAssistApi::nameChanged(Id const & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE

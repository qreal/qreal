/****************************************************************************
** Meta object code from reading C++ file 'contextMenuAction.h'
**
** Created: Fri Jul 1 16:40:04 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../umllib/contextMenuAction.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'contextMenuAction.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_UML__ContextMenuAction[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      28,   24,   23,   23, 0x05,

 // slots: signature, parameters, type, tag, flags
      47,   23,   23,   23, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_UML__ContextMenuAction[] = {
    "UML::ContextMenuAction\0\0pos\0"
    "triggered(QPointF)\0trigger()\0"
};

const QMetaObject UML::ContextMenuAction::staticMetaObject = {
    { &QAction::staticMetaObject, qt_meta_stringdata_UML__ContextMenuAction,
      qt_meta_data_UML__ContextMenuAction, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &UML::ContextMenuAction::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *UML::ContextMenuAction::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *UML::ContextMenuAction::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_UML__ContextMenuAction))
        return static_cast<void*>(const_cast< ContextMenuAction*>(this));
    return QAction::qt_metacast(_clname);
}

int UML::ContextMenuAction::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAction::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: triggered((*reinterpret_cast< const QPointF(*)>(_a[1]))); break;
        case 1: trigger(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void UML::ContextMenuAction::triggered(QPointF const & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE

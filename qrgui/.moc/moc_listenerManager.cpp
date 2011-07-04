/****************************************************************************
** Meta object code from reading C++ file 'listenerManager.h'
**
** Created: Fri Jul 1 16:40:37 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../editorManager/listenerManager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'listenerManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_qReal__ListenerManager[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      27,   24,   23,   23, 0x05,
      57,   52,   23,   23, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_qReal__ListenerManager[] = {
    "qReal::ListenerManager\0\0id\0"
    "objectCreated(qReal::Id)\0name\0"
    "contextMenuActionTriggered(QString)\0"
};

const QMetaObject qReal::ListenerManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_qReal__ListenerManager,
      qt_meta_data_qReal__ListenerManager, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &qReal::ListenerManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *qReal::ListenerManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *qReal::ListenerManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_qReal__ListenerManager))
        return static_cast<void*>(const_cast< ListenerManager*>(this));
    return QObject::qt_metacast(_clname);
}

int qReal::ListenerManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: objectCreated((*reinterpret_cast< const qReal::Id(*)>(_a[1]))); break;
        case 1: contextMenuActionTriggered((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void qReal::ListenerManager::objectCreated(qReal::Id const & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void qReal::ListenerManager::contextMenuActionTriggered(QString const & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE

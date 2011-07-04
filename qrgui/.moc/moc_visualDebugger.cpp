/****************************************************************************
** Meta object code from reading C++ file 'visualDebugger.h'
**
** Created: Fri Jul 1 16:40:46 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../visualDebugger/visualDebugger.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'visualDebugger.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_qReal__VisualDebugger[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      23,   22,   22,   22, 0x0a,
      38,   22,   22,   22, 0x0a,
      46,   22,   22,   22, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_qReal__VisualDebugger[] = {
    "qReal::VisualDebugger\0\0generateCode()\0"
    "debug()\0debugSingleStep()\0"
};

const QMetaObject qReal::VisualDebugger::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_qReal__VisualDebugger,
      qt_meta_data_qReal__VisualDebugger, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &qReal::VisualDebugger::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *qReal::VisualDebugger::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *qReal::VisualDebugger::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_qReal__VisualDebugger))
        return static_cast<void*>(const_cast< VisualDebugger*>(this));
    return QObject::qt_metacast(_clname);
}

int qReal::VisualDebugger::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: generateCode(); break;
        case 1: debug(); break;
        case 2: debugSingleStep(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

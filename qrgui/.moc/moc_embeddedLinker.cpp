/****************************************************************************
** Meta object code from reading C++ file 'embeddedLinker.h'
**
** Created: Fri Jul 1 16:40:05 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../umllib/embeddedLinker.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'embeddedLinker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_UML__EmbeddedLinker[] = {

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
      21,   20,   20,   20, 0x05,

 // slots: signature, parameters, type, tag, flags
      38,   20,   20,   20, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_UML__EmbeddedLinker[] = {
    "UML::EmbeddedLinker\0\0coveredChanged()\0"
    "changeShowState()\0"
};

const QMetaObject UML::EmbeddedLinker::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_UML__EmbeddedLinker,
      qt_meta_data_UML__EmbeddedLinker, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &UML::EmbeddedLinker::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *UML::EmbeddedLinker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *UML::EmbeddedLinker::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_UML__EmbeddedLinker))
        return static_cast<void*>(const_cast< EmbeddedLinker*>(this));
    if (!strcmp(_clname, "QGraphicsItem"))
        return static_cast< QGraphicsItem*>(const_cast< EmbeddedLinker*>(this));
    if (!strcmp(_clname, "com.trolltech.Qt.QGraphicsItem"))
        return static_cast< QGraphicsItem*>(const_cast< EmbeddedLinker*>(this));
    return QObject::qt_metacast(_clname);
}

int UML::EmbeddedLinker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: coveredChanged(); break;
        case 1: changeShowState(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void UML::EmbeddedLinker::coveredChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE

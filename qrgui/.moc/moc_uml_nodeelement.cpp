/****************************************************************************
** Meta object code from reading C++ file 'uml_nodeelement.h'
**
** Created: Fri Jul 1 16:40:01 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../umllib/uml_nodeelement.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'uml_nodeelement.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_UML__NodeElement[] = {

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
      28,   18,   17,   17, 0x0a,
      45,   17,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_UML__NodeElement[] = {
    "UML::NodeElement\0\0isChecked\0"
    "switchGrid(bool)\0copyAndPlaceOnDiagram()\0"
};

const QMetaObject UML::NodeElement::staticMetaObject = {
    { &Element::staticMetaObject, qt_meta_stringdata_UML__NodeElement,
      qt_meta_data_UML__NodeElement, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &UML::NodeElement::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *UML::NodeElement::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *UML::NodeElement::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_UML__NodeElement))
        return static_cast<void*>(const_cast< NodeElement*>(this));
    return Element::qt_metacast(_clname);
}

int UML::NodeElement::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = Element::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: switchGrid((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: copyAndPlaceOnDiagram(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'shapeEdit.h'
**
** Created: Fri Jul 1 16:40:19 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow/shapeEdit/shapeEdit.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'shapeEdit.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ShapeEdit[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      28,   11,   10,   10, 0x05,
      74,   10,   10,   10, 0x05,
      87,   10,   10,   10, 0x05,
     105,   10,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
     118,   10,   10,   10, 0x08,
     132,   10,   10,   10, 0x08,
     144,   10,   10,   10, 0x08,
     151,   10,   10,   10, 0x08,
     166,  158,   10,   10, 0x08,
     181,   10,   10,   10, 0x08,
     214,  196,   10,   10, 0x08,
     242,   10,   10,   10, 0x08,
     283,  261,   10,   10, 0x08,
     322,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ShapeEdit[] = {
    "ShapeEdit\0\0shape,index,role\0"
    "shapeSaved(QString,QPersistentModelIndex,int)\0"
    "saveSignal()\0saveToXmlSignal()\0"
    "openSignal()\0savePicture()\0saveToXml()\0"
    "save()\0open()\0checked\0addImage(bool)\0"
    "setNoPalette()\0penItem,brushItem\0"
    "setItemPalette(QPen,QBrush)\0"
    "setNoFontPalette()\0penItem,fontItem,name\0"
    "setItemFontPalette(QPen,QFont,QString)\0"
    "changeTextName()\0"
};

const QMetaObject ShapeEdit::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ShapeEdit,
      qt_meta_data_ShapeEdit, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ShapeEdit::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ShapeEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ShapeEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ShapeEdit))
        return static_cast<void*>(const_cast< ShapeEdit*>(this));
    return QWidget::qt_metacast(_clname);
}

int ShapeEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: shapeSaved((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QPersistentModelIndex(*)>(_a[2])),(*reinterpret_cast< const int(*)>(_a[3]))); break;
        case 1: saveSignal(); break;
        case 2: saveToXmlSignal(); break;
        case 3: openSignal(); break;
        case 4: savePicture(); break;
        case 5: saveToXml(); break;
        case 6: save(); break;
        case 7: open(); break;
        case 8: addImage((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: setNoPalette(); break;
        case 10: setItemPalette((*reinterpret_cast< const QPen(*)>(_a[1])),(*reinterpret_cast< const QBrush(*)>(_a[2]))); break;
        case 11: setNoFontPalette(); break;
        case 12: setItemFontPalette((*reinterpret_cast< const QPen(*)>(_a[1])),(*reinterpret_cast< const QFont(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 13: changeTextName(); break;
        default: ;
        }
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void ShapeEdit::shapeSaved(QString const & _t1, QPersistentModelIndex const & _t2, int const & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ShapeEdit::saveSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void ShapeEdit::saveToXmlSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void ShapeEdit::openSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'refWindow.h'
**
** Created: Fri Jul 1 16:40:28 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow/refWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'refWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RefWindow[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x0a,
      38,   30,   10,   10, 0x0a,
      83,   78,   10,   10, 0x2a,
     118,   10,   10,   10, 0x0a,
     127,   78,   10,   10, 0x08,
     160,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_RefWindow[] = {
    "RefWindow\0\0setPropertyValue()\0item,bl\0"
    "highlightElement(QListWidgetItem*,bool)\0"
    "item\0highlightElement(QListWidgetItem*)\0"
    "cancel()\0enableOkButton(QListWidgetItem*)\0"
    "setElementId()\0"
};

const QMetaObject RefWindow::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_RefWindow,
      qt_meta_data_RefWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &RefWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *RefWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *RefWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RefWindow))
        return static_cast<void*>(const_cast< RefWindow*>(this));
    return QWidget::qt_metacast(_clname);
}

int RefWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setPropertyValue(); break;
        case 1: highlightElement((*reinterpret_cast< QListWidgetItem*(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 2: highlightElement((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 3: cancel(); break;
        case 4: enableOkButton((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 5: setElementId(); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

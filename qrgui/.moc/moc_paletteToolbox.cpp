/****************************************************************************
** Meta object code from reading C++ file 'paletteToolbox.h'
**
** Created: Fri Jul 1 16:40:13 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow/paletteToolbox.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'paletteToolbox.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_qReal__gui__PaletteToolbox[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      40,   28,   27,   27, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_qReal__gui__PaletteToolbox[] = {
    "qReal::gui::PaletteToolbox\0\0editorIndex\0"
    "setActiveEditor(int)\0"
};

const QMetaObject qReal::gui::PaletteToolbox::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_qReal__gui__PaletteToolbox,
      qt_meta_data_qReal__gui__PaletteToolbox, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &qReal::gui::PaletteToolbox::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *qReal::gui::PaletteToolbox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *qReal::gui::PaletteToolbox::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_qReal__gui__PaletteToolbox))
        return static_cast<void*>(const_cast< PaletteToolbox*>(this));
    return QWidget::qt_metacast(_clname);
}

int qReal::gui::PaletteToolbox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setActiveEditor((*reinterpret_cast< const int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

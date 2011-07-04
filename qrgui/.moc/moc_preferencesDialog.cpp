/****************************************************************************
** Meta object code from reading C++ file 'preferencesDialog.h'
**
** Created: Fri Jul 1 16:40:24 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow/preferencesDialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'preferencesDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PreferencesDialog[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x05,
      33,   18,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
      51,   18,   18,   18, 0x08,
      60,   18,   18,   18, 0x08,
      75,   18,   18,   18, 0x08,
      90,   18,   18,   18, 0x08,
     111,  105,   18,   18, 0x08,
     137,  105,   18,   18, 0x08,
     163,   18,   18,   18, 0x08,
     188,   18,   18,   18, 0x08,
     208,   18,   18,   18, 0x08,
     229,   18,   18,   18, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_PreferencesDialog[] = {
    "PreferencesDialog\0\0gridChanged()\0"
    "settingsApplied()\0cancel()\0applyChanges()\0"
    "saveAndClose()\0changeSystem()\0value\0"
    "widthGridSliderMoved(int)\0"
    "indexGridSliderMoved(int)\0"
    "changeWorkingDirectory()\0changeBuilderPath()\0"
    "changeDebuggerPath()\0chooseTab(QModelIndex)\0"
};

const QMetaObject PreferencesDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_PreferencesDialog,
      qt_meta_data_PreferencesDialog, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PreferencesDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PreferencesDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PreferencesDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PreferencesDialog))
        return static_cast<void*>(const_cast< PreferencesDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int PreferencesDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: gridChanged(); break;
        case 1: settingsApplied(); break;
        case 2: cancel(); break;
        case 3: applyChanges(); break;
        case 4: saveAndClose(); break;
        case 5: changeSystem(); break;
        case 6: widthGridSliderMoved((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: indexGridSliderMoved((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: changeWorkingDirectory(); break;
        case 9: changeBuilderPath(); break;
        case 10: changeDebuggerPath(); break;
        case 11: chooseTab((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void PreferencesDialog::gridChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void PreferencesDialog::settingsApplied()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE

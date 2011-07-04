/****************************************************************************
** Meta object code from reading C++ file 'editorview.h'
**
** Created: Fri Jul 1 16:40:31 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../view/editorview.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'editorview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_qReal__EditorView[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x0a,
      44,   18,   18,   18, 0x0a,
      63,   18,   18,   18, 0x0a,
      72,   18,   18,   18, 0x0a,
      82,   18,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_qReal__EditorView[] = {
    "qReal::EditorView\0\0toggleAntialiasing(bool)\0"
    "toggleOpenGL(bool)\0zoomIn()\0zoomOut()\0"
    "invalidateScene()\0"
};

const QMetaObject qReal::EditorView::staticMetaObject = {
    { &QGraphicsView::staticMetaObject, qt_meta_stringdata_qReal__EditorView,
      qt_meta_data_qReal__EditorView, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &qReal::EditorView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *qReal::EditorView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *qReal::EditorView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_qReal__EditorView))
        return static_cast<void*>(const_cast< EditorView*>(this));
    return QGraphicsView::qt_metacast(_clname);
}

int qReal::EditorView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: toggleAntialiasing((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: toggleOpenGL((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: zoomIn(); break;
        case 3: zoomOut(); break;
        case 4: invalidateScene(); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE

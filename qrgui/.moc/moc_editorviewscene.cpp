/****************************************************************************
** Meta object code from reading C++ file 'editorviewscene.h'
**
** Created: Fri Jul 1 16:40:33 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../view/editorviewscene.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'editorviewscene.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_EditorViewScene[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      20,   17,   16,   16, 0x05,
      46,   16,   16,   16, 0x05,
      55,   16,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
      76,   16,   65,   16, 0x0a,
      99,   16,   16,   16, 0x0a,
     106,   16,   16,   16, 0x0a,
     114,   16,   16,   16, 0x08,
     139,   16,   16,   16, 0x08,
     161,   16,   16,   16, 0x08,
     189,   16,   16,   16, 0x08,
     215,   16,   16,   16, 0x08,
     244,   16,   16,   16, 0x08,
     273,   16,   16,   16, 0x08,
     292,   16,   16,   16, 0x08,
     315,   16,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_EditorViewScene[] = {
    "EditorViewScene\0\0id\0elementCreated(qReal::Id)\0"
    "zoomIn()\0zoomOut()\0qReal::Id*\0"
    "createElement(QString)\0copy()\0paste()\0"
    "connectActionTriggered()\0goToActionTriggered()\0"
    "disconnectActionTriggered()\0"
    "addUsageActionTriggered()\0"
    "deleteUsageActionTriggered()\0"
    "printElementsOfRootDiagram()\0"
    "drawIdealGesture()\0initMouseMoveManager()\0"
    "createEdge(QString)\0"
};

const QMetaObject EditorViewScene::staticMetaObject = {
    { &QGraphicsScene::staticMetaObject, qt_meta_stringdata_EditorViewScene,
      qt_meta_data_EditorViewScene, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &EditorViewScene::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *EditorViewScene::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *EditorViewScene::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_EditorViewScene))
        return static_cast<void*>(const_cast< EditorViewScene*>(this));
    return QGraphicsScene::qt_metacast(_clname);
}

int EditorViewScene::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsScene::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: elementCreated((*reinterpret_cast< const qReal::Id(*)>(_a[1]))); break;
        case 1: zoomIn(); break;
        case 2: zoomOut(); break;
        case 3: { qReal::Id* _r = createElement((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< qReal::Id**>(_a[0]) = _r; }  break;
        case 4: copy(); break;
        case 5: paste(); break;
        case 6: connectActionTriggered(); break;
        case 7: goToActionTriggered(); break;
        case 8: disconnectActionTriggered(); break;
        case 9: addUsageActionTriggered(); break;
        case 10: deleteUsageActionTriggered(); break;
        case 11: printElementsOfRootDiagram(); break;
        case 12: drawIdealGesture(); break;
        case 13: initMouseMoveManager(); break;
        case 14: createEdge((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void EditorViewScene::elementCreated(qReal::Id const & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void EditorViewScene::zoomIn()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void EditorViewScene::zoomOut()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE

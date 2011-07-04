/****************************************************************************
** Meta object code from reading C++ file 'scene.h'
**
** Created: Fri Jul 1 16:40:20 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow/shapeEdit/scene.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'scene.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Scene[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      30,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
       7,    6,    6,    6, 0x05,
      43,   25,    6,    6, 0x05,
      75,    6,    6,    6, 0x05,
     126,  104,    6,    6, 0x05,

 // slots: signature, parameters, type, tag, flags
     184,  176,    6,    6, 0x08,
     199,  176,    6,    6, 0x08,
     217,  176,    6,    6, 0x08,
     233,  176,    6,    6, 0x08,
     253,  176,    6,    6, 0x08,
     267,  176,    6,    6, 0x08,
     288,  176,    6,    6, 0x08,
     309,  176,    6,    6, 0x08,
     328,  176,    6,    6, 0x08,
     346,  176,    6,    6, 0x08,
     362,  176,    6,    6, 0x08,
     381,  376,    6,    6, 0x08,
     411,  405,    6,    6, 0x08,
     431,  376,    6,    6, 0x08,
     455,  376,    6,    6, 0x08,
     481,  376,    6,    6, 0x08,
     507,    6,    6,    6, 0x08,
     523,    6,    6,    6, 0x08,
     548,  543,    6,    6, 0x08,
     577,  572,    6,    6, 0x08,
     602,  376,    6,    6, 0x08,
     637,  627,    6,    6, 0x08,
     660,  627,    6,    6, 0x08,
     681,  627,    6,    6, 0x08,
     707,    6,    6,    6, 0x08,
     720,    6,    6,    6, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Scene[] = {
    "Scene\0\0noSelectedItems()\0penItem,brushItem\0"
    "existSelectedItems(QPen,QBrush)\0"
    "noSelectedTextPictureItems()\0"
    "penItem,fontItem,name\0"
    "existSelectedTextPictureItems(QPen,QFont,QString)\0"
    "checked\0drawLine(bool)\0drawEllipse(bool)\0"
    "drawCurve(bool)\0drawRectangle(bool)\0"
    "addText(bool)\0addDynamicText(bool)\0"
    "addTextPicture(bool)\0addPointPort(bool)\0"
    "addLinePort(bool)\0addStylus(bool)\0"
    "addNone(bool)\0text\0changePenStyle(QString)\0"
    "width\0changePenWidth(int)\0"
    "changePenColor(QString)\0"
    "changeBrushStyle(QString)\0"
    "changeBrushColor(QString)\0changePalette()\0"
    "changeFontPalette()\0font\0"
    "changeFontFamily(QFont)\0size\0"
    "changeFontPixelSize(int)\0"
    "changeFontColor(QString)\0isChecked\0"
    "changeFontItalic(bool)\0changeFontBold(bool)\0"
    "changeFontUnderline(bool)\0deleteItem()\0"
    "clearScene()\0"
};

const QMetaObject Scene::staticMetaObject = {
    { &QGraphicsScene::staticMetaObject, qt_meta_stringdata_Scene,
      qt_meta_data_Scene, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Scene::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Scene::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Scene::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Scene))
        return static_cast<void*>(const_cast< Scene*>(this));
    return QGraphicsScene::qt_metacast(_clname);
}

int Scene::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsScene::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: noSelectedItems(); break;
        case 1: existSelectedItems((*reinterpret_cast< const QPen(*)>(_a[1])),(*reinterpret_cast< const QBrush(*)>(_a[2]))); break;
        case 2: noSelectedTextPictureItems(); break;
        case 3: existSelectedTextPictureItems((*reinterpret_cast< const QPen(*)>(_a[1])),(*reinterpret_cast< const QFont(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 4: drawLine((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: drawEllipse((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: drawCurve((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: drawRectangle((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: addText((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: addDynamicText((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: addTextPicture((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: addPointPort((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: addLinePort((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: addStylus((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: addNone((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 15: changePenStyle((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 16: changePenWidth((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: changePenColor((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 18: changeBrushStyle((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 19: changeBrushColor((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 20: changePalette(); break;
        case 21: changeFontPalette(); break;
        case 22: changeFontFamily((*reinterpret_cast< const QFont(*)>(_a[1]))); break;
        case 23: changeFontPixelSize((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 24: changeFontColor((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 25: changeFontItalic((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 26: changeFontBold((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 27: changeFontUnderline((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 28: deleteItem(); break;
        case 29: clearScene(); break;
        default: ;
        }
        _id -= 30;
    }
    return _id;
}

// SIGNAL 0
void Scene::noSelectedItems()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void Scene::existSelectedItems(QPen const & _t1, QBrush const & _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Scene::noSelectedTextPictureItems()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void Scene::existSelectedTextPictureItems(QPen const & _t1, QFont const & _t2, QString const & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE

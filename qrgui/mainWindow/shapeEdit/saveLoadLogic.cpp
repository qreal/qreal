#include "saveLoadLogic.h"

using namespace qReal;
using namespace qReal::shapeEdit;

SaveLoadLogic::SaveLoadLogic(IShapeEdit *parent, Scene *scene, const bool isUsingTypedPorts) :
    mParent(parent)
  , mScene(scene)
  , mIsUsingTypedPorts(isUsingTypedPorts)
  , mParser(XmlLoader(scene))
{}

SaveLoadLogic::~SaveLoadLogic() {}

void SaveLoadLogic::loadFromFile(const QString &fileName)
{
    if (fileName.isEmpty()) {
        return;
    }
    mParser.readFile(fileName);
}

void SaveLoadLogic::loadFromText(const QString &text)
{
    if (text.isEmpty()) {
        return;
    }
    mParser.readString(text);
}

void SaveLoadLogic::save()
{
    doSave();
    emit mParent->needUpdate();
}

void SaveLoadLogic::saveToXml(const QString &fileName)
{
    if (fileName.isEmpty()) {
        return;
    }

    utils::OutFile file(fileName);
    QDomDocument doc = generateDom();
    file() << mXmlTitle;
    file() << doc.toString(4);
    file() << "\n";
}

void SaveLoadLogic::saveAsPicture(const QString &fileName)
{
    if (fileName.isEmpty()) {
        return;
    }

    QRectF sceneRect = mScene->itemsBoundingRect();
    QImage image(sceneRect.size().toSize(), QImage::Format_RGB32);
    QPainter painter(&image);

    QBrush brush(Qt::SolidPattern);
    brush.setColor(Qt::white);
    painter.setBrush(brush);
    painter.setPen(QPen(Qt::black));
    painter.drawRect(sceneRect);

    mScene->render(&painter);
    image.save(fileName);
}

QStringList SaveLoadLogic::loadPortTypes()
{
    QStringList result;
    result << "NonTyped";

    doLoadPortTypes(result);
    return result;
}

QMap<QString, PropertyInfo> SaveLoadLogic::loadProperties()
{
    QMap<QString, PropertyInfo> result;
    doLoadProperties(result);
    return result;
}

void SaveLoadLogic::doSave() {}
void SaveLoadLogic::doLoadPortTypes(QStringList &result) {Q_UNUSED(result)}
void SaveLoadLogic::doLoadProperties(QMap<QString, PropertyInfo> &result) {Q_UNUSED(result)}

QDomDocument SaveLoadLogic::generateDom()
{
    QDomDocument doc;
    QDomElement graphics = doc.createElement("graphics");
    doc.appendChild(graphics);

    QList<QDomElement> list = generateGraphics(doc);
    foreach (QDomElement domItem, list) {
        graphics.appendChild(domItem);
    }

    return doc;
}

QList<QDomElement> SaveLoadLogic::generateGraphics(QDomDocument &doc)
{
    QDomElement picture = doc.createElement("picture");
    QDomElement label = doc.createElement("labels");
    QDomElement ports = doc.createElement("ports");

    QRect sceneBoundingRect = mScene->realItemsBoundingRect();
    QPoint mTopLeftPicture = sceneBoundingRect.topLeft();
    QList<QGraphicsItem *> list = mScene->items();

    foreach (QGraphicsItem *graphicsItem, list) {

        Item* item = dynamic_cast<Item*>(graphicsItem);
        if (item != nullptr) {

            QPair<QDomElement, Item::DomElementTypes> genItem = item->generateDom(doc, mTopLeftPicture);

            QDomElement domItem = genItem.first;
            Item::DomElementTypes domType = genItem.second;
            switch (domType) {
            case Item::pictureType:
                picture.appendChild(domItem);
                break;
            case Item::labelType:
                label.appendChild(domItem);
                break;
            case Item::portType:
                ports.appendChild(domItem);
                break;
            default:
                break;
            }
        }
    }

    picture.setAttribute("sizex", static_cast<int>(sceneBoundingRect.width() + 1));
    picture.setAttribute("sizey", static_cast<int>(sceneBoundingRect.height() + 1));

    QList<QDomElement> domList;
    domList.push_back(picture);
    domList.push_back(label);
    domList.push_back(ports);

    return domList;
}

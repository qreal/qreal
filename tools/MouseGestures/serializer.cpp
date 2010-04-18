#include <QXmlStreamWriter>
#include "adopter.h"
#include "serializer.h"
#include "GeometricForms.h"
#include "math.h"

static const QString lineKey = "line";
static const QString ellipseKey = "ellipse";
static const QString nodeKey = "node";
static const QString nodeNameKey = "name";
static const QString pathKey = "path";
static const QString gestureKey = "gesture";
static const QString noAttributeKey = "no attribute with such name";
static const QString x1Key = "x1";
static const QString x2Key = "x2";
static const QString y1Key = "y1";
static const QString y2Key = "y2";
static const double pi = 3.141592;
static const int pointsOnEllipse = 16;

Serializer::Serializer(QString const & pathToFile)
{
    mPathToFile = pathToFile;
    mDomElement = getFirstDomElement();
}

QDomElement Serializer::getFirstDomElement()
{
    QFile file(mPathToFile);
    QDomDocument doc("document");
    if (file.open(QIODevice::ReadWrite))
    {
        doc.setContent(&file);
        file.close();
    }
    return doc.documentElement();
}

EntityVector Serializer::parseXml()
{
    EntityVector entities;
    QDomNodeList nodes = mDomElement.elementsByTagName(nodeKey);
    for (unsigned i = 0; i < nodes.length(); i++)
    {
        QDomNode node = nodes.at(i);
        QDomElement element = node.toElement();
        if (element.tagName() == nodeKey)
        {
            Entity entity = parseNode(element);
            entities.push_back(entity);
        }
    }
    return entities;
}

Entity Serializer::parseNode(QDomElement const & domElement)
{
    QString name = domElement.attribute(nodeNameKey, noAttributeKey);
    PathVector components;
    QDomNodeList geometricElements = domElement.elementsByTagName(lineKey);
    for (int i = 0; i < geometricElements.size(); i++)
    {
        QDomElement geometricElement = geometricElements.at(i).toElement();
        components.push_back(getPoints(geometricElement));
    }
    geometricElements = domElement.elementsByTagName(ellipseKey);
    for (int i = 0; i < geometricElements.size(); i++)
    {
        QDomElement geometricElement = geometricElements.at(i).toElement();
        QList<QPoint> diam = getPoints(geometricElement);
        if (diam.size() > 1)
            components.push_back(getEllipsePath(diam[0], diam[1]));
    }
    Entity entity;
    entity.name = name;
    entity.components = components;
    return entity;
}

QList<QPoint> Serializer::getPoints(const QDomElement &geometricElement)
{
    bool isValid;
    QPoint point1(geometricElement.attribute(x1Key, noAttributeKey).toInt(&isValid, 10),
                  geometricElement.attribute(y1Key, noAttributeKey).toInt(&isValid, 10));
    QPoint point2(geometricElement.attribute(x2Key, noAttributeKey).toInt(&isValid, 10),
                  geometricElement.attribute(y2Key, noAttributeKey).toInt(&isValid, 10));
    QList<QPoint> component;
    component.push_back(point1);
    component.push_back(point2);
    return component;
}

QList<QPoint> Serializer::getEllipsePath(const QPoint &point1, const QPoint &point2)
{
    QList<QPoint> ellipse;
    QPoint centre = (point1 + point2) / 2;
    int diam = static_cast<int>(sqrt(pow((point1 - point2).x(), 2) + pow((point1 - point2).y(), 2)));
    for (int i = 0; i < pointsOnEllipse; i++)
    {
        int x = static_cast<int>(diam * cos(2 * pi * i / pointsOnEllipse) / 2);
        int y = static_cast<int>(diam * sin(2 * pi * i / pointsOnEllipse) / 2);
        ellipse.push_back(centre + QPoint(x, y));
    }
    ellipse.push_back(QPoint(centre.x() + diam / 2, centre.y()));
    return ellipse;
}

void Serializer::serialize(const Objects &objects)
{
    QFile file(this->mPathToFile);
    if (file.open(QFile::ReadWrite))
    {
        QXmlStreamWriter xml;
        xml.setDevice(&file);
        xml.writeStartDocument();
        xml.writeDTD("<!DOCTYPE xml>");
        xml.writeStartElement("xml");
        xml.writeAttribute("version", "1.0");
        foreach (Object object, objects)
        {
            xml.writeEmptyElement(gestureKey);
            xml.writeAttribute(nodeNameKey, object.name);
            xml.writeAttribute(pathKey, Adopter::pathToString(object.path));
        }
        xml.writeEndElement();
        xml.writeEndDocument();
    }
    file.close();
}

Objects Serializer::deserialize()
{
    Objects objects;
    QDomNodeList nodes = this->mDomElement.elementsByTagName(gestureKey);
    for (int i = 0; i < nodes.size(); i++)
    {
        QDomElement element = nodes.at(i).toElement();
        QString name = element.attribute(nodeNameKey, "");
        QString path = element.attribute(pathKey, "");
        Object object(name, Adopter::stringToPath(path));
        objects.push_back(object);
    }
    return objects;
}

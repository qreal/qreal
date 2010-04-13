#include "xmlparser.h"
#include "GeometricForms.h"
#include "math.h"

static const QString lineKey = "line";
static const QString ellipseKey = "ellipse";
static const QString nodeKey = "node";
static const QString nodeNameKey = "name";
static const QString noAttributeKey = "no attribute with such name";
static const QString x1Key = "x1";
static const QString x2Key = "x2";
static const QString y1Key = "y1";
static const QString y2Key = "y2";
static const double pi = 3.141592;
static const int pointsOnEllipse = 16;

XmlParser::XmlParser(QString const & pathToFile)
{
    mPathToFile = pathToFile;
    initializeFirstDomElement();
}

void XmlParser::initializeFirstDomElement()
{
    QFile file(mPathToFile);
    QDomDocument doc("document");
    if (!file.open(QIODevice::ReadWrite))
    {
        return;
    }
    if (!doc.setContent(&file))
    {
        file.close();
        return;
    }
    mDomElement = doc.documentElement();
    file.close();
}

EntityVector XmlParser::parseXml()
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

Entity XmlParser::parseNode(QDomElement const & domElement)
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
        {
            components.push_back(getEllipsePath(diam[0], diam[1]));
        }
    }
    Entity entity;
    entity.name = name;
    entity.components = components;
    return entity;
}

QList<QPoint> XmlParser::getPoints(const QDomElement &geometricElement)
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

QList<QPoint> XmlParser::getEllipsePath(const QPoint &point1, const QPoint &point2)
{
    QList<QPoint> ellipse;
    QPoint centre = (point1 + point2) / 2;
    int diam = (int)(sqrt(pow((point1 - point2).x(), 2) + pow((point1 - point2).y(), 2)));
    for (int i = 0; i < pointsOnEllipse; i++)
    {
        int x = (int)(diam * cos(2 * pi * i / pointsOnEllipse) / 2);
        int y = (int)(diam * sin(2 * pi * i / pointsOnEllipse) / 2);
        ellipse.push_back(centre + QPoint(x, y));
    }
    ellipse.push_back(QPoint(centre.x() + diam / 2, centre.y()));
    return ellipse;
}

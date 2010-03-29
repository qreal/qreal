#include "xmlparser.h"
#include "GeometricForms.h"

static const QString lineKey = "line";
static const QString nodeKey = "node";
static const QString nodeNameKey = "name";
static const QString noAttributeKey = "no attribute with such name";
static const QString x1Key = "x1";
static const QString x2Key = "x2";
static const QString y1Key = "y1";
static const QString y2Key = "y2";

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
    Graph lines;
    Line line;
    bool isValid;
    QDomNodeList geometricElements = domElement.elementsByTagName(lineKey);
    for (unsigned i = 0; i < geometricElements.length(); i++)
    {
        QDomNode geometricNode = geometricElements.at(i);
        QDomElement geometricElement = geometricNode.toElement();
        if (geometricElement.tagName() == lineKey)
        {
            QPoint point1(geometricElement.attribute(x1Key, noAttributeKey).toInt(&isValid, 10),
                          geometricElement.attribute(y1Key, noAttributeKey).toInt(&isValid, 10));
            line.point1 = point1;
            QPoint point2(geometricElement.attribute(x2Key, noAttributeKey).toInt(&isValid, 10),
                          geometricElement.attribute(y2Key, noAttributeKey).toInt(&isValid, 10));
            line.point2 = point2;
            lines.push_back(line);
        }
    }
    Entity entity;
    entity.name = name;
    entity.lines = lines;
    return entity;
}

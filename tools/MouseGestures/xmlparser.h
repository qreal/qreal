#pragma once
#include <GeometricForms.h>
#include <QList>
#include <QString>
#include <QFile>
#include <QDomElement>
#include <QDomDocument>

// парсит xml - распознает имена объектов и их графическое описание
class XmlParser
{

public:
    XmlParser(QString const & pathToFile);
    EntityVector parseXml();

private:
    void initializeFirstDomElement();
    Entity parseNode(QDomElement const & domElement);
    QList<QPoint> getEllipsePath(QPoint const &point1, QPoint const &point2);
    QString mPathToFile;
    QDomElement mDomElement;
    QList<QPoint> getPoints(QDomElement const & domElement);
};

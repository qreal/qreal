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
    QString mPathToFile;
    QDomElement mDomElement;
};

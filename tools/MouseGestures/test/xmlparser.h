#pragma once
#include <QPair>
#include <QPoint>
#include <QMap>
#include <QDomNodeList>
#include "GeometricForms.h"

class XmlParser
{
public:
    static QMap<QString, UsersGestures> parseXml(QString const & file);
    static void save(const QMap<QString, UsersGestures> &map, QString const & fileName);
    static void saveMore(const QMap<QString, UsersGestures> &map, QString const & fileName);

private:
    static QList<QString> getUsersGestures(const QDomNodeList & list);
    static void saveDocument(const QMap<QString, UsersGestures> &map, QDomDocument doc, QString const & file);
};


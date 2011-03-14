#pragma once
#include <QPair>
#include <QPoint>
#include <QMap>
#include <QDomNodeList>

typedef QPair<QString, QList<QPoint> > GestureObject;
typedef QPair<QList<QPoint>, QList<QString> > UsersGestures;

class XmlParser
{
public:
        static QMap<QString, UsersGestures> parseXml(QString const & file);
        static void save(const QMap<QString, UsersGestures> &map, QString const & fileName);

private:
	static QList<QString> getUsersGestures(const QDomNodeList & list);
};


#pragma once
#include <QPair>
#include <QPoint>
#include <QMap>
#include <QDomNodeList>

//todo:: по-хорошему вынести в отдельный файл
typedef QPair<QString, QList<QPoint> > GestureObject;
typedef QPair<QList<QPoint>, QList<QString> > UsersGestures;

class XmlParser
{
public:
	static QMap<QString, UsersGestures > parseXml();
	static void save(const QMap<QString, QPair<QString, QList<QString> > > & map);
	static void saveResults(const QMap<QString, QPair<double, double> > & results);

private:
	static QList<QString> getUsersGestures(const QDomNodeList & list);
};


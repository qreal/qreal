#include "xmlparser.h"
#include "adopter.h"
#include <QFile>
#include <QDomElement>
#include <QStringList>
#include <QTextStream>

const QString pathToFile = "usersGestures.xml";

//TODO:: вынести в константы
QMap<QString, QPair<QList<QPoint>, QList<QString> > > XmlParser::parseXml()
{
	QMap<QString, QPair<QList<QPoint>, QList<QString> > > gestures;
	QFile file(pathToFile);
	QDomDocument doc("document");
	if (!file.open(QIODevice::ReadWrite))
		return gestures;
	doc.setContent(&file);
	file.close();
	QDomNodeList elements = doc.elementsByTagName("gesture");
	for (int i = 0; i < elements.size(); i++)
	{
		QDomElement element = elements.at(i).toElement();
		QString path = element.attribute("idealPath");
		QString name = element.attribute("name");
		QDomNodeList paths = element.elementsByTagName("userPath");
		if (!path.isEmpty())
		{
			QList<QPoint> points = Adopter::stringToPath(path);
			QPair<QList<QPoint>, QList<QString> > usersGestures(points,
														getUsersGestures(paths));
			gestures.insert(name, usersGestures);
		}
	}
	return gestures;
}

QList<QString> XmlParser::getUsersGestures(const QDomNodeList &list)
{
	QList<QString> gestures;
	for (int i = 0; i < list.size(); i++)
	{
		gestures.push_back(list.at(i).toElement().attribute("path", ""));
	}
	return gestures;
}

//map: name -> gestures
void XmlParser::save(const QMap<QString, QPair<QString, QList<QString> > > &map)
{
	QFile file(pathToFile);
	if (!file.open(QFile::ReadWrite))
		return;
	QTextStream textStream(&file);
	QDomDocument doc;
	QDomElement root = doc.createElement("usersGestures");
	doc.appendChild(root);
	foreach (QString name, map.keys())
	{
		QDomElement element = doc.createElement("gesture");
		element.setAttribute("name", name);
		element.setAttribute("idealPath", map[name].first);
		foreach (QString usersGesture, map[name].second)
		{
			QDomElement usersPath = doc.createElement("userPath");
			usersPath.setAttribute("path", usersGesture);
			element.appendChild(usersPath);
		}
		root.appendChild(element);
	}
	doc.save(textStream, 2);
	file.close();
}

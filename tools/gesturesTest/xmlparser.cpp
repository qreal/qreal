#include "xmlparser.h"
#include "adopter.h"
#include <QFile>
#include <QDomElement>
#include <QStringList>
#include <QTextStream>

const QString pathToFile = "usersGestures.xml";
const QString matchingAlgorithmsFile = "matchingAlgorithms.txt";

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

void XmlParser::saveResults(const QMap<QString, QList<double> > & results)
{
	QFile file(matchingAlgorithmsFile);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;
	QTextStream textStream(&file);
	foreach (QString object, results.keys())
	{
		textStream << object << " \n"
				<< "gestures: " << results[object].at(0) << " \n"
				<< "rect: " << " \n"
				<< QString("	recoznized: %1%").arg(results[object].at(1)) << " \n"
				<< QString("	false positives: %1%").arg(results[object].at(2)) << " \n"
				<< QString("	not recognized: %1%").arg(results[object].at(3)) << " \n"
				<< "qt: " << " \n"
				<< QString("	recognized: %1%").arg(results[object].at(4)) << " \n"
				<< QString("	false positives %1%").arg(results[object].at(5)) << " \n"
				<< QString("	not recognized: %1%").arg(results[object].at(6)) << " \n"
				 << "============================\n";
	}
	file.close();
}

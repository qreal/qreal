#include "xmlparser.h"
#include "adopter.h"
#include <QFile>
#include <QDomElement>
#include <QStringList>
#include <QTextStream>

const QString pathToFile = "../gesturesTest/usersGestures.xml";
const QString matchingAlgorithmsFile = "matchingAlgorithms.txt";

//TODO:: вынести в константы
QMap<QString, UsersGestures> XmlParser::parseXml()
{
	QMap<QString, QPair<QList<QPoint>, QList<QString> > > gestures;
	QFile file(pathToFile);

	if (!file.exists())
		file.setFileName("../" + pathToFile);

	QDomDocument doc("document");
	if (!file.open(QIODevice::ReadWrite))
		return gestures;
	doc.setContent(&file);
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
        file.close();
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
//void XmlParser::save(const QMap<QString, QPair<QString, QList<QString> > > &map)
//{
//	QFile file(pathToFile);
//	if (!file.open(QFile::ReadWrite))
//		return;
//	QTextStream textStream(&file);
//	QDomDocument doc;
//	QDomElement root = doc.createElement("usersGestures");
//	doc.appendChild(root);
//	foreach (QString name, map.keys())
//	{
//		QDomElement element = doc.createElement("gesture");
//		element.setAttribute("name", name);
//		element.setAttribute("idealPath", map[name].first);
//		foreach (QString usersGesture, map[name].second)
//		{
//			QDomElement usersPath = doc.createElement("userPath");
//			usersPath.setAttribute("path", usersGesture);
//			element.appendChild(usersPath);
//		}
//		root.appendChild(element);
//	}
//	doc.save(textStream, 2);
//	file.close();
//}

//void XmlParser::saveResults(const QMap<QString, QList<double> > & results
//		, int allGestures, int qtGestures, int rectGestures, int chaosGestures)
//{
//	QFile file(matchingAlgorithmsFile);
//	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
//		return;
//	QTextStream textStream(&file);

//	QString const recognized = "	recognized: %1%";
//	QString const falsePositives = "	false positives: %1%";
//	QString const notRecognized = "	not recognized: %1%";

//	foreach (QString object, results.keys())
//	{
//		textStream << object << " \n"
//				<< "gestures: " << results[object].at(0) << " \n"
//				<< "rect: " << " \n"
//				<< recognized.arg(results[object].at(1)) << " \n"
//				<< falsePositives.arg(results[object].at(2)) << " \n"
//				<< notRecognized.arg(results[object].at(3)) << " \n"
//				<< "qt: " << " \n"
//				<< recognized.arg(results[object].at(4)) << " \n"
//				<< falsePositives.arg(results[object].at(5)) << " \n"
//				<< notRecognized.arg(results[object].at(6)) << " \n"
//				<< "chaos: " << " \n"
//				<< recognized.arg(results[object].at(7)) << " \n"
//				<< falsePositives.arg(results[object].at(8)) << " \n"
//				<< notRecognized.arg(results[object].at(9)) << " \n"
//				<< "============================\n";
//	}

//	textStream << "Total: " << QString::number(allGestures) << " gestures \n"
//			<< "rect recognized: " << QString::number(rectGestures) << " \n"
//			<< "qt recognized: " << QString::number(qtGestures) << " \n"
//			<< "chaos recognized: " << QString::number(chaosGestures) << " \n"
//			;

//	file.close();
//}

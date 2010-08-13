#include "xmlparser.h"
#include <QFile>
#include <QDomElement>
#include <QStringList>

const QString comma = ", ";
const QString pointDelimeter = " : ";

QList<GestureObject> XmlParser::parseXml(const QString &filePath)
{
	QList<GestureObject> gestures;
	QFile file(filePath);
	QDomDocument doc("document");
	if (!file.open(QIODevice::ReadWrite))
		return gestures;
	doc.setContent(&file);
	file.close();
	QDomNodeList elements = doc.elementsByTagName("node");
	for (int i = 0; i < elements.size(); i++)
	{
		QDomElement element = elements.at(i).toElement();
		QString path = element.attribute("path");
		QString name = element.attribute("name");
		if (!path.isEmpty())
		{
			QList<QPoint> points = stringToPath(path);
			QPair<QString, QList<QPoint> > gesture(name, points);
			gestures.push_back(gesture);
		}
	}
	return gestures;
}

QList<QPoint> XmlParser::stringToPath(QString const &valueStr)
{
	QStringList points = valueStr.split(pointDelimeter, QString::SkipEmptyParts);
	QList<QPoint> result;
	foreach (QString str, points)
	{
		QPoint point = stringToPoint(str);
		result.push_back(point);
	}
	return result;
}

QPoint XmlParser::stringToPoint(QString const &str)
{
	bool isInt;
	int x = str.section(comma, 0, 0).toInt(&isInt, 0);
	int y = str.section(comma, 1, 1).toInt(&isInt, 0);
	return QPoint(x, y);
}

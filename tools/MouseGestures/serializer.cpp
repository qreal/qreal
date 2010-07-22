#include <QXmlStreamWriter>
#include <QTextStream>
#include "adopter.h"
#include "serializer.h"
#include "GeometricForms.h"
#include "math.h"

static const QString lineKey = "line";
static const QString ellipseKey = "ellipse";
static const QString rectangleKey = "rectangle";
static const QString nodeKey = "node";
static const QString nodeNameKey = "name";
static const QString pathKey = "path";
static const QString x1Key = "x1";
static const QString x2Key = "x2";
static const QString y1Key = "y1";
static const QString y2Key = "y2";
static const double pi = 3.141592;
static const int pointsOnEllipse = 16;

Serializer::Serializer(QString const & pathToFile)
{
	mPathToFile = pathToFile;
	mDomElement = getFirstDomElement();
}

QDomElement Serializer::getFirstDomElement()
{
	QFile file(mPathToFile);
	QDomDocument doc("document");
	if (file.open(QIODevice::ReadWrite))
	{
		doc.setContent(&file);
		file.close();
	}
	return doc.documentElement();
}

EntityVector Serializer::deserialize()
{
	EntityVector entities;
	QDomNodeList nodes = mDomElement.elementsByTagName(nodeKey);
	for (unsigned i = 0; i < nodes.length(); i++)
	{
		QDomNode node = nodes.at(i);
		QDomElement element = node.toElement();
		Entity entity = parseNode(element);
		entities.push_back(entity);
	}
	return entities;
}

Entity Serializer::parseNode(QDomElement const & domElement)
{
	QString name = domElement.attribute(nodeNameKey, "");
	PathVector components;
	QString path = domElement.attribute(pathKey, "");
	if (!path.isEmpty())
	{
		components.push_back(Adopter::stringToPath(path));
	}
	else
	{
		QDomNodeList geometricElements = domElement.elementsByTagName(lineKey);
		for (int i = 0; i < geometricElements.size(); i++)
		{
			QDomElement geometricElement = geometricElements.at(i).toElement();
			components.push_back(getPoints(geometricElement));
		}
		geometricElements = domElement.elementsByTagName(ellipseKey);
		for (int i = 0; i < geometricElements.size(); i++)
		{
			QDomElement geometricElement = geometricElements.at(i).toElement();
			QList<QPoint> diam = getPoints(geometricElement);
			if (diam.size() > 1)
				components.push_back(getEllipsePath(diam[0], diam[1]));
		}
		geometricElements = domElement.elementsByTagName(rectangleKey);
		for (int i = 0; i < geometricElements.size(); i++)
		{
			QDomElement geometricElement = geometricElements.at(i).toElement();
			components.push_back(getRectanglePath(geometricElement));
		}
	}
	Entity entity;
	entity.name = name;
	entity.components = components;
	return entity;
}

QList<QPoint> Serializer::getPoints(const QDomElement &geometricElement)
{
	bool isValid;
	QPoint point1(geometricElement.attribute(x1Key, "").toInt(&isValid, 10),
				  geometricElement.attribute(y1Key, "").toInt(&isValid, 10));
	QPoint point2(geometricElement.attribute(x2Key, "").toInt(&isValid, 10),
				  geometricElement.attribute(y2Key, "").toInt(&isValid, 10));
	QList<QPoint> component;
	component.push_back(point1);
	component.push_back(point2);
	return component;
}

QList<QPoint> Serializer::getRectanglePath(const QDomElement &domElement)
{
	int x1 = domElement.attribute(x1Key, "").toInt();
	int y1 = domElement.attribute(y1Key, "").toInt();
	int x2 = domElement.attribute(x2Key, "").toInt();
	int y2 = domElement.attribute(y2Key, "").toInt();
	QList<QPoint> component;
	component.push_back(QPoint(x1, y1));
	component.push_back(QPoint(x2, y1));
	component.push_back(QPoint(x2, y2));
	component.push_back(QPoint(x1, y2));
	component.push_back(QPoint(x1, y1));
	return component;
}

QList<QPoint> Serializer::getEllipsePath(const QPoint &point1, const QPoint &point2)
{
	QList<QPoint> ellipse;
	QPoint centre = (point1 + point2) / 2;
	int diam = static_cast<int>(sqrt(pow((point1 - point2).x(), 2) + pow((point1 - point2).y(), 2)));
	for (int i = 0; i < pointsOnEllipse; i++)
	{
		int x = static_cast<int>(diam * cos(2 * pi * i / pointsOnEllipse) / 2);
		int y = static_cast<int>(diam * sin(2 * pi * i / pointsOnEllipse) / 2);
		ellipse.push_back(centre + QPoint(x, y));
	}
	ellipse.push_back(QPoint(centre.x() + diam / 2, centre.y()));
	return ellipse;
}

void Serializer::serialize(const Objects &objects)
{
	QFile file(this->mPathToFile);
	if (file.open(QFile::ReadWrite))
	{
		QTextStream textStream(&file);
		QDomNodeList nodes = mDomElement.elementsByTagName(nodeKey);
		for (unsigned i = 0; i < nodes.length(); i++)
		{
			QDomNode node = nodes.at(i);
			QDomElement element = node.toElement();
			foreach (Object object, objects)
			{
				if (object.name == element.attribute(nodeNameKey, ""))
				{
					QString path = Adopter::pathToString(object.path);
					if (!path.isEmpty())
						element.setAttribute(pathKey, path);
				}
			}
		}
		mDomElement.save(textStream, 2);
	}
	file.close();
}

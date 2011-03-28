#include <QXmlStreamWriter>
#include <QTextStream>
#include "adopter.h"
#include "serializer.h"
#include "GeometricForms.h"
#include "math.h"
#include "figures.h"

static const QString lineKey = "line";
static const QString ellipseKey = "ellipse";
static const QString rectangleKey = "rectangle";
static const QString nodeKey = "node";
static const QString nodeNameKey = "name";
static const QString pathKey = "path";


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
                components = Parser::stringToPath(path);
	}
	else
	{
		QDomNodeList geometricElements = domElement.elementsByTagName(lineKey);
		for (int i = 0; i < geometricElements.size(); i++)
		{
			QDomElement geometricElement = geometricElements.at(i).toElement();
			Line line(geometricElement);
			components.push_back(line.getCurve());
		}
		geometricElements = domElement.elementsByTagName(ellipseKey);
		for (int i = 0; i < geometricElements.size(); i++)
		{
			QDomElement geometricElement = geometricElements.at(i).toElement();
			Ellipse ellipse(geometricElement);
			components.push_back(ellipse.getCurve());
		}
		geometricElements = domElement.elementsByTagName(rectangleKey);
		for (int i = 0; i < geometricElements.size(); i++)
		{
			QDomElement geometricElement = geometricElements.at(i).toElement();
			Rectangle rectangle(geometricElement);
			components.push_back(rectangle.getCurve());
		}
	}
	Entity entity;
	entity.first = name;
	entity.second = components;
	return entity;
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
					QString path = Parser::pathToString(object.path);
					if (!path.isEmpty())
						element.setAttribute(pathKey, path);
				}
			}
		}
		mDomElement.save(textStream, 2);
	}
	file.close();
}

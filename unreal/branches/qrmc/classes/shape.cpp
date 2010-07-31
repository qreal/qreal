#include "shape.h"

#include <QtCore/QDebug>

Shape::Shape(const QString &shape)
{
	init(shape);
}

void Shape::init(const QString &shape)
{

	QString error = "";
	int errorLine = 0;
	int errorCol = 0;
	QDomDocument doc;
	if (!doc.setContent(shape, false, &error, &errorLine, &errorCol))
		return;

	QDomElement graphics = doc.firstChildElement("graphics");

	int start = shape.indexOf("<picture");
	int end = shape.indexOf("</picture>");
	mPicture = shape.mid(start, end - start + QString("</picture>").size()); // zomg, WTF?!

	mWidth = graphics.firstChildElement("picture").attribute("sizex", "88").toInt();
	mHeight = graphics.firstChildElement("picture").attribute("sizey", "88").toInt();

	initLabels(graphics);
	initPorts(graphics);
}

void Shape::initLabels(QDomElement const &graphics)
{
	int count = 1;
	for (QDomElement element = graphics.firstChildElement("labels").firstChildElement("label");
		!element.isNull();
		element = element.nextSiblingElement("label"))
	{
		Label *label = new Label();
		if (!label->init(element, count, true))
			delete label;
		else {
			mLabels.append(label);
			++count;
		}
	}
	return;

}

void Shape::initPorts(QDomElement const &graphics)
{
	QDomElement portsElement = graphics.firstChildElement("ports");
	if (portsElement.isNull())
		return;
	initPointPorts(portsElement);
	initLinePorts(portsElement);

	return;
}

void Shape::initPointPorts(QDomElement const &portsElement)
{
	for (QDomElement portElement = portsElement.firstChildElement("pointPort");
		!portElement.isNull();
		portElement = portElement.nextSiblingElement("pointPort"))
	{
		Port *pointPort = new PointPort();
		if (!pointPort->init(portElement, mWidth, mHeight)) {
			delete pointPort;
			return;
		}
		mPorts.append(pointPort);
	}
	return;
}

void Shape::initLinePorts(QDomElement const &portsElement)
{
	for (QDomElement portElement = portsElement.firstChildElement("linePort");
		!portElement.isNull();
		portElement = portElement.nextSiblingElement("linePort"))
	{
		Port *linePort = new LinePort();
		if (!linePort->init(portElement, mWidth, mHeight))
		{
			delete linePort;
			return;
		}
		mPorts.append(linePort);
	}
	return;
}


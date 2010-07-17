#pragma once

#include <QtXml/QDomDocument>

#include "scene.h"
#include "item.h"

class XmlLoader
{
public:
	XmlLoader(Scene *scene);
	void readString(const QString &text);
	void readFile(const QString &fileName);

private:
	QDomDocument mDocument;
	Scene *mScene;
	QPointF mDrift;

	void readDocument();
	void readGraphics(QDomElement const &graphic);
	void readPicture(QDomElement const &picture);
	void readLabels(QDomElement const &label);
	void readPorts(QDomElement const &port);
	QRectF readRectOfXandY(QDomElement const &docItem);
	QPointF readXandY(QDomElement const &docItem);
	void readLine(QDomElement const &line);
	void readEllipse(QDomElement const &ellipse);
	void readArch(QDomElement const &arch);
	void readRectangle(QDomElement const &rectangle);
	void readStylus(QDomElement const &stylus);
	void readPath(QDomElement const &element);
	void readLabel(QDomElement const &label);
	void readLinePort(QDomElement const &linePort);
	void readPointPort(QDomElement const &pointPort);

	bool isNotLCMZ(QString str, int i);
};

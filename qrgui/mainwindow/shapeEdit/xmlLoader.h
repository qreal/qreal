#pragma once

#include <QtXml/QDomDocument>

#include "scene.h"
#include "item.h"

const int distanceFigure = 50;

class XmlLoader
{
public:
	XmlLoader(Scene *scene);
	void readString(const QString &text);
	void readFile(const QString &fileName);

private:
	QDomDocument mDocument;
	Scene *mScene;
	QPoint mDrift;
	QList<QPair<Item::ScalingPointState, QColor> > mListScalePoint;
	int mStrY;
	int mStrX;
	int mFloorY;
	bool mReadFile;

	void initListScalePoint();
	void readDocument();
	void readGraphics(QDomElement const &graphic);
	void readPicture(QDomElement const &picture);
	void readLabels(QDomElement const &label);
	void readPorts(QDomElement const &port);
	void readImage(QDomElement const &image);
	void changeScaleColor(int i);
	void checkScale(QPair<QString, bool> pointX1, QPair<QString, bool> pointX2, QPair<QString, bool> pointY1, QPair<QString, bool> pointY2);
	QPair<QString, bool> readScaleCoord(QString point, QDomElement const &docItem);
	QPair<QPointF, QPointF> calcLineOfXandY(QPair<QString, bool> pointX1, QPair<QString, bool> pointX2, QPair<QString, bool> pointY1, QPair<QString, bool> pointY2);
	QPair<QPointF, QPointF> readLineOfXandY(QDomElement const &docItem);
	QPair<QPointF, QPointF> readLinePortOfXandY(QDomElement const &start, QDomElement const &end);
	QRectF readRectOfXandY(QDomElement const &docItem);
	QPointF readXandY(QDomElement const &docItem);
	void readLine(QDomElement const &line);
	void readEllipse(QDomElement const &ellipse);
	void readArch(QDomElement const &arch);
	void readRectangle(QDomElement const &rectangle);
	void readStylus(QDomElement const &stylus);
	void readPath(QDomElement const &element);
	void readCurve(QDomElement const &element);
	void readText(QDomElement const &element);
	void readLabel(QDomElement const &label);
	void readLinePort(QDomElement const &linePort);
	void readPointPort(QDomElement const &pointPort);

	bool isNotLCMZ(QString str, int i);
};

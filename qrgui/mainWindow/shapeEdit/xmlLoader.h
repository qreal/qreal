/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <QtXml/QDomDocument>

#include "mainWindow/shapeEdit/scene.h"
#include "mainWindow/shapeEdit/item.h"

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
	void readGraphics(const QDomElement &graphic);
	void readPicture(const QDomElement &picture);
	void readLabels(const QDomElement &label);
	void readPorts(const QDomElement &port);
	void readImage(const QDomElement &image);
	void changeScaleColor(int i);
	void checkScale(QPair<QString, bool> pointX1, QPair<QString, bool> pointX2
			, QPair<QString, bool> pointY1, QPair<QString, bool> pointY2);
	QPair<QString, bool> readScaleCoord(QString point, const QDomElement &docItem);
	QPair<QPointF, QPointF> calcLineOfXandY(QPair<QString, bool> pointX1, QPair<QString, bool> pointX2
			, QPair<QString, bool> pointY1, QPair<QString, bool> pointY2);
	QPair<QPointF, QPointF> readLineOfXandY(const QDomElement &docItem);
	QPair<QPointF, QPointF> readLinePortOfXandY(const QDomElement &start, const QDomElement &end);
	QRectF readRectOfXandY(const QDomElement &docItem);
	QPointF readXandY(const QDomElement &docItem);
	void readLine(const QDomElement &line);
	void readEllipse(const QDomElement &ellipse);
	void readArch(const QDomElement &arch);
	void readRectangle(const QDomElement &rectangle);
	void readStylus(const QDomElement &stylus);
	void readPath(const QDomElement &element);
	void readCurve(const QDomElement &element);
	void readText(const QDomElement &element);
	void readLabel(const QDomElement &label);
	void readLinePort(const QDomElement &linePort);
	void readPointPort(const QDomElement &pointPort);

	Item::VisibilityCondition readVisibility(const QDomElement &item);

	bool isNotLCMZ(QString str, int i);
};

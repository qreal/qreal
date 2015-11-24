/* Copyright 2015 CyberTech Labs Ltd.
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

#include "utils/canvas/canvas.h"

#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>

#include "utils/canvas/pointObject.h"
#include "utils/canvas/lineObject.h"
#include "utils/canvas/rectangleObject.h"
#include "utils/canvas/ellipseObject.h"
#include "utils/canvas/arcObject.h"
#include "utils/canvas/textObject.h"

using namespace utils;

Canvas::Canvas()
	: mCurrentPenWidth(1)
	, mCurrentPenColor(Qt::black)
{
	qRegisterMetaType<QList<utils::CanvasObject *>>();
	qRegisterMetaType<QList<utils::PointObject *>>();
	qRegisterMetaType<QList<utils::LineObject *>>();
	qRegisterMetaType<QList<utils::RectangleObject *>>();
	qRegisterMetaType<QList<utils::EllipseObject *>>();
	qRegisterMetaType<QList<utils::ArcObject *>>();
	qRegisterMetaType<QList<utils::TextObject *>>();
}

void Canvas::reset()
{
	qDeleteAll(mObjects);
	mObjects.clear();
	mPixels.clear();
	mLines.clear();
	mRectangles.clear();
	mEllipses.clear();
	mArcs.clear();
	mLabels.clear();
	mCurrentPenWidth = 1;
	mCurrentPenColor = Qt::black;
}

const QList<CanvasObject *> &Canvas::objects() const
{
	return mObjects;
}

const QList<PointObject *> &Canvas::pixels() const
{
	return mPixels;
}

const QList<LineObject *> &Canvas::segments() const
{
	return mLines;
}

const QList<RectangleObject *> &Canvas::rectangles() const
{
	return mRectangles;
}

const QList<EllipseObject *> &Canvas::ellipses() const
{
	return mEllipses;
}

const QList<ArcObject *> &Canvas::arcs() const
{
	return mArcs;
}

const QList<TextObject *> &Canvas::labels() const
{
	return mLabels;
}

void Canvas::printText(int x, int y, const QString &text)
{
	TextObject * const object = new TextObject(x, y, text, mCurrentPenColor, mCurrentPenWidth);
	mObjects << object;
	mLabels << object;
}

void Canvas::drawPixel(int x, int y)
{
	PointObject * const object = new PointObject(x, y, mCurrentPenColor, mCurrentPenWidth);
	mObjects << object;
	mPixels << object;
}

void Canvas::drawLine(int x1, int y1, int x2, int y2)
{
	LineObject * const object = new LineObject(QPoint(x1, y1), QPoint(x2, y2), mCurrentPenColor, mCurrentPenWidth);
	mObjects << object;
	mLines << object;
}

void Canvas::drawRect(int x, int y, int width, int height, bool filled)
{
	RectangleObject * const object = new RectangleObject(QRect(x, y, width, height)
			, mCurrentPenColor, mCurrentPenWidth, filled);
	mObjects << object;
	mRectangles << object;
}

void Canvas::drawEllipse(int centerX, int centerY, int semiDiameterX, int semiDiameterY, bool filled)
{
	EllipseObject * const object = new EllipseObject(QPoint(centerX, centerY)
			, semiDiameterX, semiDiameterY, mCurrentPenColor, mCurrentPenWidth, filled);
	mObjects << object;
	mEllipses << object;
}

void Canvas::drawArc(int x, int y, int width, int height, int startAngle, int spanAngle)
{
	ArcObject * const object = new ArcObject(QRect(x, y, width, height)
			, startAngle, spanAngle, mCurrentPenColor, mCurrentPenWidth);
	mObjects << object;
	mArcs << object;
}

void Canvas::setPainterColor(const QColor &color)
{
	mCurrentPenColor = color;
}

void Canvas::setPainterWidth(int penWidth)
{
	mCurrentPenWidth = penWidth;
}

QJsonArray Canvas::toJson() const
{
	QJsonArray result;
	for (const CanvasObject *object : mObjects) {
		result << object->toJson();
	}

	return result;
}

void Canvas::paint(QPainter *painter)
{
	for (CanvasObject * object : mObjects) {
		object->paint(painter);
	}
}

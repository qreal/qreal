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

#pragma once

#include <qrutils/widgets/painterInterface.h>

#include "utils/utilsDeclSpec.h"

namespace utils {

class CanvasObject;
class PointObject;
class LineObject;
class RectangleObject;
class EllipseObject;
class ArcObject;
class TextObject;

/// Represent a field  for drawing shapes on it, something like QGraphicsScene but far more primitive.
class ROBOTS_UTILS_EXPORT Canvas : public qReal::ui::PainterInterface
{
public:
	Canvas();

	void reset() override;

	/// Returns a list of all objects on the canvas.
	const QList<CanvasObject *> &objects() const;

	/// Returns a list of pixels drawn on the canvas.
	const QList<PointObject *> &pixels() const;

	/// Returns a list of segments drawn on the canvas.
	const QList<LineObject *> &segments() const;

	/// Returns a list of rectangles drawn on the canvas.
	const QList<RectangleObject *> &rectangles() const;

	/// Returns a list of ellipses drawn on the canvas.
	const QList<EllipseObject *> &ellipses() const;

	/// Returns a list of arcs drawn on the canvas.
	const QList<ArcObject *> &arcs() const;

	/// Returns a list of text labels drawn on the canvas.
	const QList<TextObject *> &labels() const;

	/// Prints the given text on the canvas in the given coordinates.
	/// The text is replaced with new one when coordinates collision happens.
	virtual void printText(int x, int y, const QString &text);

	/// Draw point on the canvas without redrawing it.
	/// @param x - x coordinate.
	/// @param y - y coordinate.
	virtual void drawPixel(int x, int y);

	/// Draw line on the canvas without redrawing it.
	/// @param x1 - first point's x coordinate.
	/// @param y1 - first point's y coordinate.
	/// @param x1 - second point's x coordinate.
	/// @param y1 - second point's y coordinate.
	virtual void drawLine(int x1, int y1, int x2, int y2);

	/// Draw rect on the canvas without redrawing it.
	/// @param x - x coordinate.
	/// @param y - y coordinate.
	/// @param width - rectangle`s width.
	/// @param height - rectangle's height.
	/// @param filled - if true then rectangle will be filled with color, if false it will be empty.
	virtual void drawRect(int x, int y, int width, int height, bool filled = false);

	/// Draw ellipse on the canvas without redrawing it.
	/// @param centerX - x coordinate of the center.
	/// @param y - y coordinate of the center.
	/// @param width - the semi-axis length by the x-dimension of the ellipse.
	/// @param semiDiameterY - the semi-axis length by the y-dimension of the ellipse.
	/// @param filled - if true then ellipse will be filled with color, if false it will be empty.
	virtual void drawEllipse(int centerX, int centerY, int semiDiameterX, int semiDiameterY, bool filled = false);

	/// Draw arc on the canvas without redrawing it.
	/// @param x - x coordinate.
	/// @param y - y coordinate.
	/// @param width - width rect forming an arc.
	/// @param height - height rect forming an arc.
	/// @param startAngle - start angle.
	/// @param spanAngle - end angle.
	virtual void drawArc(int x, int y, int width, int height, int startAngle, int spanAngle);

	/// Sets painter`s pen \a color. The \a color will be used for painting of all next shapes till the next
	/// call of setPainterColor.
	/// @param color - color of painter.
	virtual void setPainterColor(const QColor &color);

	/// Set painter pen width. The \a penWidth will be used for painting of all next shapes till the next
	/// call of setPainterWidth.
	virtual void setPainterWidth(int penWidth);

	/// Serializes shapes into one big JSON array and returns it.
	QJsonArray toJson() const;

	void paint(QPainter *painter) override;

protected:
	/// Has ownership
	QList<PointObject *> mPixels;

	/// Has ownership
	QList<LineObject *> mLines;

	/// Has ownership
	QList<RectangleObject *> mRectangles;

	/// Has ownership
	QList<EllipseObject *> mEllipses;

	/// Has ownership
	QList<TextObject *> mLabels;

	/// Has ownership
	QList<ArcObject *> mArcs;

	/// We need duplicating objects map due to performance reasons:
	/// when someone requests all objects on the canvas sorted by their creation time
	/// it is much quicker to return ready list than to collect it each time from 7 lists and then sort.
	/// Has ownership
	QList<CanvasObject *> mObjects;

	int mCurrentPenWidth;
	QColor mCurrentPenColor;
};

}

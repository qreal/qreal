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

#include <QtGui/QColor>
#include <trikKit/robotModel/parts/trikDisplay.h>

#include <twoDModel/engine/twoDModelEngineInterface.h>
#include <twoDModel/engine/twoDModelDisplayInterface.h>

namespace trik {
namespace robotModel {
namespace twoD {
namespace parts {

class Display : public robotModel::parts::TrikDisplay, public graphicsUtils::PainterInterface
{
	Q_OBJECT

public:
	Display(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port
			, twoDModel::engine::TwoDModelEngineInterface &engine);

	void drawSmile(bool sad) override;
	void setBackground(const QColor &color) override;
	void printText(int x, int y, const QString &text) override;
	void clearScreen() override;
	void setPainterColor(const QColor &color) override;
	void setPainterWidth(int penWidth) override;
	void drawPixel(int x, int y) override;
	void drawLine(int x1, int y1, int x2, int y2) override;
	void drawRect(int x, int y, int width, int height)  override;
	void drawEllipse(int x, int y, int width, int height) override;
	void drawArc(int x, int y, int width, int height, int startAngle, int spanAngle) override;
	void paint(QPainter *painter) override;
	void clear() override;

private:
	twoDModel::engine::TwoDModelEngineInterface &mEngine;
	QColor mBackground;
	QImage mCurrentImage;
	/// @todo: QPoint can`t be used in map without operators declaration.
	QHash<QPair<int, int>, QPair<QString, QColor>> mLabels;

	/// Information about pixel.
	struct PixelCoordinates
	{
		PixelCoordinates(int x, int y, QColor color, int penWidth)
				: coord(QPoint(x, y)), color(color), penWidth(penWidth)
		{
		}

		QPoint coord;
		QColor color;
		int penWidth;
	};

	/// Information about line.
	struct LineCoordinates
	{
		LineCoordinates(int x1, int y1, int x2, int y2, QColor color, int penWidth)
			: line(x1, y1, x2, y2), color(color), penWidth(penWidth)
		{
		}

		QLine line;
		QColor color;
		int penWidth;
	};

	/// Information about rect.
	struct RectCoordinates
	{
		RectCoordinates(int x, int y, int width, int height, QColor color, int penWidth)
			: rect(QRect(x, y, width, height)), color(color), penWidth(penWidth)
		{
		}

		QRect rect;
		QColor color;
		int penWidth;
	};

	/// Information about ellipse.
	struct EllipseCoordinates
	{
		EllipseCoordinates(int x, int y, int width, int height, QColor color, int penWidth)
			: ellipse(QRect(x, y, width, height)), color(color), penWidth(penWidth)
		{
		}

		QRect ellipse;
		QColor color;
		int penWidth;
	};

	/// Information about arc.
	struct ArcCoordinates
	{
		ArcCoordinates(int x, int y, int width, int height, int startAngle, int spanAngle, QColor color, int penWidth)
			: arc(QRect(x, y, width, height))
			, startAngle(startAngle)
			, spanAngle(spanAngle)
			, color(color)
			, penWidth(penWidth)
		{
		}

		QRect arc;
		int startAngle;
		int spanAngle;
		QColor color;
		int penWidth;
	};

	/// List of all points.
	QList<PixelCoordinates> mPixels;

	/// List of all lines.
	QList<LineCoordinates> mLines;

	/// List of all rectangles.
	QList<RectCoordinates> mRects;

	/// List of all ellipses.
	QList<EllipseCoordinates> mEllipses;

	/// List of all arcs.
	QList<ArcCoordinates> mArcs;

	/// Current pen width.
	int mCurrentPenWidth;

	/// Current pen color.
	QColor mCurrentPenColor;
};

}
}
}
}

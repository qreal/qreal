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

#include "twoDDisplay.h"

using namespace trik::robotModel::twoD::parts;
using namespace kitBase::robotModel;

/// @todo: This constant adjusts screen coordinates shift. It must be 0.
const int yDisplayShift = 8;

Display::Display(const DeviceInfo &info
		, const PortInfo &port
		, twoDModel::engine::TwoDModelEngineInterface &engine)
	: robotModel::parts::TrikDisplay(info, port)
	, mEngine(engine)
	, mBackground(Qt::transparent)
	, mCurrentPenWidth(0)
	, mCurrentPenColor(Qt::black)
{
	mEngine.display()->setPainter(this);
}

void Display::setPainterColor(const QColor &color)
{
	mCurrentPenColor = color;
}

void Display::setPainterWidth(int penWidth)
{
	mCurrentPenWidth = penWidth;
}

void Display::drawPixel(int x, int y)
{
	mPixels << PixelCoordinates(x, y, mCurrentPenColor, mCurrentPenWidth);
	mEngine.display()->repaintDisplay();
}

void Display::drawLine(int x1, int y1, int x2, int y2)
{
	mLines << LineCoordinates(x1, y1, x2, y2, mCurrentPenColor, mCurrentPenWidth);
	mEngine.display()->repaintDisplay();
}

void Display::drawRect(int x, int y, int width, int height)
{
	mRects << RectCoordinates(x, y, width, height, mCurrentPenColor, mCurrentPenWidth);
	mEngine.display()->repaintDisplay();
}

void Display::drawEllipse(int x, int y, int width, int height)
{
	mEllipses << EllipseCoordinates(x, y, width, height, mCurrentPenColor, mCurrentPenWidth);
	mEngine.display()->repaintDisplay();
}

void Display::drawArc(int x, int y, int width, int height, int startAngle, int spanAngle)
{
	mArcs << ArcCoordinates(x, y, width, height, startAngle, spanAngle, mCurrentPenColor, mCurrentPenWidth);
	mEngine.display()->repaintDisplay();
}

void Display::drawSmile(bool sad)
{
	mCurrentImage = QImage(sad ? ":/icons/sadSmile.png" : ":/icons/smile.png");
	mEngine.display()->repaintDisplay();
}

void Display::setBackground(const QColor &color)
{
	mBackground = color;
	mEngine.display()->repaintDisplay();
}

void Display::printText(int x, int y, const QString &text)
{
	mLabels[qMakePair(x, y + yDisplayShift)] = {text, mCurrentPenColor};
	mEngine.display()->repaintDisplay();
}

void Display::clearScreen()
{
	// Background color is not cleared
	mCurrentImage = QImage();
	mLabels.clear();
	mPixels.clear();
	mLines.clear();
	mRects.clear();
	mEllipses.clear();
	mArcs.clear();
	mCurrentPenWidth = 0;
	mCurrentPenColor = Qt::black;
	if (mEngine.display()) {
		mEngine.display()->repaintDisplay();
	}
}

void Display::paint(QPainter *painter)
{
	const QRect displayRect(0, 0, mEngine.display()->displayWidth(), mEngine.display()->displayHeight());
	painter->save();
	painter->setPen(mBackground);
	painter->setBrush(mBackground);
	painter->drawRect(displayRect);
	painter->drawImage(displayRect, mCurrentImage);

	painter->restore();

	painter->save();
	painter->setPen(Qt::black);

	for (QPair<int, int> const &point : mLabels.keys()) {
		/// @todo: Honest labels must be here, without text overlapping.
		painter->setPen(QPen(mLabels[point].second, 0, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin));
		painter->drawText(QPoint(point.first, point.second), mLabels[point].first);
	}

	for (int i = 0; i < mPixels.length(); ++i) {
		painter->setPen(QPen(mPixels.at(i).color, mPixels.at(i).penWidth, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin));
		painter->drawPoint(mPixels.at(i).coord.x(), mPixels.at(i).coord.y());
	}

	for (int i = 0; i < mLines.length(); ++i) {
		painter->setPen(QPen(mLines.at(i).color, mLines.at(i).penWidth, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin));
		painter->drawLine(mLines.at(i).line.x1(), mLines.at(i).line.y1()
				, mLines.at(i).line.x2(), mLines.at(i).line.y2());
	}

	for (int i = 0; i < mRects.length(); ++i) {
		painter->setPen(QPen(mRects.at(i).color, mRects.at(i).penWidth, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin));
		painter->drawRect(mRects.at(i).rect.x(), mRects.at(i).rect.y()
				, mRects.at(i).rect.width(), mRects.at(i).rect.height());
	}

	for (int i = 0; i < mEllipses.length(); ++i) {
		painter->setPen(QPen(mEllipses.at(i).color, mEllipses.at(i).penWidth, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin));
		painter->drawEllipse(mEllipses.at(i).ellipse.x(), mEllipses.at(i).ellipse.y()
				, mEllipses.at(i).ellipse.width(), mEllipses.at(i).ellipse.height());
	}

	for (int i = 0; i < mArcs.length(); ++i) {
		painter->setPen(QPen(mArcs.at(i).color, mArcs.at(i).penWidth, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin));
		painter->drawArc(mArcs.at(i).arc.x(), mArcs.at(i).arc.y()
				, mArcs.at(i).arc.width(), mArcs.at(i).arc.height()
				, mArcs.at(i).startAngle, mArcs.at(i).spanAngle);
	}

	painter->restore();
}

void Display::clear()
{
	clearScreen();
}

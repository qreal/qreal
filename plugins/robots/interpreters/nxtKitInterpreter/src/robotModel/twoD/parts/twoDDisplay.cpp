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

using namespace nxt::robotModel::twoD::parts;
using namespace kitBase::robotModel;

Display::Display(const DeviceInfo &info
		, const PortInfo &port
		, twoDModel::engine::TwoDModelEngineInterface &engine)
	: robotModel::parts::NxtDisplay(info, port)
	, mEngine(engine)
{
	mEngine.display()->setPainter(this);
}

void Display::drawPixel(int x, int y)
{
	mPoints << QPoint(x, y);
	mEngine.display()->repaintDisplay();
}

void Display::drawLine(int x1, int y1, int x2, int y2)
{
	mLines << QLine(x1, y1, x2, y2);
	mEngine.display()->repaintDisplay();
}

void Display::drawRect(int x, int y, int width, int height)
{
	mRects << QRect(x, y, width, height);
	mEngine.display()->repaintDisplay();
}

void Display::drawCircle(int x, int y, int radius)
{
	mCircles << QRect(x - radius, y - radius, 2 * radius, 2 * radius);
	mEngine.display()->repaintDisplay();
}

void Display::printText(int x, int y, const QString &text)
{
	mStringPlaces << QPoint(x, y);
	mStrings << text;
	mEngine.display()->repaintDisplay();
}

void Display::clearScreen()
{
	mPoints.clear();
	mLines.clear();
	mRects.clear();
	mCircles.clear();
	mStrings.clear();
	mStringPlaces.clear();
	mEngine.display()->repaintDisplay();
}

void Display::paint(QPainter *painter)
{
	/// @todo ZOMG.
	const qreal pixWidth = static_cast<qreal>(mEngine.display()->displayWidth()) / nxtDisplayWidth;
	const qreal pixHeight = static_cast<qreal>(mEngine.display()->displayHeight()) / nxtDisplayHeight;

	QPen pen;
	QFont font;
	font.setPixelSize(pixHeight * textPixelHeight);

	painter->setBrush(QBrush(Qt::black, Qt::SolidPattern));
	foreach (const QPoint &point, mPoints) {
		painter->drawRect(point.x() * pixWidth, point.y() * pixHeight, pixWidth, pixHeight);
	}

	pen.setWidth((pixWidth + pixHeight) / 2);
	painter->setPen(pen);
	painter->setBrush(QBrush(Qt::black, Qt::NoBrush));
	painter->setFont(font);

	foreach (const QLine &line, mLines) {
		painter->drawLine(line.x1() * pixWidth, line.y1() * pixHeight, line.x2() * pixWidth, line.y2() * pixHeight);
	}

	foreach (const QRect &circle, mCircles) {
		painter->drawEllipse(circle.x() * pixWidth, circle.y() * pixHeight, circle.width() * pixWidth, circle.height() * pixHeight);
	}

	foreach (const QRect &rect, mRects) {
		painter->drawRect(rect.x() * pixWidth, rect.y() * pixHeight, rect.width() * pixWidth, rect.height() * pixHeight);
	}

	QListIterator<QString> strings(mStrings);
	QListIterator<QPoint> strPlaces(mStringPlaces);
	while (strings.hasNext() && strPlaces.hasNext()) {
		const QString str = strings.next();
		const QPoint place = strPlaces.next();
		painter->drawText(place.x() * pixWidth * nxtDisplayWidth / textPixelWidth
				, place.y() * pixHeight * nxtDisplayHeight / textPixelHeight, str);
	}
}

void Display::clear()
{
	clearScreen();
}

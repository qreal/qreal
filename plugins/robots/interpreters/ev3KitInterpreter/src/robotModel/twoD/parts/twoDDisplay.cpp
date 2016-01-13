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

const int textSize = 20;
const int ev3DisplayHeight = 128;
const int ev3DisplayWidth = 178;

using namespace ev3::robotModel::twoD::parts;
using namespace kitBase::robotModel;

Display::Display(const DeviceInfo &info
		, const PortInfo &port
		, twoDModel::engine::TwoDModelEngineInterface &engine)
	: robotModel::parts::Ev3Display(info, port)
	, mEngine(engine)
{
	mEngine.display()->setPainter(this);
}

void Display::redraw()
{
	mEngine.display()->repaintDisplay();
}

void Display::drawPixel(int x, int y)
{
	Canvas::drawPixel(x, y);
}

void Display::drawLine(int x1, int y1, int x2, int y2)
{
	Canvas::drawLine(x1, y1, x2, y2);
}

void Display::drawRect(int x, int y, int width, int height)
{
	drawRect(x, y, width, height, false);
}

void Display::drawRect(int x, int y, int width, int height, bool filled)
{
	Canvas::drawRect(x, y, width, height, filled);
}

void Display::drawCircle(int x, int y, int radius, bool filled)
{
	Canvas::drawEllipse(x, y, radius, radius, filled);
}

void Display::printText(int x, int y, const QString &text)
{
	Canvas::printText(x, y, text);
}

void Display::clearScreen()
{
	Canvas::reset();
}

void Display::paint(QPainter *painter)
{
	painter->save();
	painter->scale(static_cast<qreal>(mEngine.display()->displayWidth()) / ev3DisplayWidth
			, static_cast<qreal>(mEngine.display()->displayHeight()) / ev3DisplayHeight);

	QPen pen;
	QFont font;
	font.setPixelSize(textSize);

	painter->setPen(pen);
	painter->setBrush(QBrush(Qt::black, Qt::NoBrush));
	painter->setFont(font);
	painter->setRenderHint(QPainter::HighQualityAntialiasing);

	Canvas::paint(painter);

	painter->restore();
}

void Display::reset()
{
	clearScreen();
	redraw();
}

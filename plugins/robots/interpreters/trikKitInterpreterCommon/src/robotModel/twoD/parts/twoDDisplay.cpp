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

#include "trikKitInterpreterCommon/robotModel/twoD/parts/twoDDisplay.h"

#include <utils/canvas/textObject.h>

using namespace trik::robotModel::twoD::parts;
using namespace kitBase::robotModel;

const qreal realWidth = 218;
const qreal realHeight = 274;
const int textSize = 20;

Display::Display(const DeviceInfo &info
		, const PortInfo &port
		, twoDModel::engine::TwoDModelEngineInterface &engine)
	: robotModel::parts::TrikDisplay(info, port)
	, mEngine(engine)
	, mBackground(Qt::transparent)
{
	mEngine.display()->setPainter(this);
}

QString Display::background() const
{
	return mBackground.name();
}

bool Display::smiles() const
{
	return mSmiles;
}

bool Display::sadSmiles() const
{
	return mSadSmiles;
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
	Canvas::drawRect(x, y, width, height);
}

void Display::drawEllipse(int x, int y, int width, int height)
{
	Canvas::drawEllipse(x, y, width, height);
}

void Display::drawArc(int x, int y, int width, int height, int startAngle, int spanAngle)
{
	Canvas::drawArc(x, y, width, height, startAngle, spanAngle);
}

void Display::drawSmile(bool sad)
{
	mCurrentImage = QImage(sad ? ":/icons/sadSmile.png" : ":/icons/smile.png");
	mSmiles = !sad;
	mSadSmiles = sad;
	mEngine.display()->repaintDisplay();
}

void Display::setBackground(const QColor &color)
{
	mBackground = color;
}

void Display::printText(int x, int y, const QString &text)
{
	const QPair<int, int> coords(x, y);
	if (mLabelsMap.contains(coords)) {
		mLabelsMap[coords]->setText(text);
	} else {
		utils::TextObject * const textObject = new utils::TextObject(x, y + textSize, text
			, mCurrentPenColor, mCurrentPenWidth);
		mObjects << textObject;
		mLabelsMap[coords] = textObject;
		mLabels << textObject;
	}
}

void Display::clearScreen()
{
	// Background color is not cleared
	mCurrentImage = QImage();
	mSmiles = false;
	mSadSmiles = false;
	mLabelsMap.clear();
	Canvas::reset();
}

void Display::setPainterColor(const QColor &color)
{
	Canvas::setPainterColor(color);
}

void Display::setPainterWidth(int penWidth)
{
	Canvas::setPainterWidth(penWidth);
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
	painter->setRenderHint(QPainter::HighQualityAntialiasing);
	QFont font;
	font.setPixelSize(textSize);
	painter->setFont(font);
	painter->setPen(Qt::black);
	painter->scale(displayRect.width() / realWidth, displayRect.height() / realHeight);
	Canvas::paint(painter);
	painter->restore();
}

void Display::reset()
{
	clearScreen();
	setBackground(Qt::transparent);
	redraw();
}

void Display::redraw()
{
	if (mEngine.display()) {
		mEngine.display()->repaintDisplay();
	}
}

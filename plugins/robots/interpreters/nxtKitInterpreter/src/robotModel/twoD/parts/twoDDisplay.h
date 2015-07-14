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

#include <nxtKit/robotModel/parts/nxtDisplay.h>

#include <twoDModel/engine/twoDModelEngineInterface.h>
#include <twoDModel/engine/twoDModelDisplayInterface.h>

namespace nxt {
namespace robotModel {
namespace twoD {
namespace parts {

/// @todo Move this somewhere.
const int textPixelHeight = 8;
const int textPixelWidth = 16;
const int nxtDisplayHeight = 64;
const int nxtDisplayWidth = 100;

class Display : public robotModel::parts::NxtDisplay, public qReal::ui::PainterInterface
{
	Q_OBJECT

public:
	Display(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port
			, twoDModel::engine::TwoDModelEngineInterface &engine);

	void drawPixel(int x, int y, bool redraw) override;
	void drawLine(int x1, int y1, int x2, int y2, bool redraw) override;
	void drawRect(int x, int y, int width, int height, bool redraw) override;
	void drawCircle(int x, int y, int radius, bool redraw) override;
	void printText(int x, int y, const QString &text, bool redraw) override;
	void clearScreen(bool redraw) override;

	void paint(QPainter *painter) override;
	void reset() override;

private:
	twoDModel::engine::TwoDModelEngineInterface &mEngine;
	QList<QLine> mLines;
	QList<QPoint> mPoints;
	QList<QRect> mCircles;
	QList<QRect> mRects;
	QList<QString> mStrings;
	QList<QPoint> mStringPlaces;
};

}
}
}
}

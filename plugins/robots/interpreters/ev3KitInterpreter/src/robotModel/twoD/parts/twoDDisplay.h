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

#include "robotModel/parts/nxtDisplay.h"

#include <commonTwoDModel/engine/twoDModelEngineInterface.h>
#include <commonTwoDModel/engine/twoDModelDisplayInterface.h>

namespace nxtKitInterpreter {
namespace robotModel {
namespace twoD {
namespace parts {

/// @todo Move this somewhere.
int const textPixelHeight = 8;
int const textPixelWidth = 16;
int const nxtDisplayHeight = 64;
int const nxtDisplayWidth = 100;

class Display : public robotModel::parts::NxtDisplay, public graphicsUtils::PainterInterface
{
	Q_OBJECT

public:
	Display(kitBase::robotModel::DeviceInfo const &info
			, kitBase::robotModel::PortInfo const &port
			, twoDModel::engine::TwoDModelEngineInterface &engine);

	void drawPixel(int x, int y) override;
	void drawLine(int x1, int y1, int x2, int y2) override;
	void drawRect(int x, int y, int width, int height) override;
	void drawCircle(int x, int y, int radius) override;
	void printText(int x, int y, QString const &text) override;
	void clearScreen() override;

	void paint(QPainter *painter) override;
	void clear() override;

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

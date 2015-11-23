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

#include <utils/canvas/canvas.h>
#include <twoDModel/engine/twoDModelEngineInterface.h>
#include <twoDModel/engine/twoDModelDisplayInterface.h>

namespace nxt {
namespace robotModel {
namespace twoD {
namespace parts {

class Display : public robotModel::parts::NxtDisplay, public utils::Canvas
{
	Q_OBJECT
	// Canvas cannot be QObject because of ambiguous base so we are forced to copy properties here.
	Q_PROPERTY(QList<utils::CanvasObject *> objects READ objects)
	Q_PROPERTY(QList<utils::PointObject *> pixels READ pixels)
	Q_PROPERTY(QList<utils::LineObject *> segments READ segments)
	Q_PROPERTY(QList<utils::RectangleObject *> rectangles READ rectangles)
	Q_PROPERTY(QList<utils::EllipseObject *> ellipses READ ellipses)
	Q_PROPERTY(QList<utils::ArcObject *> arcs READ arcs)
	Q_PROPERTY(QList<utils::TextObject *> labels READ labels)

public:
	Display(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port
			, twoDModel::engine::TwoDModelEngineInterface &engine);

	void clearScreen() override;
	void redraw() override;
	void reset() override;

	void drawPixel(int x, int y) override;
	void drawLine(int x1, int y1, int x2, int y2) override;
	void drawRect(int x, int y, int width, int height) override;
	void drawCircle(int x, int y, int radius) override;
	void printText(int x, int y, const QString &text) override;

	void paint(QPainter *painter) override;

private:
	twoDModel::engine::TwoDModelEngineInterface &mEngine;
};

}
}
}
}

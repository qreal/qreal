#pragma once

#include "robotModel/parts/trikDisplay.h"

#include <commonTwoDModel/engine/twoDModelEngineInterface.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace twoD {
namespace parts {

/// @todo Move this somewhere.
int const textPixelHeight = 8;
int const textPixelWidth = 16;
int const nxtDisplayHeight = 64;
int const nxtDisplayWidth = 100;

class Display : public robotModel::parts::TrikDisplay, public graphicsUtils::PainterInterface
{
	Q_OBJECT

public:
	Display(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port
			, twoDModel::engine::TwoDModelEngineInterface &engine);

	void attachToPaintWidget();

	void drawPixel(int x, int y) override;
	void drawLine(int x1, int y1, int x2, int y2) override;
	void drawRect(int x, int y, int width, int height) override;
	void drawCircle(int x, int y, int radius) override;
	void printText(int x, int y, QString const &text) override;
	void clearScreen() override;

	void paint(QPainter *painter) override;

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

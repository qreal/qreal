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

	void drawSmile(bool sad) override;
	void setBackground(QColor const &color) override;
	void clearScreen() override;

	void paint(QPainter *painter) override;
	void clear() override;

private:
	twoDModel::engine::TwoDModelEngineInterface &mEngine;
	QColor mBackground;
	QImage mCurrentImage;
};

}
}
}
}

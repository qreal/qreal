#pragma once

#include "interpreterBase/robotModel/robotParts/display.h"

#include "commonTwoDModel/commonTwoDModelDeclSpec.h"

namespace twoDModel {

namespace engine {
class TwoDModelEngineInterface;
}

namespace robotModel {
namespace parts {

class Display : public interpreterBase::robotModel::robotParts::Display
{
	Q_OBJECT

public:
	Display(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port
			, engine::TwoDModelEngineInterface &engine);

	/// @todo Implement.
//	void drawPixel(int x, int y) override;
//	void drawLine(int x1, int y1, int x2, int y2) override;
//	void drawRect(int x, int y, int width, int height) override;
//	void drawCircle(int x, int y, int radius) override;
//	void printText(int x, int y, QString const &text) override;
//	void clearScreen() override;

protected:
	void doConfiguration() override;

private:
	engine::TwoDModelEngineInterface &mEngine;
};

}
}
}

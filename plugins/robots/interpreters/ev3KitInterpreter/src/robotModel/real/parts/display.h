#pragma once

#include "robotModel/parts/ev3Display.h"
#include <utils/robotCommunication/robotCommunicator.h>

#include "src/robotModel/real/ev3DirectCommand.h"
#include "commandConstants.h"

namespace ev3KitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class Display : public robotModel::parts::Ev3Display
{
	Q_OBJECT

public:
	Display(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port
			, utils::robotCommunication::RobotCommunicator &robotCommunicator);

	void drawPixel(int x, int y) override;
	void drawLine(int x1, int y1, int x2, int y2) override;
	void drawRect(int x, int y, int width, int height, bool filled) override;
	void drawRect(int x, int y, int width, int height) override;

	void drawCircle(int x, int y, int radius, bool filled) override;

	void printText(int x, int y, QString const &text) override;
	void clearScreen() override;

private:
	utils::robotCommunication::RobotCommunicator &mRobotCommunicator;
};

}
}
}
}

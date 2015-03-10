#pragma once

#include <ev3Kit/robotModel/parts/ev3Display.h>
#include <utils/robotCommunication/robotCommunicator.h>

namespace ev3 {
namespace robotModel {
namespace real {
namespace parts {

class Display : public robotModel::parts::Ev3Display
{
	Q_OBJECT

public:
	Display(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port
			, utils::robotCommunication::RobotCommunicator &robotCommunicator);

	void drawPixel(int x, int y) override;
	void drawLine(int x1, int y1, int x2, int y2) override;
	void drawRect(int x, int y, int width, int height, bool filled) override;
	void drawRect(int x, int y, int width, int height) override;

	void drawCircle(int x, int y, int radius, bool filled) override;

	void printText(int x, int y, const QString &text) override;
	void clearScreen() override;

private:
	utils::robotCommunication::RobotCommunicator &mRobotCommunicator;
};

}
}
}
}

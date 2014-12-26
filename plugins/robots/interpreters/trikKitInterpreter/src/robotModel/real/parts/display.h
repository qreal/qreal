#pragma once

#include "robotModel/parts/trikDisplay.h"
#include <utils/tcpRobotCommunicator.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace real {
namespace parts {

class Display : public robotModel::parts::TrikDisplay
{
	Q_OBJECT

public:
	Display(interpreterBase::robotModel::DeviceInfo const &info
		, interpreterBase::robotModel::PortInfo const &port
		, utils::TcpRobotCommunicator &tcpRobotCommunicator);

	void drawSmile(bool sad) override;
	/// @todo: color enum here?
	void setBackground(QColor const &color) override;
	void clearScreen() override;
	void printText(int x, int y, QString const &text) override;

	void setPainterColor(QColor const &color) override;
	void setPainterWidth(int penWidth) override;
	void drawPixel(int x, int y) override;
	void drawLine(int x1, int y1, int x2, int y2) override;
	void drawRect(int x, int y, int width, int height)  override;
	void drawEllipse(int x, int y, int width, int height) override;
	void drawArc(int x, int y, int width, int height, int startAngle, int spanAngle)  override;

private:
	utils::TcpRobotCommunicator &mRobotCommunicator;
};

}
}
}
}

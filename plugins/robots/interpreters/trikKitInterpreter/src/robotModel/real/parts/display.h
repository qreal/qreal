#pragma once

#include <trikKit/robotModel/parts/trikDisplay.h>
#include <utils/tcpRobotCommunicator.h>

namespace trik {
namespace robotModel {
namespace real {
namespace parts {

class Display : public robotModel::parts::TrikDisplay
{
	Q_OBJECT

public:
	Display(const kitBase::robotModel::DeviceInfo &info
		, const kitBase::robotModel::PortInfo &port
		, utils::TcpRobotCommunicator &tcpRobotCommunicator);

	void drawSmile(bool sad) override;
	/// @todo: color enum here?
	void setBackground(const QColor &color) override;
	void clearScreen() override;
	void printText(int x, int y, const QString &text) override;

	void setPainterColor(const QColor &color) override;
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

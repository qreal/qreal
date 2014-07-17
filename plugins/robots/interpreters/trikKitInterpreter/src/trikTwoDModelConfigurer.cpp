#include "trikTwoDModelConfigurer.h"

#include <interpreterBase/robotModel/robotParts/lightSensor.h>
#include <interpreterBase/robotModel/robotParts/colorSensor.h>
#include <interpreterBase/robotModel/robotParts/rangeSensor.h>
#include <interpreterBase/robotModel/robotParts/touchSensor.h>

#include "trikDisplayWidget.h"

using namespace trikKitInterpreter;
using namespace interpreterBase::robotModel;

TrikTwoDModelConfigurer::TrikTwoDModelConfigurer(QString const &leftWheelPort, QString const &rightWheelPort)
	: mLeftWheelPort(leftWheelPort)
	, mRightWheelPort(rightWheelPort)
{
}

QString TrikTwoDModelConfigurer::robotImage() const
{
	return ":icons/trikTwoDRobot.svg";
}

PortInfo TrikTwoDModelConfigurer::defaultLeftWheelPort() const
{
	return PortInfo(mLeftWheelPort, output);
}

PortInfo TrikTwoDModelConfigurer::defaultRightWheelPort() const
{
	return PortInfo(mRightWheelPort, output);
}

twoDModel::engine::TwoDModelDisplayWidget *TrikTwoDModelConfigurer::displayWidget(QWidget * parent) const
{
	return new TrikDisplayWidget(parent);
}

QString trikKitInterpreter::TrikTwoDModelConfigurer::sensorImagePath(const DeviceInfo &deviceType) const
{
	if (deviceType.isA<interpreterBase::robotModel::robotParts::LightSensor>()) {
		return ":icons/twoDColorEmpty.svg";
	}

	return "";
}

QRect TrikTwoDModelConfigurer::sensorImageRect(interpreterBase::robotModel::DeviceInfo const &deviceType) const
{
	if (deviceType.isA<robotParts::TouchSensor>()) {
		return QRect(-12, -5, 25, 10);
	} else if (deviceType.isA<robotParts::ColorSensor>()
			|| deviceType.isA<robotParts::LightSensor>())
	{
		return QRect(-6, -6, 12, 12);
	}
	if (deviceType.isA<robotParts::RangeSensor>()) {
		return QRect(-20, -10, 40, 20);;
	} else {
		Q_ASSERT(!"Unknown sensor type");
		return QRect();
	}
}


#include "twoDRobotModel.h"

#include <interpreterBase/robotModel/robotParts/speaker.h>
#include <interpreterBase/robotModel/robotParts/motor.h>
#include <interpreterBase/robotModel/robotParts/encoderSensor.h>
#include <interpreterBase/robotModel/robotParts/touchSensor.h>
#include <interpreterBase/robotModel/robotParts/rangeSensor.h>
#include <interpreterBase/robotModel/robotParts/lightSensor.h>
#include <interpreterBase/robotModel/robotParts/colorSensorBlue.h>
#include <interpreterBase/robotModel/robotParts/colorSensorFull.h>
#include <interpreterBase/robotModel/robotParts/colorSensorGreen.h>
#include <interpreterBase/robotModel/robotParts/colorSensorPassive.h>
#include <interpreterBase/robotModel/robotParts/colorSensorRed.h>

#include "nxtDisplayWidget.h"
#include "robotModel/twoD/parts/twoDDisplay.h"
#include "robotModel/twoD/parts/twoDSpeaker.h"
#include "robotModel/twoD/parts/twoDMotor.h"
#include "nxtDisplayWidget.h"

using namespace nxtKitInterpreter::robotModel;
using namespace nxtKitInterpreter::robotModel::twoD;
using namespace interpreterBase::robotModel;

TwoDRobotModel::TwoDRobotModel(RobotModelInterface &realModel)
	: twoDModel::robotModel::TwoDRobotModel(realModel)
{
}

robotParts::Device *TwoDRobotModel::createDevice(PortInfo const &port, DeviceInfo const &deviceInfo)
{
	if (deviceInfo.isA<robotParts::Display>()) {
		return new parts::Display(deviceInfo, port, *engine());
	}

	if (deviceInfo.isA<robotParts::Speaker>()) {
		return new parts::TwoDSpeaker(deviceInfo, port, *engine());
	}

	if (deviceInfo.isA<robotParts::Motor>()) {
		return new parts::TwoDMotor(deviceInfo, port, *engine());
	}

	return twoDModel::robotModel::TwoDRobotModel::createDevice(port, deviceInfo);
}

QString TwoDRobotModel::robotImage() const
{
	return ":/icons/robot.png";
}

PortInfo TwoDRobotModel::defaultLeftWheelPort() const
{
	return PortInfo("B", output);
}

PortInfo TwoDRobotModel::defaultRightWheelPort() const
{
	return PortInfo("C", output);
}

twoDModel::engine::TwoDModelDisplayWidget *TwoDRobotModel::displayWidget(QWidget * parent) const
{
	return new NxtDisplayWidget(parent);
}

QRect TwoDRobotModel::sensorImageRect(interpreterBase::robotModel::DeviceInfo const &deviceType) const
{
	if (deviceType.isA<robotParts::TouchSensor>()) {
		return QRect(-12, -5, 25, 10);
	}

	if (deviceType.isA<robotParts::ColorSensor>()
			|| deviceType.isA<robotParts::LightSensor>())
	{
		return QRect(-6, -6, 12, 12);
	}

	if (deviceType.isA<robotParts::RangeSensor>()) {
		return QRect(-20, -10, 40, 20);;
	}

	return QRect();
}

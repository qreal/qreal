#include "twoDRobotModel.h"

#include <QtGui/QColor>

#include <interpreterBase/robotModel/robotParts/button.h>
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

#include <interpreterBase/robotModel/robotModelUtils.h>

#include "trikDisplayWidget.h"
#include "robotModel/twoD/parts/twoDDisplay.h"
#include "robotModel/twoD/parts/twoDSpeaker.h"
#include "robotModel/twoD/parts/twoDInfraredSensor.h"
#include "robotModel/twoD/parts/twoDLed.h"
#include "robotModel/parts/trikInfraredSensor.h"
#include "robotModel/parts/trikSonarSensor.h"

using namespace trikKitInterpreter::robotModel;
using namespace trikKitInterpreter::robotModel::twoD;
using namespace interpreterBase::robotModel;

TwoDRobotModel::TwoDRobotModel(RobotModelInterface const &realModel)
	: twoDModel::robotModel::TwoDRobotModel(realModel)
	, mLeftWheelPort("M3")
	, mRightWheelPort("M4")
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

	if (deviceInfo.isA<robotModel::parts::TrikInfraredSensor>()) {
		return new parts::TwoDInfraredSensor(deviceInfo, port, *engine());
	}

	if (deviceInfo.isA<robotModel::parts::TrikLed>()) {
		return new parts::TwoDLed(deviceInfo, port, *engine());
	}

	return twoDModel::robotModel::TwoDRobotModel::createDevice(port, deviceInfo);
}

void TwoDRobotModel::onInterpretationStarted()
{
	robotModel::parts::TrikDisplay * const display =
			RobotModelUtils::findDevice<robotModel::parts::TrikDisplay>(*this, "DisplayPort");
	if (display) {
		display->clearScreen();
		display->setBackground(QColor(Qt::gray));
	} else {
		/// @todo: if we get here it is wrong because display is not configured before the interpretation!
	}
}

QString TwoDRobotModel::robotImage() const
{
	return ":icons/trikTwoDRobot.svg";
}

PortInfo TwoDRobotModel::defaultLeftWheelPort() const
{
	return PortInfo(mLeftWheelPort, output);
}

PortInfo TwoDRobotModel::defaultRightWheelPort() const
{
	return PortInfo(mRightWheelPort, output);
}

twoDModel::engine::TwoDModelDisplayWidget *TwoDRobotModel::displayWidget(QWidget *parent) const
{
	return new TrikDisplayWidget(parent);
}

QString TwoDRobotModel::sensorImagePath(DeviceInfo const &deviceType) const
{
	if (deviceType.isA<interpreterBase::robotModel::robotParts::LightSensor>()) {
		return ":icons/twoDColorEmpty.svg";
	}

	return QString();
}

QRect TwoDRobotModel::sensorImageRect(interpreterBase::robotModel::DeviceInfo const &deviceType) const
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

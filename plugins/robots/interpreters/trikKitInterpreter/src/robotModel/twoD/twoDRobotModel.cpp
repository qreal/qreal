#include "twoDRobotModel.h"

#include <QtGui/QColor>

#include <interpreterBase/robotModel/robotModelUtils.h>
#include <interpreterBase/robotModel/robotParts/lightSensor.h>

#include "trikDisplayWidget.h"
#include "robotModel/twoD/parts/twoDDisplay.h"
#include "robotModel/twoD/parts/twoDSpeaker.h"
#include "robotModel/twoD/parts/twoDInfraredSensor.h"
#include "robotModel/twoD/parts/twoDLed.h"
#include "robotModel/twoD/parts/twoDLineSensor.h"
#include "robotModel/twoD/parts/twoDObjectSensor.h"
#include "robotModel/twoD/parts/twoDColorSensor.h"
#include "robotModel/parts/trikLineSensor.h"
#include "robotModel/parts/trikObjectSensor.h"
#include "robotModel/parts/trikColorSensor.h"
#include "robotModel/parts/trikInfraredSensor.h"
#include "robotModel/parts/trikSonarSensor.h"

#include "trikDisplayWidget.h"

using namespace trikKitInterpreter::robotModel;
using namespace trikKitInterpreter::robotModel::twoD;
using namespace interpreterBase::robotModel;

TwoDRobotModel::TwoDRobotModel(RobotModelInterface &realModel)
	: twoDModel::robotModel::TwoDRobotModel(realModel)
	, mLeftWheelPort("M3")
	, mRightWheelPort("M4")
	, mDisplayWidget(new TrikDisplayWidget())
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

	if (deviceInfo.isA<robotModel::parts::TrikLineSensor>()) {
		return new parts::LineSensor(deviceInfo, port);
	}

	if (deviceInfo.isA<robotModel::parts::TrikObjectSensor>()) {
		return new parts::ObjectSensor(deviceInfo, port);
	}

	if (deviceInfo.isA<robotModel::parts::TrikColorSensor>()) {
		return new parts::ColorSensor(deviceInfo, port);
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

twoDModel::engine::TwoDModelDisplayWidget *TwoDRobotModel::displayWidget() const
{
	return mDisplayWidget;
}

QString TwoDRobotModel::sensorImagePath(DeviceInfo const &deviceType) const
{
	if (deviceType.isA<interpreterBase::robotModel::robotParts::LightSensor>()) {
		return ":icons/twoDColorEmpty.svg";
	}

	return QString();
}

void TwoDRobotModel::setWheelPorts(QString const &leftWheelPort, QString const &rightWheelPort)
{
	mLeftWheelPort = leftWheelPort;
	mRightWheelPort = rightWheelPort;
}

QRect TwoDRobotModel::sensorImageRect(interpreterBase::robotModel::DeviceInfo const &deviceType) const
{
	if (deviceType.isA<robotParts::LightSensor>()) {
		return QRect(-6, -6, 12, 12);
	}

	if (deviceType.isA<robotParts::RangeSensor>()) {
		return QRect(-20, -10, 40, 20);;
	}

	return QRect();
}

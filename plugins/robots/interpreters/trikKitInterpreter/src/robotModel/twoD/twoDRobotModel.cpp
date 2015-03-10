#include "twoDRobotModel.h"

#include <QtGui/QColor>

#include <kitBase/robotModel/robotModelUtils.h>
#include <kitBase/robotModel/robotParts/lightSensor.h>

#include "trikDisplayWidget.h"
#include "robotModel/twoD/parts/twoDDisplay.h"
#include "robotModel/twoD/parts/twoDSpeaker.h"
#include "robotModel/twoD/parts/twoDInfraredSensor.h"
#include "robotModel/twoD/parts/twoDLed.h"
#include "robotModel/twoD/parts/twoDLineSensor.h"
#include "robotModel/twoD/parts/twoDObjectSensor.h"
#include "robotModel/twoD/parts/twoDColorSensor.h"
#include <trikKit/robotModel/parts/trikLineSensor.h>
#include <trikKit/robotModel/parts/trikObjectSensor.h>
#include <trikKit/robotModel/parts/trikColorSensor.h>
#include <trikKit/robotModel/parts/trikInfraredSensor.h>
#include <trikKit/robotModel/parts/trikSonarSensor.h>

#include "trikDisplayWidget.h"

using namespace trik::robotModel;
using namespace trik::robotModel::twoD;
using namespace kitBase::robotModel;

TwoDRobotModel::TwoDRobotModel(RobotModelInterface &realModel)
	: twoDModel::robotModel::TwoDRobotModel(realModel)
	, mLeftWheelPort("M3")
	, mRightWheelPort("M4")
{
}

robotParts::Device *TwoDRobotModel::createDevice(const PortInfo &port, const DeviceInfo &deviceInfo)
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

twoDModel::engine::TwoDModelDisplayWidget *TwoDRobotModel::displayWidget(QWidget *parent) const
{
	return new TrikDisplayWidget(parent);
}

QString TwoDRobotModel::sensorImagePath(const DeviceInfo &deviceType) const
{
	if (deviceType.isA<kitBase::robotModel::robotParts::LightSensor>()) {
		return ":icons/twoDColorEmpty.svg";
	} else if (deviceType.isA<robotModel::parts::TrikInfraredSensor>()) {
		return ":icons/twoDIrRangeSensor.svg";
	} else if (deviceType.isA<robotModel::parts::TrikSonarSensor>()) {
		return ":icons/twoDUsRangeSensor.svg";
	}

	return QString();
}

void TwoDRobotModel::setWheelPorts(const QString &leftWheelPort, const QString &rightWheelPort)
{
	mLeftWheelPort = leftWheelPort;
	mRightWheelPort = rightWheelPort;
}

QRect TwoDRobotModel::sensorImageRect(const kitBase::robotModel::DeviceInfo &deviceType) const
{
	if (deviceType.isA<robotParts::LightSensor>()) {
		return QRect(-6, -6, 12, 12);
	} else if (deviceType.isA<robotModel::parts::TrikInfraredSensor>()) {
		return QRect(-18, -18, 36, 36);
	} else if (deviceType.isA<robotModel::parts::TrikSonarSensor>()) {
		return QRect(-18, -18, 36, 36);
	}

	return QRect();
}

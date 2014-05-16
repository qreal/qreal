#include "twoDRobotModel.h"

#include "robotModel/twoD/parts/twoDDisplay.h"
#include "robotModel/twoD/parts/twoDButtons.h"
#include "robotModel/twoD/parts/twoDSpeaker.h"
#include "robotModel/twoD/parts/twoDInfraredSensor.h"
#include "robotModel/parts/trikInfraredSensor.h"
#include "robotModel/parts/trikSonarSensor.h"
#include "robotModel/parts/trikLed.h"

#include <interpreterBase/robotModel/robotParts/buttons.h>
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

using namespace trikKitInterpreter::robotModel;
using namespace trikKitInterpreter::robotModel::twoD;
using namespace interpreterBase::robotModel;

TwoDRobotModel::TwoDRobotModel(interpreterBase::robotModel::RobotModelInterface const &realModel)
	: mRealModel(realModel)
{
	for (auto port : realModel.availablePorts()) {
		addAllowedConnection(port, realModel.allowedDevices(port));
	}
}

QString TwoDRobotModel::name() const
{
	return "TrikTwoDRobotModel" + mRealModel.name();
}

QString TwoDRobotModel::friendlyName() const
{
	QRegExp versionRegExp("\\(.*\\)");
	int const pos = versionRegExp.indexIn(mRealModel.friendlyName());
	if (pos == -1) {
		return tr("2D Model");
	}

	return tr("2D Model") + " " + versionRegExp.capturedTexts().at(0);
}

robotParts::Device *TwoDRobotModel::createDevice(PortInfo const &port, DeviceInfo const &deviceInfo)
{
	if (deviceInfo.isA<robotParts::Display>()) {
		return new parts::Display(deviceInfo, port, *engine());
	}

	if (deviceInfo.isA<robotParts::Buttons>()) {
		return new parts::TwoDButtons(deviceInfo, port, *engine());
	}

	if (deviceInfo.isA<robotParts::Speaker>()) {
		return new parts::TwoDSpeaker(deviceInfo, port, *engine());
	}

	if (deviceInfo.isA<robotModel::parts::TrikInfraredSensor>()) {
		return new parts::TwoDInfraredSensor(deviceInfo, port, *engine());
	}

	return twoDModel::robotModel::TwoDRobotModel::createDevice(port, deviceInfo);
}

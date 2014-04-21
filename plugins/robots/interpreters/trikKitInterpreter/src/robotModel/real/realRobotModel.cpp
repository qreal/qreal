#include "realRobotModel.h"

#include <qrkernel/settingsManager.h>
#include <qrkernel/exception/exception.h>

#include "parts/display.h"
#include "parts/speaker.h"
#include "parts/buttons.h"
#include "parts/motor.h"
#include "parts/encoderSensor.h"
#include "parts/lightSensor.h"

using namespace trikKitInterpreter::robotModel::real;
using namespace utils::robotCommunication;
using namespace interpreterBase::robotModel;

RealRobotModel::RealRobotModel()
{
}

QString RealRobotModel::name() const
{
	return "TrikRealRobotModel";
}

QString RealRobotModel::friendlyName() const
{
	return tr("Real Robot");
}

bool RealRobotModel::needsConnection() const
{
	return true;
}

void RealRobotModel::rereadSettings()
{
}

void RealRobotModel::connectToRobot()
{
}

void RealRobotModel::disconnectFromRobot()
{
}

robotParts::Device *RealRobotModel::createDevice(PortInfo const &port, DeviceInfo const &deviceInfo)
{
	if (deviceInfo.isA(displayInfo())) {
		return new parts::Display(displayInfo(), port);
	}

	if (deviceInfo.isA(speakerInfo())) {
		return new parts::Speaker(speakerInfo(), port);
	}

	if (deviceInfo.isA(buttonsInfo())) {
		return new parts::Buttons(buttonsInfo(), port);
	}

	if (deviceInfo.isA(motorInfo())) {
		return new parts::Motor(motorInfo(), port);
	}

	if (deviceInfo.isA(encoderInfo())) {
		return new parts::EncoderSensor(encoderInfo(), port);
	}

	if (deviceInfo.isA(lightSensorInfo())) {
		return new parts::LightSensor(lightSensorInfo(), port);
	}

	throw qReal::Exception("Unknown device " + deviceInfo.toString() + " requested on port " + port.name());
}

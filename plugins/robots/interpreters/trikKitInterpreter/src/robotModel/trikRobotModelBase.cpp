#include "trikRobotModelBase.h"

#include <interpreterBase/robotModel/robotParts/display.h>
#include <interpreterBase/robotModel/robotParts/speaker.h>
#include <interpreterBase/robotModel/robotParts/buttons.h>

#include "parts/trikServoMotor.h"
#include "parts/trikPowerMotor.h"
#include <interpreterBase/robotModel/robotParts/encoderSensor.h>

#include <interpreterBase/robotModel/robotParts/lightSensor.h>
#include "parts/trikInfraredSensor.h"

#include "parts/trikSonarSensor.h"

#include "parts/trikLed.h"
#include "parts/trikCameraLineDetector.h"

using namespace trikKitInterpreter::robotModel;
using namespace interpreterBase::robotModel;

TrikRobotModelBase::TrikRobotModelBase()
{
	QList<DeviceInfo> const analogPortConnections = {
		lightSensorInfo()
		, infraredSensorInfo()
	};

	addAllowedConnection(PortInfo("DisplayPort", output), { displayInfo() });
	addAllowedConnection(PortInfo("SpeakerPort", output), { speakerInfo() });
	addAllowedConnection(PortInfo("ButtonsPort", input), { buttonsInfo() });

	addAllowedConnection(PortInfo("JC1", output), { servoMotorInfo() });
	addAllowedConnection(PortInfo("JC2", output), { servoMotorInfo() });
	addAllowedConnection(PortInfo("JC3", output), { servoMotorInfo() });
	addAllowedConnection(PortInfo("JE1", output), { servoMotorInfo() });
	addAllowedConnection(PortInfo("JE2", output), { servoMotorInfo() });
	addAllowedConnection(PortInfo("JE3", output), { servoMotorInfo() });
	addAllowedConnection(PortInfo("JE4", output), { servoMotorInfo() });

	addAllowedConnection(PortInfo("JM1", output), { powerMotorInfo() });
	addAllowedConnection(PortInfo("JM2", output), { powerMotorInfo() });
	addAllowedConnection(PortInfo("JM3", output), { powerMotorInfo() });
	addAllowedConnection(PortInfo("JM4", output), { powerMotorInfo() });

	addAllowedConnection(PortInfo("JB1", input), { encoderInfo() });
	addAllowedConnection(PortInfo("JB2", input), { encoderInfo() });
	addAllowedConnection(PortInfo("JB3", input), { encoderInfo() });
	addAllowedConnection(PortInfo("JB4", input), { encoderInfo() });

	addAllowedConnection(PortInfo("JA1", input), analogPortConnections);
	addAllowedConnection(PortInfo("JA2", input), analogPortConnections);
	addAllowedConnection(PortInfo("JA3", input), analogPortConnections);
	addAllowedConnection(PortInfo("JA4", input), analogPortConnections);
	addAllowedConnection(PortInfo("JA5", input), analogPortConnections);
	addAllowedConnection(PortInfo("JA6", input), analogPortConnections);

	addAllowedConnection(PortInfo("JD1", input), { sonarSensorInfo() });
	addAllowedConnection(PortInfo("JD2", input), { sonarSensorInfo() });

	addAllowedConnection(PortInfo("LedPort", output), { ledInfo() });
	addAllowedConnection(PortInfo("LineDetectorPort", output), { cameraLineDetectorSensorInfo() });
}

QList<DeviceInfo> TrikRobotModelBase::convertibleBases() const
{
	return { DeviceInfo::create<robotParts::LightSensor>()
		, DeviceInfo::create<parts::TrikInfraredSensor>()
		, DeviceInfo::create<parts::TrikSonarSensor>() };
}

DeviceInfo TrikRobotModelBase::displayInfo() const
{
	return DeviceInfo::create<robotParts::Display>();
}

DeviceInfo TrikRobotModelBase::speakerInfo() const
{
	return DeviceInfo::create<robotParts::Speaker>();
}

DeviceInfo TrikRobotModelBase::buttonsInfo() const
{
	return DeviceInfo::create<robotParts::Buttons>();
}

DeviceInfo TrikRobotModelBase::powerMotorInfo() const
{
	return DeviceInfo::create<robotParts::Motor>();
}

DeviceInfo TrikRobotModelBase::servoMotorInfo() const
{
	return DeviceInfo::create<robotParts::Motor>();
}

DeviceInfo TrikRobotModelBase::encoderInfo() const
{
	return DeviceInfo::create<robotParts::EncoderSensor>();
}

DeviceInfo TrikRobotModelBase::lightSensorInfo() const
{
	return DeviceInfo::create<robotParts::LightSensor>();
}

DeviceInfo TrikRobotModelBase::infraredSensorInfo() const
{
	return DeviceInfo::create<parts::TrikInfraredSensor>();
}

DeviceInfo TrikRobotModelBase::sonarSensorInfo() const
{
	return DeviceInfo::create<parts::TrikSonarSensor>();
}

DeviceInfo TrikRobotModelBase::ledInfo() const
{
	return DeviceInfo::create<parts::TrikLed>();
}

DeviceInfo TrikRobotModelBase::cameraLineDetectorSensorInfo() const
{
	return DeviceInfo::create<parts::TrikCameraLineDetector>();
}

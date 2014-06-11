#include "trikRobotModelBase.h"

#include <interpreterBase/robotModel/robotParts/display.h>
#include <interpreterBase/robotModel/robotParts/speaker.h>
#include <interpreterBase/robotModel/robotParts/button.h>

#include "parts/trikServoMotor.h"
#include "parts/trikPowerMotor.h"
#include <interpreterBase/robotModel/robotParts/encoderSensor.h>

#include <interpreterBase/robotModel/robotParts/lightSensor.h>
#include "parts/trikInfraredSensor.h"

#include "parts/trikSonarSensor.h"

#include "parts/trikMovementSensor.h"

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

	addAllowedConnection(PortInfo("LeftButtonPort", input, {}, "leftButton"), { buttonInfo() });
	addAllowedConnection(PortInfo("RightButtonPort", input, {}, "rightButton"), { buttonInfo() });
	addAllowedConnection(PortInfo("UpButtonPort", input, {}, "upButton"), { buttonInfo() });
	addAllowedConnection(PortInfo("DownButtonPort", input, {}, "downButton"), { buttonInfo() });
	addAllowedConnection(PortInfo("EnterButtonPort", input, {}, "enterButton"), { buttonInfo() });
	addAllowedConnection(PortInfo("EscapeButtonPort", input, {}, "escapeButton"), { buttonInfo() });
	addAllowedConnection(PortInfo("PowerButtonPort", input, {}, "powerButton"), { buttonInfo() });

	addAllowedConnection(PortInfo("C1", output, { "JC1" }), { servoMotorInfo() });
	addAllowedConnection(PortInfo("C2", output, { "JC2" }), { servoMotorInfo() });
	addAllowedConnection(PortInfo("C3", output, { "JC3" }), { servoMotorInfo() });
	addAllowedConnection(PortInfo("E1", output, { "JE1" }), { servoMotorInfo() });
	addAllowedConnection(PortInfo("E2", output, { "JE2" }), { servoMotorInfo() });
	addAllowedConnection(PortInfo("E3", output, { "JE3" }), { servoMotorInfo() });
	addAllowedConnection(PortInfo("E4", output, { "JE4" }), { servoMotorInfo() });

	addAllowedConnection(PortInfo("M1", output, { "JM1" }), { powerMotorInfo() });
	addAllowedConnection(PortInfo("M2", output, { "JM2" }), { powerMotorInfo() });
	addAllowedConnection(PortInfo("M3", output, { "JM3" }), { powerMotorInfo() });
	addAllowedConnection(PortInfo("M4", output, { "JM4" }), { powerMotorInfo() });

	addAllowedConnection(PortInfo("B1", input, { "JB1", "M1", "JM1" }, "encoder1"), { encoderInfo() });
	addAllowedConnection(PortInfo("B2", input, { "JB2", "M2", "JM2" }, "encoder2"), { encoderInfo() });
	addAllowedConnection(PortInfo("B3", input, { "JB3", "M3", "JM3" }, "encoder3"), { encoderInfo() });
	addAllowedConnection(PortInfo("B4", input, { "JB4", "M$", "JM4" }, "encoder4"), { encoderInfo() });

	addAllowedConnection(PortInfo("A1", input, { "JA1" }, "sensorA1"), analogPortConnections);
	addAllowedConnection(PortInfo("A2", input, { "JA2" }, "sensorA2"), analogPortConnections);
	addAllowedConnection(PortInfo("A3", input, { "JA3" }, "sensorA3"), analogPortConnections);
	addAllowedConnection(PortInfo("A4", input, { "JA4" }, "sensorA4"), analogPortConnections);
	addAllowedConnection(PortInfo("A5", input, { "JA5" }, "sensorA5"), analogPortConnections);
	addAllowedConnection(PortInfo("A6", input, { "JA6" }, "sensorA6"), analogPortConnections);

	addAllowedConnection(PortInfo("D1", input, { "JD1" }, "sensorD1"), { sonarSensorInfo() });
	addAllowedConnection(PortInfo("D2", input, { "JD2" }, "sensorD2"), { sonarSensorInfo() });

	addAllowedConnection(PortInfo("F1", input, { "JF1" }, "sensorF1"), { movementSensorInfo() });

	addAllowedConnection(PortInfo("LedPort", output), { ledInfo() });
	addAllowedConnection(PortInfo("LineDetectorPort", input), { cameraLineDetectorSensorInfo() });
}

QList<PortInfo> TrikRobotModelBase::configurablePorts() const
{
	QList<PortInfo> const digitalPorts = {
			  PortInfo("D1", input, { "JD1" }, "sensorD1")
			, PortInfo("D2", input, { "JD2" }, "sensorD2")
			, PortInfo("F1", input, { "JF1" }, "sensorF1") };
	return CommonRobotModel::configurablePorts() + digitalPorts;
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

DeviceInfo TrikRobotModelBase::buttonInfo() const
{
	return DeviceInfo::create<robotParts::Button>();
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

DeviceInfo TrikRobotModelBase::movementSensorInfo() const
{
	return DeviceInfo::create<parts::TrikMovementSensor>();
}

DeviceInfo TrikRobotModelBase::ledInfo() const
{
	return DeviceInfo::create<parts::TrikLed>();
}

DeviceInfo TrikRobotModelBase::cameraLineDetectorSensorInfo() const
{
	return DeviceInfo::create<parts::TrikCameraLineDetector>();
}

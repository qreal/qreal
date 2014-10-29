#include "trikRobotModelBase.h"

#include <interpreterBase/robotModel/robotParts/display.h>
#include <interpreterBase/robotModel/robotParts/speaker.h>
#include <interpreterBase/robotModel/robotParts/button.h>
#include <interpreterBase/robotModel/robotParts/gyroscopeSensor.h>
#include <interpreterBase/robotModel/robotParts/accelerometerSensor.h>

#include <interpreterBase/robotModel/robotParts/encoderSensor.h>

#include <interpreterBase/robotModel/robotParts/lightSensor.h>

#include "parts/trikServoMotor.h"
#include "parts/trikPowerMotor.h"
#include "parts/trikInfraredSensor.h"
#include "parts/trikSonarSensor.h"
#include "parts/trikMotionSensor.h"
#include "parts/trikColorSensor.h"
#include "parts/trikLineSensor.h"
#include "parts/trikObjectSensor.h"
#include "parts/trikLed.h"

using namespace trikKitInterpreter::robotModel;
using namespace interpreterBase::robotModel;

TrikRobotModelBase::TrikRobotModelBase(QString const &kitId)
	: CommonRobotModel(kitId)
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

	addAllowedConnection(PortInfo("M1", output, { "JM1", "A", "1" }), { powerMotorInfo() });
	addAllowedConnection(PortInfo("M2", output, { "JM2", "B", "2" }), { powerMotorInfo() });
	addAllowedConnection(PortInfo("M3", output, { "JM3", "C", "3" }), { powerMotorInfo() });
	addAllowedConnection(PortInfo("M4", output, { "JM4", "D", "4" }), { powerMotorInfo() });

	addAllowedConnection(PortInfo("B1", input, { "JB1", "M1", "JM1", "A", "1" }, "encoder1"), { encoderInfo() });
	addAllowedConnection(PortInfo("B2", input, { "JB2", "M2", "JM2", "B", "2" }, "encoder2"), { encoderInfo() });
	addAllowedConnection(PortInfo("B3", input, { "JB3", "M3", "JM3", "C", "3" }, "encoder3"), { encoderInfo() });
	addAllowedConnection(PortInfo("B4", input, { "JB4", "M4", "JM4", "D", "4" }, "encoder4"), { encoderInfo() });

	addAllowedConnection(PortInfo("A1", input, { "JA1" }, "sensorA1"), analogPortConnections);
	addAllowedConnection(PortInfo("A2", input, { "JA2" }, "sensorA2"), analogPortConnections);
	addAllowedConnection(PortInfo("A3", input, { "JA3" }, "sensorA3"), analogPortConnections);
	addAllowedConnection(PortInfo("A4", input, { "JA4" }, "sensorA4"), analogPortConnections);
	addAllowedConnection(PortInfo("A5", input, { "JA5" }, "sensorA5"), analogPortConnections);
	addAllowedConnection(PortInfo("A6", input, { "JA6" }, "sensorA6"), analogPortConnections);

	addAllowedConnection(PortInfo("D1", input, { "JD1" }, "sensorD1"), { sonarSensorInfo() });
	addAllowedConnection(PortInfo("D2", input, { "JD2" }, "sensorD2"), { sonarSensorInfo() });

	addAllowedConnection(PortInfo("F1", input, { "JF1" }, "sensorF1"), { motionSensorInfo() });

	addAllowedConnection(PortInfo("GyroscopePortX", input, {}, "gyroscopeX"), { gyroscopeInfo() });
	addAllowedConnection(PortInfo("GyroscopePortY", input, {}, "gyroscopeY"), { gyroscopeInfo() });
	addAllowedConnection(PortInfo("GyroscopePortZ", input, {}, "gyroscopeZ"), { gyroscopeInfo() });

	addAllowedConnection(PortInfo("AccelerometerPortX", input, {}, "accelerometerX"), { accelerometerInfo() });
	addAllowedConnection(PortInfo("AccelerometerPortY", input, {}, "accelerometerY"), { accelerometerInfo() });
	addAllowedConnection(PortInfo("AccelerometerPortZ", input, {}, "accelerometerZ"), { accelerometerInfo() });

	addAllowedConnection(PortInfo("LedPort", output), { ledInfo() });

	addAllowedConnection(PortInfo("LineSensorXPort", input, {}, "lineSensorX"), { lineSensorInfo() });
	addAllowedConnection(PortInfo("LineSensorSizePort", input, {}, "lineSensorSize"), { lineSensorInfo() });
	addAllowedConnection(PortInfo("LineSensorCrossroadsPort", input, {}, "lineSensorCross"), { lineSensorInfo() });

	addAllowedConnection(PortInfo("ObjectSensorXPort", input, {}, "objectSensorX"), { objectSensorInfo() });
	addAllowedConnection(PortInfo("ObjectSensorYPort", input, {}, "objectSensorY"), { objectSensorInfo() });
	addAllowedConnection(PortInfo("ObjectSensorSizePort", input, {}, "objectSensorSize"), { objectSensorInfo() });

	addAllowedConnection(PortInfo("ColorSensorRPort", input, {}, "colorSensorR"), { colorSensorInfo() });
	addAllowedConnection(PortInfo("ColorSensorGPort", input, {}, "colorSensorG"), { colorSensorInfo() });
	addAllowedConnection(PortInfo("ColorSensorBPort", input, {}, "colorSensorB"), { colorSensorInfo() });
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

DeviceInfo TrikRobotModelBase::motionSensorInfo() const
{
	return DeviceInfo::create<parts::TrikMotionSensor>();
}

DeviceInfo TrikRobotModelBase::gyroscopeInfo() const
{
	return DeviceInfo::create<robotParts::GyroscopeSensor>();
}

DeviceInfo TrikRobotModelBase::accelerometerInfo() const
{
	return DeviceInfo::create<robotParts::AccelerometerSensor>();
}

DeviceInfo TrikRobotModelBase::ledInfo() const
{
	return DeviceInfo::create<parts::TrikLed>();
}

DeviceInfo TrikRobotModelBase::lineSensorInfo() const
{
	return DeviceInfo::create<parts::TrikLineSensor>();
}

DeviceInfo TrikRobotModelBase::colorSensorInfo() const
{
	return DeviceInfo::create<parts::TrikColorSensor>();
}

DeviceInfo TrikRobotModelBase::objectSensorInfo() const
{
	return DeviceInfo::create<parts::TrikObjectSensor>();
}

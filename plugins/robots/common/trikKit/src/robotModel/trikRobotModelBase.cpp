#include "trikKit/robotModel/trikRobotModelBase.h"

#include <kitBase/robotModel/robotParts/display.h>
#include <kitBase/robotModel/robotParts/speaker.h>
#include <kitBase/robotModel/robotParts/button.h>
#include <kitBase/robotModel/robotParts/gyroscopeSensor.h>
#include <kitBase/robotModel/robotParts/accelerometerSensor.h>

#include <kitBase/robotModel/robotParts/encoderSensor.h>

#include <kitBase/robotModel/robotParts/lightSensor.h>

#include "trikKit/robotModel/parts/trikServoMotor.h"
#include "trikKit/robotModel/parts/trikPowerMotor.h"
#include "trikKit/robotModel/parts/trikInfraredSensor.h"
#include "trikKit/robotModel/parts/trikSonarSensor.h"
#include "trikKit/robotModel/parts/trikMotionSensor.h"
#include "trikKit/robotModel/parts/trikColorSensor.h"
#include "trikKit/robotModel/parts/trikLineSensor.h"
#include "trikKit/robotModel/parts/trikObjectSensor.h"
#include "trikKit/robotModel/parts/trikLed.h"
#include "trikKit/robotModel/parts/trikShell.h"
#include "trikKit/robotModel/parts/trikGamepadButton.h"
#include "trikKit/robotModel/parts/trikGamepadConnectionIndicator.h"
#include "trikKit/robotModel/parts/trikGamepadPad.h"
#include "trikKit/robotModel/parts/trikGamepadPadPressSensor.h"
#include "trikKit/robotModel/parts/trikGamepadWheel.h"

using namespace trik::robotModel;
using namespace kitBase::robotModel;

TrikRobotModelBase::TrikRobotModelBase(const QString &kitId, const QString &robotId)
	: CommonRobotModel(kitId, robotId)
{
	QList<DeviceInfo> const analogPortConnections = {
		lightSensorInfo()
		, infraredSensorInfo()
	};

	addAllowedConnection(PortInfo("DisplayPort", output), { displayInfo() });
	addAllowedConnection(PortInfo("SpeakerPort", output), { speakerInfo() });

	addAllowedConnection(PortInfo("Left", input, {}, "buttonLeft"), { buttonInfo() });
	addAllowedConnection(PortInfo("Right", input, {}, "buttonRight"), { buttonInfo() });
	addAllowedConnection(PortInfo("Up", input, {}, "buttonUp"), { buttonInfo() });
	addAllowedConnection(PortInfo("Down", input, {}, "buttonDown"), { buttonInfo() });
	addAllowedConnection(PortInfo("Enter", input, {}, "buttonEnter"), { buttonInfo() });
	addAllowedConnection(PortInfo("Esc", input, {}, "buttonEsc"), { buttonInfo() });

	addAllowedConnection(PortInfo("C1", output, {}), { servoMotorInfo() });
	addAllowedConnection(PortInfo("C2", output, {}), { servoMotorInfo() });
	addAllowedConnection(PortInfo("C3", output, {}), { servoMotorInfo() });
	addAllowedConnection(PortInfo("E1", output, {}), { servoMotorInfo() });
	addAllowedConnection(PortInfo("E2", output, {}), { servoMotorInfo() });
	addAllowedConnection(PortInfo("E3", output, {}), { servoMotorInfo() });
	addAllowedConnection(PortInfo("E4", output, {}), { servoMotorInfo() });

	addAllowedConnection(PortInfo("M1", output, {}), { powerMotorInfo() });
	addAllowedConnection(PortInfo("M2", output, {}), { powerMotorInfo() });
	addAllowedConnection(PortInfo("M3", output, {}), { powerMotorInfo() });
	addAllowedConnection(PortInfo("M4", output, {}), { powerMotorInfo() });

	addAllowedConnection(PortInfo("B1", input, {"M1"}, "encoder1"), { encoderInfo() });
	addAllowedConnection(PortInfo("B2", input, {"M2"}, "encoder2"), { encoderInfo() });
	addAllowedConnection(PortInfo("B3", input, {"M3"}, "encoder3"), { encoderInfo() });
	addAllowedConnection(PortInfo("B4", input, {"M4"}, "encoder4"), { encoderInfo() });

	addAllowedConnection(PortInfo("A1", input, {}, "sensorA1"), analogPortConnections);
	addAllowedConnection(PortInfo("A2", input, {}, "sensorA2"), analogPortConnections);
	addAllowedConnection(PortInfo("A3", input, {}, "sensorA3"), analogPortConnections);
	addAllowedConnection(PortInfo("A4", input, {}, "sensorA4"), analogPortConnections);
	addAllowedConnection(PortInfo("A5", input, {}, "sensorA5"), analogPortConnections);
	addAllowedConnection(PortInfo("A6", input, {}, "sensorA6"), analogPortConnections);

	addAllowedConnection(PortInfo("D1", input, {}, "sensorD1"), { sonarSensorInfo() });
	addAllowedConnection(PortInfo("D2", input, {}, "sensorD2"), { sonarSensorInfo() });

	addAllowedConnection(PortInfo("F1", input, {}, "sensorF1"), { motionSensorInfo() });

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

	addAllowedConnection(PortInfo("ShellPort", output), { shellInfo() });

	addAllowedConnection(PortInfo("GamepadPad1PosPort", input, {}, "gamepadPad1"
			, PortInfo::ReservedVariableType::vector), { gamepadPadInfo() });
	addAllowedConnection(PortInfo("GamepadPad2PosPort", input, {}, "gamepadPad2"
			, PortInfo::ReservedVariableType::vector), { gamepadPadInfo() });

	addAllowedConnection(PortInfo("GamepadPad1PressedPort", input, {}, "gamepadPad1Pressed")
			, { gamepadPadPressSensorInfo() });
	addAllowedConnection(PortInfo("GamepadPad2PressedPort", input, {}, "gamepadPad2Pressed")
			, { gamepadPadPressSensorInfo() });

	addAllowedConnection(PortInfo("GamepadWheelPort", input, {}, "gamepadWheel"), { gamepadWheelInfo() });

	addAllowedConnection(PortInfo("GamepadButton1Port", input, {}, "gamepadButton1"), { gamepadButtonInfo() });
	addAllowedConnection(PortInfo("GamepadButton2Port", input, {}, "gamepadButton2"), { gamepadButtonInfo() });
	addAllowedConnection(PortInfo("GamepadButton3Port", input, {}, "gamepadButton3"), { gamepadButtonInfo() });
	addAllowedConnection(PortInfo("GamepadButton4Port", input, {}, "gamepadButton4"), { gamepadButtonInfo() });
	addAllowedConnection(PortInfo("GamepadButton5Port", input, {}, "gamepadButton5"), { gamepadButtonInfo() });

	addAllowedConnection(PortInfo("GamepadConnectionIndicatorPort", input, {}, "gamepadConnected")
			, { gamepadConnectionIndicatorInfo() });
}

QList<PortInfo> TrikRobotModelBase::configurablePorts() const
{
	QList<PortInfo> const digitalPorts = {
			  PortInfo("D1", input, {}, "sensorD1")
			, PortInfo("D2", input, {}, "sensorD2")
			, PortInfo("F1", input, {}, "sensorF1") };

	return CommonRobotModel::configurablePorts() + digitalPorts;
}

QList<DeviceInfo> TrikRobotModelBase::convertibleBases() const
{
	return { DeviceInfo::create<robotParts::LightSensor>()
		, DeviceInfo::create<parts::TrikInfraredSensor>()
		, DeviceInfo::create<parts::TrikSonarSensor>()
		, DeviceInfo::create<parts::TrikMotionSensor>()
	};
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

DeviceInfo TrikRobotModelBase::shellInfo() const
{
	return DeviceInfo::create<parts::TrikShell>();
}

DeviceInfo TrikRobotModelBase::gamepadButtonInfo() const
{
	return DeviceInfo::create<parts::TrikGamepadButton>();
}

DeviceInfo TrikRobotModelBase::gamepadPadInfo() const
{
	return DeviceInfo::create<parts::TrikGamepadPad>();
}

DeviceInfo TrikRobotModelBase::gamepadPadPressSensorInfo() const
{
	return DeviceInfo::create<parts::TrikGamepadPadPressSensor>();
}

DeviceInfo TrikRobotModelBase::gamepadWheelInfo() const
{
	return DeviceInfo::create<parts::TrikGamepadWheel>();
}

DeviceInfo TrikRobotModelBase::gamepadConnectionIndicatorInfo() const
{
	return DeviceInfo::create<parts::TrikGamepadConnectionIndicator>();
}

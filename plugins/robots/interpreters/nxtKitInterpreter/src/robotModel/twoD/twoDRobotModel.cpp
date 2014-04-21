#include "twoDRobotModel.h"

#include "src/robotModel/twoD/parts/twoDDisplay.h"
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

using namespace nxtKitInterpreter::robotModel;
using namespace nxtKitInterpreter::robotModel::twoD;
using namespace interpreterBase::robotModel;

TwoDRobotModel::TwoDRobotModel()
{
	/// @todo: This is copy-paste from base model. It is unneeded!

	QList<DeviceInfo> const inputPortConnections = {
			DeviceInfo::create<robotParts::TouchSensor>()
			, DeviceInfo::create<robotParts::RangeSensor>()
			, DeviceInfo::create<robotParts::LightSensor>()
			, DeviceInfo::create<robotParts::ColorSensorBlue>()
			, DeviceInfo::create<robotParts::ColorSensorFull>()
			, DeviceInfo::create<robotParts::ColorSensorGreen>()
			, DeviceInfo::create<robotParts::ColorSensorPassive>()
			, DeviceInfo::create<robotParts::ColorSensorRed>()
	};

	addAllowedConnection(PortInfo("DisplayPort", output), { DeviceInfo::create<parts::Display>() });
	addAllowedConnection(PortInfo("SpeakerPort", output), { DeviceInfo::create<robotParts::Speaker>() });
	addAllowedConnection(PortInfo("ButtonsPort", input), { DeviceInfo::create<robotParts::Buttons>() });
	addAllowedConnection(PortInfo("A", output), { DeviceInfo::create<robotParts::Motor>() });
	addAllowedConnection(PortInfo("B", output), { DeviceInfo::create<robotParts::Motor>() });
	addAllowedConnection(PortInfo("C", output), { DeviceInfo::create<robotParts::Motor>() });
	addAllowedConnection(PortInfo("A", input, {}, "encoderA"), { DeviceInfo::create<robotParts::EncoderSensor>() });
	addAllowedConnection(PortInfo("B", input, {}, "encoderB"), { DeviceInfo::create<robotParts::EncoderSensor>() });
	addAllowedConnection(PortInfo("C", input, {}, "encoderC"), { DeviceInfo::create<robotParts::EncoderSensor>() });
	addAllowedConnection(PortInfo("1", input, {}, "sensor1"), inputPortConnections);
	addAllowedConnection(PortInfo("2", input, {}, "sensor2"), inputPortConnections);
	addAllowedConnection(PortInfo("3", input, {}, "sensor3"), inputPortConnections);
	addAllowedConnection(PortInfo("4", input, {}, "sensor4"), inputPortConnections);
}

QString TwoDRobotModel::name() const
{
	return "NxtTwoDRobotModel";
}

QString TwoDRobotModel::friendlyName() const
{
	return tr("2D Model");
}

robotParts::Device *TwoDRobotModel::createDevice(PortInfo const &port, DeviceInfo const &deviceInfo)
{
	if (deviceInfo.isA<robotParts::Display>()) {
		auto display = new parts::Display(deviceInfo, port, *engine());

		/// @todo Why not do this in display constructor?!
		display->attachToPaintWidget();

		return display;
	}

	return twoDModel::robotModel::TwoDRobotModel::createDevice(port, deviceInfo);
}

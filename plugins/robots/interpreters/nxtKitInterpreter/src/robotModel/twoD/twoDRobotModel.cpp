#include "twoDRobotModel.h"

#include <interpreterBase/robotModel/robotParts/display.h>
#include <interpreterBase/robotModel/robotParts/speaker.h>
#include <interpreterBase/robotModel/robotParts/buttons.h>
#include <interpreterBase/robotModel/robotParts/motor.h>
#include <interpreterBase/robotModel/robotParts/encoderSensor.h>
#include <interpreterBase/robotModel/robotParts/touchSensor.h>
#include <interpreterBase/robotModel/robotParts/rangeSensor.h>
#include <interpreterBase/robotModel/robotParts/lightSensor.h>

/// @todo They kind of the same sensors as in real NXT model, but they are not part of interpreterBase model, so
///       they can't be used directly.
#include <commonTwoDModel/robotModel/parts/colorSensorBlue.h>
#include <commonTwoDModel/robotModel/parts/colorSensorFull.h>
#include <commonTwoDModel/robotModel/parts/colorSensorGreen.h>
#include <commonTwoDModel/robotModel/parts/colorSensorPassive.h>
#include <commonTwoDModel/robotModel/parts/colorSensorRed.h>

#include <interpreterBase/robotModel/robotParts/soundSensor.h>
#include <interpreterBase/robotModel/robotParts/gyroscopeSensor.h>
#include <interpreterBase/robotModel/robotParts/accelerometerSensor.h>

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
			, DeviceInfo::create<twoDModel::robotModel::parts::ColorSensorBlue>()
			, DeviceInfo::create<twoDModel::robotModel::parts::ColorSensorFull>()
			, DeviceInfo::create<twoDModel::robotModel::parts::ColorSensorGreen>()
			, DeviceInfo::create<twoDModel::robotModel::parts::ColorSensorPassive>()
			, DeviceInfo::create<twoDModel::robotModel::parts::ColorSensorRed>()
	};

	addAllowedConnection(PortInfo("DisplayPort", output), { DeviceInfo::create<robotParts::Display>() });
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

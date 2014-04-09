#include "twoDRobotModel.h"

#include <interpreterBase/robotModel/robotParts/display.h>
#include <interpreterBase/robotModel/robotParts/speaker.h>
#include <interpreterBase/robotModel/robotParts/buttons.h>
#include <interpreterBase/robotModel/robotParts/motor.h>
#include <interpreterBase/robotModel/robotParts/encoderSensor.h>
#include <interpreterBase/robotModel/robotParts/touchSensor.h>
#include <interpreterBase/robotModel/robotParts/rangeSensor.h>
#include <interpreterBase/robotModel/robotParts/lightSensor.h>
#include <interpreterBase/robotModel/robotParts/colorSensor.h>
#include <interpreterBase/robotModel/robotParts/soundSensor.h>
#include <interpreterBase/robotModel/robotParts/gyroscopeSensor.h>
#include <interpreterBase/robotModel/robotParts/accelerometerSensor.h>

using namespace nxtKitInterpreter::robotModel;
using namespace nxtKitInterpreter::robotModel::twoD;
using namespace interpreterBase::robotModel;

TwoDRobotModel::TwoDRobotModel()
{
	/// @todo Implement.
	QList<DeviceInfo> const outputPortConnections = {
		DeviceInfo::create<robotParts::Motor>()
		/// @todo: implement multidevice architecture
		//, encoderInfo()
	};

	QList<DeviceInfo> const inputPortConnections = {
			DeviceInfo::create<robotParts::TouchSensor>()
			, DeviceInfo::create<robotParts::RangeSensor>()
			, DeviceInfo::create<robotParts::LightSensor>()
			, DeviceInfo::create<robotParts::ColorSensor>()
			, DeviceInfo::create<robotParts::SoundSensor>()
			, DeviceInfo::create<robotParts::GyroscopeSensor>()
			, DeviceInfo::create<robotParts::AccelerometerSensor>()
	};

	addAllowedConnection(PortInfo("DisplayPort"), { DeviceInfo::create<robotParts::Display>() });
	addAllowedConnection(PortInfo("SpeakerPort"), { DeviceInfo::create<robotParts::Speaker>() });
	addAllowedConnection(PortInfo("ButtonsPort"), { DeviceInfo::create<robotParts::Buttons>() });
	addAllowedConnection(PortInfo("A"), outputPortConnections);
	addAllowedConnection(PortInfo("B"), outputPortConnections);
	addAllowedConnection(PortInfo("C"), outputPortConnections);
	addAllowedConnection(PortInfo("1"), inputPortConnections);
	addAllowedConnection(PortInfo("2"), inputPortConnections);
	addAllowedConnection(PortInfo("3"), inputPortConnections);
	addAllowedConnection(PortInfo("4"), inputPortConnections);
}

QString TwoDRobotModel::name() const
{
	return "NxtTwoDRobotModel";
}

QString TwoDRobotModel::friendlyName() const
{
	return tr("2D Model");
}

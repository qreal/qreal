#include "twoDRobotModel.h"

#include "src/robotModel/twoD/parts/twoDDisplay.h"
#include "src/robotModel/twoD/parts/twoDButtons.h"
#include "src/robotModel/parts/trikInfraredSensor.h"
#include "src/robotModel/parts/trikSonarSensor.h"
#include "src/robotModel/parts/trikLed.h"

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

TwoDRobotModel::TwoDRobotModel()
{
	/// @todo: This is copy-paste from base model. It is unneeded!

	QList<DeviceInfo> const analogPortConnections = {
		DeviceInfo::create<robotParts::LightSensor>()
		, DeviceInfo::create<trikKitInterpreter::robotModel::parts::TrikInfraredSensor>()
	};

	addAllowedConnection(PortInfo("DisplayPort", output), { DeviceInfo::create<parts::Display>() });
	addAllowedConnection(PortInfo("SpeakerPort", output), { DeviceInfo::create<robotParts::Speaker>() });
	addAllowedConnection(PortInfo("ButtonsPort", input), { DeviceInfo::create<parts::TwoDButtons>() });

	addAllowedConnection(PortInfo("JC1", output), { DeviceInfo::create<robotParts::Motor>() });
	addAllowedConnection(PortInfo("JC2", output), { DeviceInfo::create<robotParts::Motor>() });
	addAllowedConnection(PortInfo("JC3", output), { DeviceInfo::create<robotParts::Motor>() });
	addAllowedConnection(PortInfo("JE1", output), { DeviceInfo::create<robotParts::Motor>() });
	addAllowedConnection(PortInfo("JE2", output), { DeviceInfo::create<robotParts::Motor>() });
	addAllowedConnection(PortInfo("JE3", output), { DeviceInfo::create<robotParts::Motor>() });
	addAllowedConnection(PortInfo("JE4", output), { DeviceInfo::create<robotParts::Motor>() });

	addAllowedConnection(PortInfo("JM1", output), { DeviceInfo::create<robotParts::Motor>() });
	addAllowedConnection(PortInfo("JM2", output), { DeviceInfo::create<robotParts::Motor>() });
	addAllowedConnection(PortInfo("JM3", output), { DeviceInfo::create<robotParts::Motor>() });
	addAllowedConnection(PortInfo("JM4", output), { DeviceInfo::create<robotParts::Motor>() });

	addAllowedConnection(PortInfo("JB1", input), { DeviceInfo::create<robotParts::EncoderSensor>() });
	addAllowedConnection(PortInfo("JB2", input), { DeviceInfo::create<robotParts::EncoderSensor>() });
	addAllowedConnection(PortInfo("JB3", input), { DeviceInfo::create<robotParts::EncoderSensor>() });
	addAllowedConnection(PortInfo("JB4", input), { DeviceInfo::create<robotParts::EncoderSensor>() });

	addAllowedConnection(PortInfo("JA1", input), analogPortConnections);
	addAllowedConnection(PortInfo("JA2", input), analogPortConnections);
	addAllowedConnection(PortInfo("JA3", input), analogPortConnections);
	addAllowedConnection(PortInfo("JA4", input), analogPortConnections);
	addAllowedConnection(PortInfo("JA5", input), analogPortConnections);
	addAllowedConnection(PortInfo("JA6", input), analogPortConnections);

	addAllowedConnection(PortInfo("JD1", input), {
			DeviceInfo::create<trikKitInterpreter::robotModel::parts::TrikSonarSensor>()
	});

	addAllowedConnection(PortInfo("JD2", input), {
			DeviceInfo::create<trikKitInterpreter::robotModel::parts::TrikSonarSensor>()
	});

	addAllowedConnection(PortInfo("LedPort", output), {
			DeviceInfo::create<trikKitInterpreter::robotModel::parts::TrikLed>()
	});
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

	if (deviceInfo.isA<robotParts::Buttons>()) {
		return new parts::TwoDButtons(deviceInfo, port, *engine());
	}

	return twoDModel::robotModel::TwoDRobotModel::createDevice(port, deviceInfo);
}

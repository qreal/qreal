#include "nxtTwoDModelConfigurer.h"

#include <interpreterBase/robotModel/robotParts/lightSensor.h>
#include <interpreterBase/robotModel/robotParts/colorSensor.h>
#include <interpreterBase/robotModel/robotParts/rangeSensor.h>
#include <interpreterBase/robotModel/robotParts/touchSensor.h>

#include "nxtDisplayWidget.h"

using namespace nxtKitInterpreter;
using namespace interpreterBase::robotModel;

QString NxtTwoDModelConfigurer::robotImage() const
{
	return ":/icons/robot.png";
}

PortInfo NxtTwoDModelConfigurer::defaultLeftWheelPort() const
{
	return PortInfo("B", output);
}

PortInfo NxtTwoDModelConfigurer::defaultRightWheelPort() const
{
	return PortInfo("C", output);
}

twoDModel::engine::TwoDModelDisplayWidget *NxtTwoDModelConfigurer::displayWidget(QWidget * parent) const
{
	return new NxtDisplayWidget(parent);
}

QRect NxtTwoDModelConfigurer::sensorImageRect(interpreterBase::robotModel::DeviceInfo const &deviceType) const
{
	if (deviceType.isA<robotParts::TouchSensor>()) {
		return QRect(-12, -5, 25, 10);
	} else if (deviceType.isA<robotParts::ColorSensor>()
			|| deviceType.isA<robotParts::LightSensor>())
	{
		return QRect(-6, -6, 12, 12);
	}
	if (deviceType.isA<robotParts::RangeSensor>()) {
		return QRect(-20, -10, 40, 20);;
	} else {
		Q_ASSERT(!"Unknown sensor type");
		return QRect();
	}
}

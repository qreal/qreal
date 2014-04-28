#include "trikTwoDModelConfigurer.h"

using namespace trikKitInterpreter;
using namespace interpreterBase::robotModel;

QString TrikTwoDModelConfigurer::robotImage() const
{
	return ":icons/trikTwoDRobot.svg";
}

PortInfo TrikTwoDModelConfigurer::defaultLeftWheelPort() const
{
	/// @todo make it smarter and take into account selected model (v4 or v6)
	return PortInfo("M1", output);
}

PortInfo TrikTwoDModelConfigurer::defaultRightWheelPort() const
{
	return PortInfo("JM3", output);
}

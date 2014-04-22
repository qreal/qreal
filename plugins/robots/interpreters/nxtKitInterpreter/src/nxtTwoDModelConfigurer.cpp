#include "nxtTwoDModelConfigurer.h"

using namespace nxtKitInterpreter;
using namespace interpreterBase::robotModel;

QString NxtTwoDModelConfigurer::robotImage() const
{
	return "";
}

PortInfo NxtTwoDModelConfigurer::defaultLeftWheelPort() const
{
	return PortInfo("B", output);
}

PortInfo NxtTwoDModelConfigurer::defaultRightWheelPort() const
{
	return PortInfo("C", output);
}

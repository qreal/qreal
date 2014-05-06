#include "nxtTwoDModelConfigurer.h"

#include "nxtDisplay.h"

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
	return new NxtDisplay(parent);
}

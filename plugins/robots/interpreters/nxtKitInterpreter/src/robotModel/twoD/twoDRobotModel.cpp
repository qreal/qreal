#include "twoDRobotModel.h"

using namespace nxtKitInterpreter::robotModel::twoD;

QString TwoDRobotModel::name() const
{
	return "NxtTwoDRobotModel";
}

QString TwoDRobotModel::friendlyName() const
{
	return tr("2D Model");
}

bool TwoDRobotModel::needsConnection() const
{
	return false;
}

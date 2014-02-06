#include "twoDRobotModel.h"

using namespace nxtKitInterpreter::robotModel::real;

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

#include "defaultRobotModel.h"

using namespace interpreterCore;

DefaultRobotModel::DefaultRobotModel()
	: CommonRobotModel("default", "")
{
}

QString interpreterCore::DefaultRobotModel::name() const
{
	return "DefaultModel";
}

QString DefaultRobotModel::friendlyName() const
{
	return tr("Empty model");
}

bool interpreterCore::DefaultRobotModel::needsConnection() const
{
	return false;
}

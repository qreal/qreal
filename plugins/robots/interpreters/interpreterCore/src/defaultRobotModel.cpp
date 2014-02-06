#include "defaultRobotModel.h"

using namespace interpreterCore;

QString interpreterCore::DefaultRobotModel::name() const
{
	return "DefaultModel";
}

QString interpreterCore::DefaultRobotModel::friendlyName() const
{
	return tr("Empty model");
}

bool interpreterCore::DefaultRobotModel::needsConnection() const
{
	return false;
}

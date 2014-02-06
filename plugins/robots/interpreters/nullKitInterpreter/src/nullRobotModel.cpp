#include "nullRobotModel.h"

using namespace nullKitInterpreter;

QString NullRobotModel::name() const
{
	return "NullModel";
}

QString NullRobotModel::friendlyName() const
{
	return tr("Null model");
}

bool NullRobotModel::needsConnection() const
{
	return false;
}

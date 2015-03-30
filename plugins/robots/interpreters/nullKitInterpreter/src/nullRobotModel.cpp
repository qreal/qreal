#include "nullRobotModel.h"

using namespace nullKitInterpreter;

NullRobotModel::NullRobotModel(const QString &kitId)
	: kitBase::robotModel::CommonRobotModel(kitId, "")
{
}

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

int NullRobotModel::priority() const
{
	return 0;
}

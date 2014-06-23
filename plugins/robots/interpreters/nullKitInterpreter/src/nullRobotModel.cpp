#include "nullRobotModel.h"

using namespace nullKitInterpreter;

NullRobotModel::NullRobotModel(QString const &kitId)
	: interpreterBase::robotModel::CommonRobotModel(kitId)
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

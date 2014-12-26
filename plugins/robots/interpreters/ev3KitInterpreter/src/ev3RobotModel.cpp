#include "ev3RobotModel.h"

using namespace ev3KitInterpreter;

Ev3RobotModel::Ev3RobotModel(QString const &kitId)
	: interpreterBase::robotModel::CommonRobotModel(kitId, "")
{
}

QString Ev3RobotModel::name() const
{
	return "Ev3Model";
}

QString Ev3RobotModel::friendlyName() const
{
	return tr("EV3 - comming soon!");
}

bool Ev3RobotModel::needsConnection() const
{
	return false;
}

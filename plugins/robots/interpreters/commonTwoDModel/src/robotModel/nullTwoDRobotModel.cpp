#include "commonTwoDModel/robotModel/nullTwoDRobotModel.h"

using namespace twoDModel::robotModel;

NullTwoDRobotModel::NullTwoDRobotModel(const QString &robotId)
	: TwoDRobotModel(robotId)
{
}

QString NullTwoDRobotModel::name() const
{
	return "NullTwoDRobotModel";
}
QString NullTwoDRobotModel::friendlyName() const
{
	return "";
}

QList<interpreterBase::robotModel::PortInfo> NullTwoDRobotModel::configurablePorts() const
{
	return QList<interpreterBase::robotModel::PortInfo>();
}
QList<interpreterBase::robotModel::DeviceInfo> NullTwoDRobotModel::convertibleBases() const
{
	return QList<interpreterBase::robotModel::DeviceInfo>();
}

QString NullTwoDRobotModel::robotImage() const
{
	return ":icons/trikTwoDRobot.svg";
}

interpreterBase::robotModel::PortInfo NullTwoDRobotModel::defaultLeftWheelPort() const
{
	return interpreterBase::robotModel::PortInfo();
}

interpreterBase::robotModel::PortInfo NullTwoDRobotModel::defaultRightWheelPort() const
{
	return interpreterBase::robotModel::PortInfo();
}

twoDModel::engine::TwoDModelDisplayWidget *NullTwoDRobotModel::displayWidget(QWidget * parent) const
{
	return new twoDModel::engine::NullTwoDModelDisplayWidget(parent);
}

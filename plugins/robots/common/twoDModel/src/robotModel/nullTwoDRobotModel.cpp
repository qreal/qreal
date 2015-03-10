#include "twoDModel/robotModel/nullTwoDRobotModel.h"

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

QList<kitBase::robotModel::PortInfo> NullTwoDRobotModel::configurablePorts() const
{
	return QList<kitBase::robotModel::PortInfo>();
}
QList<kitBase::robotModel::DeviceInfo> NullTwoDRobotModel::convertibleBases() const
{
	return QList<kitBase::robotModel::DeviceInfo>();
}

QString NullTwoDRobotModel::robotImage() const
{
	return ":icons/trikTwoDRobot.svg";
}

kitBase::robotModel::PortInfo NullTwoDRobotModel::defaultLeftWheelPort() const
{
	return kitBase::robotModel::PortInfo();
}

kitBase::robotModel::PortInfo NullTwoDRobotModel::defaultRightWheelPort() const
{
	return kitBase::robotModel::PortInfo();
}

twoDModel::engine::TwoDModelDisplayWidget *NullTwoDRobotModel::displayWidget(QWidget * parent) const
{
	return new twoDModel::engine::NullTwoDModelDisplayWidget(parent);
}

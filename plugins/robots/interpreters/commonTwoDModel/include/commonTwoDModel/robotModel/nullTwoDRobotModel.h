#pragma once

#include "twoDRobotModel.h"

namespace twoDModel {

namespace engine {
class TwoDModelEngineInterface;
}

namespace robotModel {

class NullTwoDRobotModel : public twoDModel::robotModel::TwoDRobotModel
{
	Q_OBJECT

public:
	NullTwoDRobotModel()
		: TwoDRobotModel()
	{
	}

	QString name() const override
	{
		return "";
	}
	QString friendlyName() const override
	{
		return "";
	}

	QList<interpreterBase::robotModel::PortInfo> configurablePorts() const override
	{
		return QList<interpreterBase::robotModel::PortInfo>();
	}
	QList<interpreterBase::robotModel::DeviceInfo> convertibleBases() const override
	{
		return QList<interpreterBase::robotModel::DeviceInfo>();
	}

	QString robotImage() const
	{
		return ":icons/trikTwoDRobot.svg";
	}

	interpreterBase::robotModel::PortInfo defaultLeftWheelPort() const
	{
		return interpreterBase::robotModel::PortInfo();
	}

	virtual interpreterBase::robotModel::PortInfo defaultRightWheelPort() const
	{
		return interpreterBase::robotModel::PortInfo();
	}

	engine::TwoDModelDisplayWidget *displayWidget(QWidget * parent) const
	{
		Q_UNUSED(parent)
		return nullptr;
	}
};

}
}

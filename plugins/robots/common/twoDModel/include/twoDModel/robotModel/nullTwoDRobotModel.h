#pragma once

#include "twoDRobotModel.h"
#include "include/twoDModel/engine/nullTwoDModelDisplayWidget.h"

namespace twoDModel {

namespace engine {
class TwoDModelEngineInterface;
class NullTwoDModelDisplayWidget;
}

namespace robotModel {

class NullTwoDRobotModel : public twoDModel::robotModel::TwoDRobotModel
{
	Q_OBJECT

public:
	NullTwoDRobotModel(const QString &robotId);

	QString name() const override;
	QString friendlyName() const override;
	QList<kitBase::robotModel::PortInfo> configurablePorts() const override;
	QList<kitBase::robotModel::DeviceInfo> convertibleBases() const override;
	QString robotImage() const;
	kitBase::robotModel::PortInfo defaultLeftWheelPort() const;
	kitBase::robotModel::PortInfo defaultRightWheelPort() const override;

	engine::TwoDModelDisplayWidget *displayWidget(QWidget * parent) const;
};

}
}

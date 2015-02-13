#pragma once

#include "twoDRobotModel.h"
#include "include/commonTwoDModel/engine/nullTwoDModelDisplayWidget.h"

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
	QList<interpreterBase::robotModel::PortInfo> configurablePorts() const override;
	QList<interpreterBase::robotModel::DeviceInfo> convertibleBases() const override;
	QString robotImage() const;
	interpreterBase::robotModel::PortInfo defaultLeftWheelPort() const;
	interpreterBase::robotModel::PortInfo defaultRightWheelPort() const override;

	engine::TwoDModelDisplayWidget *displayWidget(QWidget * parent) const;
};

}
}

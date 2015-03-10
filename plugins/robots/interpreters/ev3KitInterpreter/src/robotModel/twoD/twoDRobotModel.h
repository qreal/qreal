#pragma once

#include <commonTwoDModel/robotModel/twoDRobotModel.h>

namespace nxtKitInterpreter {
namespace robotModel {
namespace twoD {

class TwoDRobotModel : public twoDModel::robotModel::TwoDRobotModel
{
	Q_OBJECT

public:
	explicit TwoDRobotModel(kitBase::robotModel::RobotModelInterface const &realModel);

	QString robotImage() const override;
	kitBase::robotModel::PortInfo defaultLeftWheelPort() const override;
	kitBase::robotModel::PortInfo defaultRightWheelPort() const override;
	twoDModel::engine::TwoDModelDisplayWidget *displayWidget(QWidget * parent) const override;
	QRect sensorImageRect(kitBase::robotModel::DeviceInfo const &deviceType) const;

protected:
	kitBase::robotModel::robotParts::Device *createDevice(
			kitBase::robotModel::PortInfo const &port
			, kitBase::robotModel::DeviceInfo const &deviceInfo
			) override;
};

}
}
}

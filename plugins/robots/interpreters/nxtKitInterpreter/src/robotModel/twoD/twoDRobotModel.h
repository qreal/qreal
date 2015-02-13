#pragma once

#include <commonTwoDModel/robotModel/twoDRobotModel.h>

namespace nxtKitInterpreter {
namespace robotModel {
namespace twoD {

class TwoDRobotModel : public twoDModel::robotModel::TwoDRobotModel
{
	Q_OBJECT

public:
	explicit TwoDRobotModel(interpreterBase::robotModel::RobotModelInterface &realModel);

	QString robotImage() const override;
	interpreterBase::robotModel::PortInfo defaultLeftWheelPort() const override;
	interpreterBase::robotModel::PortInfo defaultRightWheelPort() const override;
	twoDModel::engine::TwoDModelDisplayWidget *displayWidget(QWidget * parent) const override;
	QRect sensorImageRect(const interpreterBase::robotModel::DeviceInfo &deviceType) const;

protected:
	interpreterBase::robotModel::robotParts::Device *createDevice(
			const interpreterBase::robotModel::PortInfo &port
			, const interpreterBase::robotModel::DeviceInfo &deviceInfo
			) override;
};

}
}
}

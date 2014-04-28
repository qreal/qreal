#pragma once

#include <commonTwoDModel/robotModel/twoDRobotModel.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace twoD {

class TwoDRobotModel : public twoDModel::robotModel::TwoDRobotModel
{
	Q_OBJECT

public:
	TwoDRobotModel(interpreterBase::robotModel::RobotModelInterface const &realModel);
	QString name() const override;
	QString friendlyName() const override;

protected:
	interpreterBase::robotModel::robotParts::Device *createDevice(
			interpreterBase::robotModel::PortInfo const &port
			, interpreterBase::robotModel::DeviceInfo const &deviceInfo
			) override;

private:
	interpreterBase::robotModel::RobotModelInterface const &mRealModel;
};

}
}
}

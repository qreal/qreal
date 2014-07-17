#pragma once

#include <commonTwoDModel/robotModel/twoDRobotModel.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace twoD {

class TwoDRobotModel : public twoDModel::robotModel::TwoDRobotModel
{
	Q_OBJECT

public:
	explicit TwoDRobotModel(interpreterBase::robotModel::RobotModelInterface const &realModel);

	QString robotImage() const override;
	interpreterBase::robotModel::PortInfo defaultLeftWheelPort() const override;
	interpreterBase::robotModel::PortInfo defaultRightWheelPort() const override;
	twoDModel::engine::TwoDModelDisplayWidget *displayWidget(QWidget * parent) const override;
	QString sensorImagePath(const interpreterBase::robotModel::DeviceInfo &deviceType) const override;

	void setWheelPorts(QString const &leftWheelPort, QString const &rightWheelPort);

private:
	interpreterBase::robotModel::robotParts::Device *createDevice(
			interpreterBase::robotModel::PortInfo const &port
			, interpreterBase::robotModel::DeviceInfo const &deviceInfo
			) override;

	void onInterpretationStarted() override;

	QString const mLeftWheelPort;
	QString const mRightWheelPort;
};

}
}
}

#pragma once

#include <commonTwoDModel/robotModel/twoDRobotModel.h>

namespace trikKitInterpreter {
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
	QString sensorImagePath(const interpreterBase::robotModel::DeviceInfo &deviceType) const override;
	QRect sensorImageRect(const interpreterBase::robotModel::DeviceInfo &deviceType) const;

	void setWheelPorts(const QString &leftWheelPort, const QString &rightWheelPort);

private:
	interpreterBase::robotModel::robotParts::Device *createDevice(
			const interpreterBase::robotModel::PortInfo &port
			, const interpreterBase::robotModel::DeviceInfo &deviceInfo
			) override;

	void onInterpretationStarted() override;

	QString mLeftWheelPort;
	QString mRightWheelPort;
};

}
}
}

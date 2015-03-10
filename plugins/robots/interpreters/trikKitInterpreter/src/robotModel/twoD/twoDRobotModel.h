#pragma once

#include <twoDModel/robotModel/twoDRobotModel.h>

namespace trik {
namespace robotModel {
namespace twoD {

class TwoDRobotModel : public twoDModel::robotModel::TwoDRobotModel
{
	Q_OBJECT

public:
	explicit TwoDRobotModel(kitBase::robotModel::RobotModelInterface &realModel);

	QString robotImage() const override;
	kitBase::robotModel::PortInfo defaultLeftWheelPort() const override;
	kitBase::robotModel::PortInfo defaultRightWheelPort() const override;
	twoDModel::engine::TwoDModelDisplayWidget *displayWidget(QWidget * parent) const override;
	QString sensorImagePath(const kitBase::robotModel::DeviceInfo &deviceType) const override;
	QRect sensorImageRect(const kitBase::robotModel::DeviceInfo &deviceType) const;

	void setWheelPorts(const QString &leftWheelPort, const QString &rightWheelPort);

private:
	kitBase::robotModel::robotParts::Device *createDevice(
			const kitBase::robotModel::PortInfo &port
			, const kitBase::robotModel::DeviceInfo &deviceInfo
			) override;

	void onInterpretationStarted() override;

	QString mLeftWheelPort;
	QString mRightWheelPort;
};

}
}
}

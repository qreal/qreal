#pragma once

#include <commonTwoDModel/engine/configurer.h>

namespace trikKitInterpreter {

class TrikTwoDModelConfigurer : public twoDModel::Configurer
{
public:
	TrikTwoDModelConfigurer(QString const &leftWheelPort, QString const &rightWheelPort);

	QString robotImage() const override;
	interpreterBase::robotModel::PortInfo defaultLeftWheelPort() const override;
	interpreterBase::robotModel::PortInfo defaultRightWheelPort() const override;
	twoDModel::engine::TwoDModelDisplayWidget *displayWidget(QWidget * parent) const override;
	QString sensorImagePath(const interpreterBase::robotModel::DeviceInfo &deviceType) const override;

private:
	QString const mLeftWheelPort;
	QString const mRightWheelPort;
};

}

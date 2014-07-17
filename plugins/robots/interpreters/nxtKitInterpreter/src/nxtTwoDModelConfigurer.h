#pragma once

#include <commonTwoDModel/engine/configurer.h>

namespace nxtKitInterpreter {

class NxtTwoDModelConfigurer : public twoDModel::Configurer
{
public:
	QString robotImage() const override;
	interpreterBase::robotModel::PortInfo defaultLeftWheelPort() const override;
	interpreterBase::robotModel::PortInfo defaultRightWheelPort() const override;
	twoDModel::engine::TwoDModelDisplayWidget *displayWidget(QWidget * parent) const override;
	QRect sensorImageRect(interpreterBase::robotModel::DeviceInfo const &deviceType) const;
};

}

#pragma once

#include <commonTwoDModel/engine/configurer.h>

namespace nxtKitInterpreter {

class NxtTwoDModelConfigurer : public twoDModel::Configurer
{
public:
	QString robotImage() const override;
	interpreterBase::robotModel::PortInfo defaultLeftWheelPort() const override;
	interpreterBase::robotModel::PortInfo defaultRightWheelPort() const override;
};

}

#pragma once

#include <QtCore/QString>

#include <interpreterBase/robotModel/portInfo.h>

#include <utils/abstractTimer.h>

namespace twoDModel {
namespace engine {

class TwoDModelEngineInterface
{
public:
	virtual ~TwoDModelEngineInterface() {}

	virtual void setNewMotor(int speed, uint degrees
			, interpreterBase::robotModel::PortInfo const &port, bool breakMode) = 0;

	virtual utils::AbstractTimer *modelTimer() const = 0;
};

}
}

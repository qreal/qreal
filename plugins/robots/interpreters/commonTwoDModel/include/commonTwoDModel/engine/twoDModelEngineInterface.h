#pragma once

#include <QtCore/QString>

#include <utils/abstractTimer.h>
#include <interpreterBase/robotModel/portInfo.h>

#include "commonTwoDModel/engine/twoDModelDisplayInterface.h"

namespace twoDModel {
namespace engine {

class TwoDModelEngineInterface
{
public:
	virtual ~TwoDModelEngineInterface() {}

	virtual void setNewMotor(int speed, uint degrees
			, interpreterBase::robotModel::PortInfo const &port, bool breakMode) = 0;

	virtual int readEncoder(interpreterBase::robotModel::PortInfo const &port) const = 0;
	virtual void resetEncoder(interpreterBase::robotModel::PortInfo const &port) = 0;

	virtual int readTouchSensor(interpreterBase::robotModel::PortInfo const &port) const = 0;
	virtual int readSonarSensor(interpreterBase::robotModel::PortInfo const &port) const = 0;
	virtual int readColorSensor(interpreterBase::robotModel::PortInfo const &port) const = 0;
	virtual int readLightSensor(interpreterBase::robotModel::PortInfo const &port) const = 0;

	virtual utils::AbstractTimer *modelTimer() const = 0;

	virtual TwoDModelDisplayInterface *display() = 0;

};

}
}

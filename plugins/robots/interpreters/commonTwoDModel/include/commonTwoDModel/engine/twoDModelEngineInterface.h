#pragma once

#include <QtCore/QString>

#include <utils/abstractTimer.h>

namespace twoDModel {
namespace engine {

class TwoDModelEngineInterface
{
public:
	virtual ~TwoDModelEngineInterface() {}

	virtual void setNewMotor(int speed, uint degrees, int port, bool breakMode) = 0;

	virtual utils::AbstractTimer *modelTimer() const = 0;
};

}
}

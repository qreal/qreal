#pragma once

#include <QtCore/QString>

namespace twoDModel {
namespace engine {

class TwoDModelEngineInterface
{
public:
	virtual ~TwoDModelEngineInterface() {}

	virtual void setNewMotor(int speed, uint degrees, int port, bool breakMode) = 0;
};

}
}

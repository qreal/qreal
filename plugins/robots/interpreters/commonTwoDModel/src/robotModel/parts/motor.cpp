#include "commonTwoDModel/robotModel/parts/motor.h"

#include "commonTwoDModel/engine/twoDModelEngineInterface.h"

using namespace twoDModel::robotModel::parts;
using namespace interpreterBase;
using namespace robotModel;

Motor::Motor(DeviceInfo const &info, PortInfo const &port, engine::TwoDModelEngineInterface &engine)
	: interpreterBase::robotModel::robotParts::Motor(info, port)
	, mEngine(engine)
{
}

void Motor::on(int speed)
{
	on(speed, true);
}

void Motor::stop()
{
	stop(true);
}

void Motor::on(int speed, bool breakMode)
{
	on(speed, 0, breakMode);
}

void Motor::on(int speed, long unsigned int degrees, bool breakMode)
{
	mEngine.setNewMotor(speed, degrees, port(), breakMode);
}

void Motor::stop(bool breakMode)
{
	on(0, 0, breakMode);
}

void Motor::off()
{
	stop(false);
}

void Motor::doConfiguration()
{
	/// @todo Remove this to base class.
	configurationCompleted(true);
}

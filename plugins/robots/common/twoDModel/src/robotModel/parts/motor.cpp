#include "twoDModel/robotModel/parts/motor.h"

#include "twoDModel/engine/twoDModelEngineInterface.h"

using namespace twoDModel::robotModel::parts;
using namespace kitBase;
using namespace robotModel;

Motor::Motor(const DeviceInfo &info, const PortInfo &port, engine::TwoDModelEngineInterface &engine)
	: kitBase::robotModel::robotParts::Motor(info, port)
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
	kitBase::robotModel::robotParts::Motor::on(speed);
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

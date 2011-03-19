#include "motor.h"
#include "../robotImplementations/motorImplementations/abstractMotorImplementation.h"

using namespace qReal::interpreters::robots::details;
using namespace robotImplementations::motorImplementations;
using namespace robotParts;

Motor::Motor(int const port, robotImplementations::motorImplementations::AbstractMotorImplementation *motorImpl)
	: mPort(static_cast<outputPort::OutputPortEnum>(port)), mMotorImpl(motorImpl)
{
	mMotorImpl->setPort(mPort);
}

Motor::~Motor()
{
}

AbstractMotorImplementation &Motor::motorImpl()
{
	return *mMotorImpl;
}

void Motor::on(int speed)
{
	mMotorImpl->on(speed);
}

void Motor::on(int speed, long unsigned int degrees)
{
	mMotorImpl->on(speed, degrees);
}

void Motor::stop()
{
	mMotorImpl->stop();
}

void Motor::off()
{
	mMotorImpl->off();
}

void Motor::resetMotorPosition(bool relative)
{
	mMotorImpl->resetMotorPosition(relative);
}

void Motor::setImplementation(robotImplementations::motorImplementations::AbstractMotorImplementation *motorImpl)
{
	mMotorImpl = motorImpl;
	mMotorImpl->setPort(mPort);
}

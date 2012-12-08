#include "unrealMotorImplementation.h"
using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::motorImplementations;

UnrealMotorImplementation::UnrealMotorImplementation(int const port, d2Model::D2RobotModel *d2Model)
	: AbstractMotorImplementation(port)
	, mD2Model(d2Model)
{
}

void UnrealMotorImplementation::on(int speed)
{
	on(speed, 0);
}

void UnrealMotorImplementation::on(int speed, long unsigned int degrees)
{

	mD2Model->setNewMotor(speed, degrees, mPort);
}

void UnrealMotorImplementation::stop()
{
	mD2Model->setNewMotor(0, 0, mPort);
}

void UnrealMotorImplementation::off()
{
	mD2Model->setNewMotor(0, 0, mPort);
}

void UnrealMotorImplementation::resetMotorPosition(bool relative)
{
	Q_UNUSED(relative);
}

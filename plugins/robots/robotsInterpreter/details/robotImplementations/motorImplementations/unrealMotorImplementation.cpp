#include "unrealMotorImplementation.h"
using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::motorImplementations;

UnrealMotorImplementation::UnrealMotorImplementation(int const port, d2Model::D2RobotModel *d2Model)
	: AbstractMotorImplementation(port)
	, mD2Model(d2Model)
{
}

void UnrealMotorImplementation::on(int speed, bool breakMode)
{
	on(speed, 0, breakMode);
}

void UnrealMotorImplementation::on(int speed, long unsigned int degrees, bool breakMode)
{
	mD2Model->setNewMotor(speed, degrees, mPort, breakMode);
	connect(mD2Model, SIGNAL(d2MotorTimeout()), this, SIGNAL(motorImplTimeout()));
}

void UnrealMotorImplementation::stop(bool breakMode)
{
	mD2Model->setNewMotor(0, 0, mPort, breakMode);
}

void UnrealMotorImplementation::off()
{
	mD2Model->setNewMotor(0, 0, mPort, true);
}

void UnrealMotorImplementation::resetMotorPosition(bool relative)
{
	Q_UNUSED(relative);
}

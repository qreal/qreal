#include "unrealBrickImplementation.h"
using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::brickImplementations;

UnrealBrickImplementation::UnrealBrickImplementation(d2Model::D2RobotModel *d2Model)
	: mD2Model(d2Model)
{
}

void UnrealBrickImplementation::playTone(unsigned freq, unsigned time)
{
	mD2Model->setBeep(freq, time);
}

void UnrealBrickImplementation::beep(unsigned time)
{
	playTone(1000, time);
}

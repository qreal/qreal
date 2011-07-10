#include "brick.h"
#include "../robotImplementations/brickImplementations/abstractBrickImplementation.h"
#include "../robotCommandConstants.h"

using namespace qReal::interpreters::robots::details;
using namespace robotImplementations::brickImplementations;
using namespace robotParts;

Brick::Brick(robotImplementations::brickImplementations::AbstractBrickImplementation *brickImpl)
	: mBrickImpl(brickImpl)
{
}

Brick::~Brick()
{
}

AbstractBrickImplementation &Brick::brickImpl()
{
	return *mBrickImpl;
}

void Brick::playTone(unsigned freq, unsigned time)
{
	mBrickImpl->playTone(freq, time);
}

void Brick::beep(unsigned time)
{
	mBrickImpl->beep(time);
}

void Brick::setImplementation(robotImplementations::brickImplementations::AbstractBrickImplementation *brickImpl)
{
	mBrickImpl = brickImpl;
}

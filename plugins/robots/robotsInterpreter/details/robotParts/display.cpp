#include "display.h"
#include <QtCore/QDebug>

using namespace qReal::interpreters::robots;
using namespace details;
using namespace robotImplementations::sensorImplementations;
using namespace robotParts;

Display::Display(robotImplementations::sensorImplementations::AbstractDisplayImplementation *displayImpl)
	: mDisplayImpl(displayImpl)
{
}

Display::~Display()
{
}

void Display::read()
{
	mDisplayImpl->read();
}

AbstractDisplayImplementation *Display::displayImpl()
{
	return mDisplayImpl;
}

void Display::setImplementation(robotImplementations::sensorImplementations::AbstractDisplayImplementation *displayImpl)
{
	mDisplayImpl = displayImpl;
}


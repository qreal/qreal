#include <QtCore/QDebug>

#include "display.h"

using namespace qReal::interpreters::robots;
using namespace details;
using namespace robotImplementations::displayImplementations;
using namespace robotParts;

Display::Display(robotImplementations::displayImplementations::AbstractDisplayImplementation *displayImpl)
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

void Display::clearScreen()
{
	mDisplayImpl->clearScreen();
}

AbstractDisplayImplementation *Display::displayImpl()
{
	return mDisplayImpl;
}

void Display::setImplementation(robotImplementations::displayImplementations::AbstractDisplayImplementation *displayImpl)
{
	mDisplayImpl = displayImpl;
}

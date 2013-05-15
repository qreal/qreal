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

void Display::drawPixel(int x, int y)
{
	mDisplayImpl->drawPixel(x, y);
}

void Display::drawLine(int x1, int y1, int x2, int y2)
{
	mDisplayImpl->drawLine(x1, y1, x2, y2);
}

void Display::drawCircle(int x, int y, int radius)
{
	mDisplayImpl->drawCircle(x, y, radius);
}

void Display::printText(int x, int y, QString text)
{
	mDisplayImpl->printText(x, y, text);
}

void Display::drawRect(int x, int y, int width, int height)
{
	mDisplayImpl->drawRect(x, y, width, height);
}

void Display::clearScreen()
{
	mDisplayImpl->clearScreen();
}

void Display::attachToPaintWidget()
{
	mDisplayImpl->attachToPaintWidget();
}

AbstractDisplayImplementation *Display::displayImpl()
{
	return mDisplayImpl;
}

void Display::setImplementation(robotImplementations::displayImplementations::AbstractDisplayImplementation *displayImpl)
{
	mDisplayImpl = displayImpl;
}


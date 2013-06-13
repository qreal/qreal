#include "realDisplayImplementation.h"

using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::displayImplementations;

RealDisplayImplementation::RealDisplayImplementation()
{
}

void RealDisplayImplementation::read()
{
	emit response(false, false, false, false);
}

void RealDisplayImplementation::drawPixel(int x, int y)
{
	Q_UNUSED(x)
	Q_UNUSED(y)
}

void RealDisplayImplementation::drawLine(int x1, int y1, int x2, int y2)
{
	Q_UNUSED(x1)
	Q_UNUSED(y1)
	Q_UNUSED(x2)
	Q_UNUSED(y2)
}

void RealDisplayImplementation::drawCircle(int x, int y, int radius)
{
	Q_UNUSED(x)
	Q_UNUSED(y)
	Q_UNUSED(radius)
}

void RealDisplayImplementation::printText(int x, int y, QString const &text)
{
	Q_UNUSED(x)
	Q_UNUSED(y)
	Q_UNUSED(text)
}

void RealDisplayImplementation::attachToPaintWidget()
{
}

void RealDisplayImplementation::drawRect(int x, int y, int width, int height)
{
	Q_UNUSED(x)
	Q_UNUSED(y)
	Q_UNUSED(width)
	Q_UNUSED(height)
}

void RealDisplayImplementation::clearScreen()
{
}

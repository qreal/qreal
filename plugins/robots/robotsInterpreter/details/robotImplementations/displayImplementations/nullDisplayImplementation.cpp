#include "nullDisplayImplementation.h"

using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::displayImplementations;

NullDisplayImplementation::NullDisplayImplementation()
{
}

void NullDisplayImplementation::read()
{
	emit response(false, false, false, false);
}

void NullDisplayImplementation::drawPixel(int x, int y)
{
	Q_UNUSED(x)
	Q_UNUSED(y)
}

void NullDisplayImplementation::drawLine(int x1, int y1, int x2, int y2)
{
	Q_UNUSED(x1)
	Q_UNUSED(y1)
	Q_UNUSED(x2)
	Q_UNUSED(y2)
}

void NullDisplayImplementation::drawCircle(int x, int y, int radius)
{
	Q_UNUSED(x)
	Q_UNUSED(y)
	Q_UNUSED(radius)
}

void NullDisplayImplementation::printText(int x, int y, QString const &text)
{
	Q_UNUSED(x)
	Q_UNUSED(y)
	Q_UNUSED(text)
}

void NullDisplayImplementation::drawRect(int x, int y, int width, int height)
{
	Q_UNUSED(x)
	Q_UNUSED(y)
	Q_UNUSED(width)
	Q_UNUSED(height)
}

void NullDisplayImplementation::clearScreen()
{
}

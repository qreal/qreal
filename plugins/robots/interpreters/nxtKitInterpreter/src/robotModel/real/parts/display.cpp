#include "display.h"

using namespace nxtKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

Display::Display(const DeviceInfo &info, const PortInfo &port)
	: robotModel::parts::NxtDisplay(info, port)
{
}

void Display::drawPixel(int x, int y)
{
	Q_UNUSED(x)
	Q_UNUSED(y)
}

void Display::drawLine(int x1, int y1, int x2, int y2)
{
	Q_UNUSED(x1)
	Q_UNUSED(y1)
	Q_UNUSED(x2)
	Q_UNUSED(y2)
}

void Display::drawRect(int x, int y, int width, int height)
{
	Q_UNUSED(x)
	Q_UNUSED(y)
	Q_UNUSED(width)
	Q_UNUSED(height)
}

void Display::drawCircle(int x, int y, int radius)
{
	Q_UNUSED(x)
	Q_UNUSED(y)
	Q_UNUSED(radius)
}

void Display::printText(int x, int y, const QString &text)
{
	Q_UNUSED(x)
	Q_UNUSED(y)
	Q_UNUSED(text)
}

void Display::clearScreen()
{
}

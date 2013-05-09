#include "realDisplayImplementation.h"

using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::displayImplementations;

RealDisplayImplementation::RealDisplayImplementation()
{
}

void RealDisplayImplementation::read()
{
	emit response(false,false,false,false);
}

void RealDisplayImplementation::drawPixel(int x, int y)
{
}

void RealDisplayImplementation::drawLine(int x1, int y1, int x2, int y2)
{
}

void RealDisplayImplementation::drawCircle(int x, int y, int radius)
{
}

void RealDisplayImplementation::printText(int x, int y, QString text)
{
}

#include "display.h"

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

Display::Display(DeviceInfo const &info, PortInfo const &port)
	: robotModel::parts::TrikDisplay(info, port)
{
}

void Display::drawSmile(bool sad)
{
	Q_UNUSED(sad)
}

void Display::setBackground(QColor const &color)
{
	Q_UNUSED(color)
}

void Display::clearScreen()
{
}

void Display::printText(int x, int y, QString const &text)
{
	Q_UNUSED(x)
	Q_UNUSED(y)
	Q_UNUSED(text)
}

//void setPainterColor(QColor const &color)
//{
//	Q_UNUSED(color)
//}

//void setPainterWidth(int penWidth)
//{
//	Q_UNUSED(penWidth)
//}

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

void Display::drawEllipse(int x, int y, int width, int height)
{
	Q_UNUSED(x)
	Q_UNUSED(y)
	Q_UNUSED(width)
	Q_UNUSED(height)
}

void Display::drawArc(int x, int y, int width, int height, int startAngle, int spanAngle)
{
	Q_UNUSED(x)
	Q_UNUSED(y)
	Q_UNUSED(width)
	Q_UNUSED(height)
	Q_UNUSED(startAngle)
	Q_UNUSED(spanAngle)
}

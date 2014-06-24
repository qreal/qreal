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

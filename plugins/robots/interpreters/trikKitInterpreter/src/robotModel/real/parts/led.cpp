#include "led.h"

using namespace trikKitInterpreter::robotModel::real::parts;
using namespace interpreterBase::robotModel;

Led::Led(DeviceInfo const &info, PortInfo const &port)
	: robotModel::parts::TrikLed(info, port)
{
}

void Led::setColor(QColor const &color)
{
	Q_UNUSED(color)
}


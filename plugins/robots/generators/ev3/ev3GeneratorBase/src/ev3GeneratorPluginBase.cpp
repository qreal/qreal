#include "ev3GeneratorBase/ev3GeneratorPluginBase.h"

using namespace ev3;

Ev3GeneratorPluginBase::Ev3GeneratorPluginBase()
{
}

QString Ev3GeneratorPluginBase::kitId() const
{
	return "ev3Kit";
}

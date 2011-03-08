#include "abstractMotorImplementation.h"
using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::motorImplementations;

AbstractMotorImplementation::AbstractMotorImplementation(int const port)
	: mPort(static_cast<outputPort::OutputPortEnum>(port))
{
}

void AbstractMotorImplementation::setPort(int const port)
{
	mPort = static_cast<outputPort::OutputPortEnum>(port);
}

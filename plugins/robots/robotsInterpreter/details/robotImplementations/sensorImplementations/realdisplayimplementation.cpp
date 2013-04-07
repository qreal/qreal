#include "realDisplayImplementation.h"

using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::sensorImplementations;

RealDisplayImplementation::RealDisplayImplementation()
{
}

void RealDisplayImplementation::read()
{
	emit response(false,false,false,false);
}


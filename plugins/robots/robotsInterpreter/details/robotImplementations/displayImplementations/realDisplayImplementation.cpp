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


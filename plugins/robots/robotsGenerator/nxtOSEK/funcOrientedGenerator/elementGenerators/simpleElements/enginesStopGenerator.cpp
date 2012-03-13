#include "enginesStopGenerator.h"

using namespace qReal;
using namespace robots::generator;

void EnginesStopGenerator::generateBodyWithoutNextElementCall()
{
	foreach (QString enginePort, portsToEngineNames(mNxtGen->mApi->stringProperty(mElementId, "Ports"))) {
		mNxtGen->mGeneratedStrings.append(SmartLine(
					"nxt_motor_set_speed(" + enginePort + ", 0, 0);",
					mElementId));
	}
}

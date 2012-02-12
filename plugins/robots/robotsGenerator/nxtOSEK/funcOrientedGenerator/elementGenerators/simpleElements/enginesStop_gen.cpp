#include "enginesStop_gen.h"

using namespace qReal;
using namespace robots::generator;

void EnginesStop_gen::generateBodyWithoutNextElementCall()
{
	foreach (QString enginePort, portsToEngineNames(mNxtGen->mApi->stringProperty(mElementId, "Ports"))) {
		mNxtGen->mGeneratedStrings.append(SmartLine(
					"nxt_motor_set_speed(" + enginePort + ", 0, 0);",
					mElementId));
	}
}

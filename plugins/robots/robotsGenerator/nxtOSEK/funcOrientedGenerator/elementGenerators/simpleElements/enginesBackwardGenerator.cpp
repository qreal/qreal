#include "enginesBackwardGenerator.h"

using namespace qReal;
using namespace robots::generator;

void EnginesBackwardGenerator::generateBodyWithoutNextElementCall()
{
	foreach (QString enginePort, portsToEngineNames(mNxtGen->mApi->stringProperty(mElementId, "Ports"))) {
		mNxtGen->mGeneratedStrings.append(SmartLine(
					"nxt_motor_set_speed(" + enginePort + ", "
					+ "-" + mNxtGen->mApi->stringProperty(mElementId, "Power") + ", 0);",
					mElementId));
	}
}

#include "enginesBackward_gen.h"

#include "../../smartLine.h"

using namespace qReal;
using namespace generators;

void EnginesBackward_gen::generateBodyWithoutNextElementCall()
{
	foreach (QString enginePort, portsToEngineNames(mNxtGen->mApi->stringProperty(mElementId, "Ports"))) {
		mNxtGen->mGeneratedStrings.append(SmartLine(
					"nxt_motor_set_speed(" + enginePort + ", "
					+ "-" + mNxtGen->mApi->stringProperty(mElementId, "Power") + ", 0);",
					mElementId));
	}
}

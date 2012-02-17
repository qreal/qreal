#include "enginesForward_gen.h"

using namespace qReal;
using namespace robots::generator;

void EnginesForward_gen::generateBodyWithoutNextElementCall()
{
	QStringList cmds = mNxtGen->mApi->stringProperty(mElementId, "Power").split(";", QString::SkipEmptyParts);
	for (int i = 0; i < cmds.size() - 1; ++i)
		mNxtGen->mGeneratedStrings.append(SmartLine(cmds.at(i) + ";", mElementId));
	foreach (QString enginePort, portsToEngineNames(mNxtGen->mApi->stringProperty(mElementId, "Ports"))) {
		mNxtGen->mGeneratedStrings.append(SmartLine(
					"nxt_motor_set_speed(" + enginePort + ", " + cmds.last() + ", 0);",
					mElementId));
	}
}

#include "enginesForwardGenerator.h"

using namespace qReal;
using namespace robots::generator;

void EnginesForwardGenerator::generateBodyWithoutNextElementCall()
{
	QStringList const cmds = mNxtGen->mApi->stringProperty(mElementId, "Power").split(";", QString::SkipEmptyParts);
	for (int i = 0; i < cmds.size() - 1; ++i)
		mNxtGen->mGeneratedStrings.append(SmartLine(cmds.at(i) + ";", mElementId));
	foreach (QString const &enginePort, portsToEngineNames(mNxtGen->mApi->stringProperty(mElementId, "Ports"))) {
		mNxtGen->mGeneratedStrings.append(SmartLine(
					"nxt_motor_set_speed(" + enginePort + ", " + cmds.last() + ", 0);",
					mElementId));
	}
}

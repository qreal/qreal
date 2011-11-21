#include "nullificationEncoder_gen.h"

#include "../../smartLine.h"

using namespace qReal;
using namespace generators;

void NullificationEncoder_gen::generateBodyWithoutNextElementCall()
{
	QString port = mNxtGen->mApi->stringProperty(mElementId, "Port");
	mNxtGen->mGeneratedStrings.append(SmartLine(
				"nxt_motor_set_count(NXT_PORT_" + port + ", 0);",
				mElementId));
}

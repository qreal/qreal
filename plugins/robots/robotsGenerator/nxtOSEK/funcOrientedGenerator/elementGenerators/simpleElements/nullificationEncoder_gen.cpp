#include "nullificationEncoder_gen.h"

using namespace qReal;
using namespace robots::generator;

void NullificationEncoder_gen::generateBodyWithoutNextElementCall()
{
	QString port = mNxtGen->mApi->stringProperty(mElementId, "Port");
	mNxtGen->mGeneratedStrings.append(SmartLine(
				"nxt_motor_set_count(NXT_PORT_" + port + ", 0);",
				mElementId));
}

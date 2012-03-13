#include "nullificationEncoderGenerator.h"

using namespace qReal;
using namespace robots::generator;

void NullificationEncoderGenerator::generateBodyWithoutNextElementCall()
{
	QString port = mNxtGen->mApi->stringProperty(mElementId, "Port");
	mNxtGen->mGeneratedStrings.append(SmartLine(
				"nxt_motor_set_count(NXT_PORT_" + port + ", 0);",
				mElementId));
}

#include "waitForEncoderGenerator.h"

using namespace qReal;
using namespace robots::generator;

void WaitForEncoderGenerator::generateBodyWithoutNextElementCall()
{
	QString port = mNxtGen->mApi->stringProperty(mElementId, "Port");
	QString tachoLimit = mNxtGen->mApi->stringProperty(mElementId, "TachoLimit");
	mNxtGen->mGeneratedStrings.append(SmartLine(
				"while (nxt_motor_get_count(NXT_PORT_" + port + ") < " + tachoLimit + ")",
				mElementId));
	mNxtGen->mGeneratedStrings.append(SmartLine("{", mElementId));
	mNxtGen->mGeneratedStrings.append(SmartLine("}", mElementId));
}

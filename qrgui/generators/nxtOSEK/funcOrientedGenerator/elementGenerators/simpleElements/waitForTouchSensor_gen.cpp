#include "waitForTouchSensor_gen.h"

using namespace qReal;
using namespace generators;
using namespace nxtOSEK;

void WaitForTouchSensor_gen::generateBodyWithoutNextElementCall()
{
	int port = mNxtGen->mApi->stringProperty(mElementId, "Port").toInt();

	mNxtGen->mGeneratedStrings.append(SmartLine(
				"while (!ecrobot_get_touch_sensor(NXT_PORT_S" + QString::number(port) + "))",
				mElementId));
	mNxtGen->mGeneratedStrings.append(SmartLine("{", mElementId));
	mNxtGen->mGeneratedStrings.append(SmartLine("}", mElementId));
}

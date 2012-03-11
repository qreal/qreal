#include "waitForColorIntensity_gen.h"

using namespace qReal;
using namespace robots::generator;

void WaitForColorIntensity_gen::generateBodyWithoutNextElementCall()
{
	int port = mNxtGen->mApi->stringProperty(mElementId, "Port").toInt();
	QString intensity = mNxtGen->mApi->stringProperty(mElementId,  "Intensity");
	QString inequalitySign = mNxtGen->mApi->stringProperty(mElementId, "Sign");

	QString condition = inequalitySign + " " + intensity;

	mNxtGen->mGeneratedStrings.append(SmartLine(
				"while (!(ecrobot_get_nxtcolorsensor_light(NXT_PORT_S" + QString::number(port)
				+ ") " + condition + "))",
				mElementId));
	mNxtGen->mGeneratedStrings.append(SmartLine("{", mElementId));
	mNxtGen->mGeneratedStrings.append(SmartLine("}", mElementId));
}

#include "waitForColorIntensityGenerator.h"

using namespace qReal;
using namespace robots::generator;

void WaitForColorIntensityGenerator::generateBodyWithoutNextElementCall()
{
	int const port = mNxtGen->mApi->stringProperty(mElementId, "Port").toInt();
	QString const intensity = mNxtGen->mApi->stringProperty(mElementId,  "Intensity");
	QString const inequalitySign = mNxtGen->mApi->stringProperty(mElementId, "Sign");

	QString const condition = inequalitySign + " " + intensity;

	mNxtGen->mGeneratedStrings.append(SmartLine(
				"while (!(ecrobot_get_nxtcolorsensor_light(NXT_PORT_S" + QString::number(port)
				+ ") " + condition + "))",
				mElementId));
	mNxtGen->mGeneratedStrings.append(SmartLine("{", mElementId));
	mNxtGen->mGeneratedStrings.append(SmartLine("}", mElementId));
}

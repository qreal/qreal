#include "waitForSonarDistance_gen.h"

using namespace qReal;
using namespace robots::generator;

void WaitForSonarDistance_gen::generateBodyWithoutNextElementCall()
{
	int port = mNxtGen->mApi->stringProperty(mElementId, "Port").toInt();
	QString distance = mNxtGen->mApi->stringProperty(mElementId, "Distance");
	QString inequalitySign = transformSign(QString(mNxtGen->mApi->stringProperty(mElementId, "Sign").toUtf8()));
	QString condition = inequalitySign + " " + distance;

	mNxtGen->mGeneratedStrings.append(SmartLine(
				"while (!(ecrobot_get_sonar_sensor(NXT_PORT_S" + QString::number(port) + ") " + condition + "))",
				mElementId));
	mNxtGen->mGeneratedStrings.append(SmartLine("{", mElementId));
	mNxtGen->mGeneratedStrings.append(SmartLine("}", mElementId));
}

#include "waitForColor_gen.h"

using namespace qReal;
using namespace robots::generator;

void WaitForColor_gen::generateBodyWithoutNextElementCall()
{
	int port = mNxtGen->mApi->stringProperty(mElementId, "Port").toInt();
	QByteArray colorStr = mNxtGen->mApi->stringProperty(mElementId, "Color").toUtf8();

	QString colorNxtType;

	if (colorStr == "Красный") {
		colorNxtType = "NXT_COLOR_RED";

	} else if (colorStr == "Зелёный") {
		colorNxtType = "NXT_COLOR_GREEN";

	} else if (colorStr == "Синий") {
		colorNxtType = "NXT_COLOR_BLUE";

	} else if (colorStr == "Чёрный") {
		colorNxtType = "NXT_COLOR_BLACK";

	} else if (colorStr == "Жёлтый") {
		colorNxtType = "NXT_COLOR_YELLOW";

	} else if (colorStr == "Белый") {
		colorNxtType = "NXT_COLOR_WHITE";
	}

	if (!colorNxtType.isEmpty()) {
		mNxtGen->mGeneratedStrings.append(SmartLine(
					"while (ecrobot_get_nxtcolorsensor_id(NXT_PORT_S" + QString::number(port)
					+ ") != " + colorNxtType + ")",
					mElementId));
		mNxtGen->mGeneratedStrings.append(SmartLine("{", mElementId));
		mNxtGen->mGeneratedStrings.append(SmartLine("}", mElementId));
	}
}

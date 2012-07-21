#include "waitForColorBlockGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"

using namespace robots::generator;

WaitForColorBlockGenerator::WaitForColorBlockGenerator()
{
}

QList<SmartLine> WaitForColorBlockGenerator::convertElememtIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;
	int const port = nxtGen->api()->stringProperty(logicElementId, "Port").toInt();
	QByteArray const colorStr = nxtGen->api()->stringProperty(logicElementId, "Color").toUtf8();

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
		result.append(SmartLine("while (ecrobot_get_nxtcolorsensor_id(NXT_PORT_S" + QString::number(port)
				+ ") != " + colorNxtType + ")", elementId));
		result.append(SmartLine("{", elementId));
		result.append(SmartLine("}", elementId));
	}

	return result;
}

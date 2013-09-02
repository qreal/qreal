#include "waitForColorBlockGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"
#include "../listsmartline.h"

using namespace qReal::robots::generators::simple;

WaitForColorBlockGenerator::WaitForColorBlockGenerator()
{
}

void WaitForColorBlockGenerator::addInitAndTerminateCode(NxtOSEKRobotGenerator *nxtGen,
		QString const &port, QString const &colorNxtType, qReal::Id const &elementId)
{
	QString const partInitCode = "ecrobot_init_nxtcolorsensor(" + port;
	QString const initCode = "ecrobot_init_nxtcolorsensor(" + port + "," + colorNxtType + ");";

	if (!ListSmartLine::isContainsPart(nxtGen->initCode(), partInitCode)) {
		QString const terminateCode = "ecrobot_init_nxtcolorsensor(" + port + "," + colorNxtType + ");";
		nxtGen->initCode().append(SmartLine_old(initCode, elementId));
		nxtGen->terminateCode().append(SmartLine_old(terminateCode, elementId));
	}
}

QList<SmartLine_old> WaitForColorBlockGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const &elementId, qReal::Id const &logicElementId)
{
	QList<SmartLine_old> result;
	int const port = nxtGen->api()->stringProperty(logicElementId, "Port").toInt();
	QByteArray const colorStr = nxtGen->api()->stringProperty(logicElementId, "Color").toUtf8();

	QString colorNxtType;

	if (colorStr == QString::fromUtf8("Красный")) {
		colorNxtType = "NXT_COLOR_RED";
	} else if (colorStr == QString::fromUtf8("Зелёный")) {
		colorNxtType = "NXT_COLOR_GREEN";
	} else if (colorStr == QString::fromUtf8("Синий")) {
		colorNxtType = "NXT_COLOR_BLUE";
	} else if (colorStr == QString::fromUtf8("Чёрный")) {
		colorNxtType = "NXT_COLOR_BLACK";
	} else if (colorStr == QString::fromUtf8("Жёлтый")) {
		colorNxtType = "NXT_COLOR_YELLOW";
	} else if (colorStr == QString::fromUtf8("Белый")) {
		colorNxtType = "NXT_COLOR_WHITE";
	}

	if (!colorNxtType.isEmpty()) {
		QString portStr = QString::number(port);
		result.append(SmartLine_old("while (ecrobot_get_nxtcolorsensor_id(NXT_PORT_S" + portStr
				+ ") != " + colorNxtType + ") {", elementId));
		result.append(SmartLine_old("}", elementId));
		addInitAndTerminateCode(nxtGen, portStr, colorNxtType, elementId);
	}

	return result;
}

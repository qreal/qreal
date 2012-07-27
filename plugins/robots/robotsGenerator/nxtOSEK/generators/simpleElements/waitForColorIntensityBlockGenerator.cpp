#include "waitForColorIntensityBlockGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"
#include "../listsmartline.h"

using namespace robots::generator;

WaitForColorIntensityBlockGenerator::WaitForColorIntensityBlockGenerator()
{
}

void WaitForColorIntensityBlockGenerator::addInitAndTerminateCode(NxtOSEKRobotGenerator *nxtGen
		, QString const &port, qReal::Id const elementId)
{
	QString const partInitCode = "ecrobot_init_nxtcolorsensor(" + port;
	if (!ListSmartLine::isContainsPart(nxtGen->initCode(), partInitCode)) {
		// i don't no, mb NXT_LIGHTSENSOR_NONE, NXT_COLORSENSOR_DEACTIVATE
		QString const initCode = "ecrobot_init_nxtcolorsensor(" + port + "," + "NXT_COLORSENSOR);";
		QString const terminateCode = "ecrobot_term_nxtcolorsensor(" + port + ");";
		nxtGen->initCode().append(SmartLine(initCode, elementId));
		nxtGen->terminateCode().append(SmartLine(terminateCode, elementId));
	}
}

QList<SmartLine> WaitForColorIntensityBlockGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;
	QString portStr = nxtGen->api()->stringProperty(logicElementId, "Port");
	QString const intensity = nxtGen->api()->stringProperty(logicElementId,  "Intensity");
	QString const inequalitySign = transformSign(QString(nxtGen->api()->stringProperty(logicElementId
			, "Sign").toUtf8()));

	QString const condition = inequalitySign + " " + intensity;

	result.append(SmartLine("while (!(ecrobot_get_nxtcolorsensor_light(NXT_PORT_S" + portStr
			+ ") " + condition + "))", elementId));
	result.append(SmartLine("{", elementId));
	result.append(SmartLine("}", elementId));

	addInitAndTerminateCode(nxtGen, "NXT_PORT_S" + portStr, elementId);

	return result;
}

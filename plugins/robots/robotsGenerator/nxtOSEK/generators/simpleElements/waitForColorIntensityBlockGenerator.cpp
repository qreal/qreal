#include "waitForColorIntensityBlockGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"

using namespace robots::generator;

WaitForColorIntensityBlockGenerator::WaitForColorIntensityBlockGenerator()
{
}

QList<SmartLine> WaitForColorIntensityBlockGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;
	int const port = nxtGen->api()->stringProperty(logicElementId, "Port").toInt();
	QString const intensity = nxtGen->api()->stringProperty(logicElementId,  "Intensity");
	QString const inequalitySign = transformSign(QString(nxtGen->api()->stringProperty(logicElementId
			, "Sign").toUtf8()));

	QString const condition = inequalitySign + " " + intensity;

	result.append(SmartLine("while (!(ecrobot_get_nxtcolorsensor_light(NXT_PORT_S" + QString::number(port)
			+ ") " + condition + "))", elementId));
	result.append(SmartLine("{", elementId));
	result.append(SmartLine("}", elementId));

	return result;
}

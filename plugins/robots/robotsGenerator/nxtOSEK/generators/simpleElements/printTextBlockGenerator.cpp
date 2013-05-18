#include "printTextBlockGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"
#include "../listsmartline.h"

using namespace robots::generator;

PrintTextBlockGenerator::PrintTextBlockGenerator()
{
}

QList<SmartLine> PrintTextBlockGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;

	int const x = nxtGen->api()->stringProperty(logicElementId, "XCoordinateText").toInt();
	int const y = nxtGen->api()->stringProperty(logicElementId, "YCoordinateText").toInt();
	QString text = nxtGen->api()->stringProperty(logicElementId, "PrintText").toUtf8();

	result.append(SmartLine("display_goto_xy(" + QString::number(x) + "," + QString::number(y) + ");", elementId));
	result.append(SmartLine("display_string(\"" + text + "\");", elementId));
	result.append(SmartLine("display_update();", elementId));

	return result;
}

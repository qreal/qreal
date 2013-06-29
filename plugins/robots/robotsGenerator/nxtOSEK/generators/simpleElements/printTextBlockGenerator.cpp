#include "printTextBlockGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"
#include "../listsmartline.h"

using namespace robots::generator;

PrintTextBlockGenerator::PrintTextBlockGenerator()
{
}

QList<SmartLine> PrintTextBlockGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const &elementId, qReal::Id const &logicElementId)
{
	QList<SmartLine> result;

	QString const x = nxtGen->intExpression(logicElementId, "XCoordinateText");
	QString const y = nxtGen->intExpression(logicElementId, "YCoordinateText");
	QString text = nxtGen->api()->stringProperty(logicElementId, "PrintText").toUtf8();

	result.append(SmartLine("display_goto_xy(" + x + ", " + y + ");", elementId));
	result.append(SmartLine("display_string(\"" + text.replace("\"", "\\\"") + "\");", elementId));
	result.append(SmartLine("display_update();", elementId));

	return result;
}

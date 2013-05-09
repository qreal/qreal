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
	QString const text = nxtGen->api()->stringProperty(logicElementId, "Text");

	return result;
}

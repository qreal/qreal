#include "drawRectBlockGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"
#include "../listsmartline.h"

using namespace robots::generator;

DrawRectBlockGenerator::DrawRectBlockGenerator()
{
}

QList<SmartLine> DrawRectBlockGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;

	int const x = nxtGen->api()->stringProperty(logicElementId, "XCoordinateRect").toInt();
	int const y = nxtGen->api()->stringProperty(logicElementId, "YCoordinateRect").toInt();
	int const width = nxtGen->api()->stringProperty(logicElementId, "WidthRect").toInt();
	int const height = nxtGen->api()->stringProperty(logicElementId, "HeightRect").toInt();

	return result;
}

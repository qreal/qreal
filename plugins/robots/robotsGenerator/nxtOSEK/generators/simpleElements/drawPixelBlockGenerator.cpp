#include "drawPixelBlockGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"
#include "../listsmartline.h"

using namespace robots::generator;

DrawPixelBlockGenerator::DrawPixelBlockGenerator()
{
}

QList<SmartLine> DrawPixelBlockGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;

	int const x = nxtGen->api()->stringProperty(logicElementId, "XCoordinatePix").toInt();
	int const y = nxtGen->api()->stringProperty(logicElementId, "YCoordinatePix").toInt();
	return result;
}

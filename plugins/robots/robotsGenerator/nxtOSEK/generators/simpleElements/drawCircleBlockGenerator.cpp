#include "drawCircleBlockGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"
#include "../listsmartline.h"

using namespace robots::generator;

DrawCircleBlockGenerator::DrawCircleBlockGenerator()
{
}

QList<SmartLine> DrawCircleBlockGenerator::convertElementIntoDirectCommand(NxtOSEKRobotGenerator *nxtGen
		, qReal::Id const elementId, qReal::Id const logicElementId)
{
	QList<SmartLine> result;

	int const x = nxtGen->api()->stringProperty(logicElementId, "XCoordinateCircle").toInt();
	int const y = nxtGen->api()->stringProperty(logicElementId, "YCoordinateCircle").toInt();
	int const radius = nxtGen->api()->stringProperty(logicElementId, "CircleRadius").toInt();

	return result;
}

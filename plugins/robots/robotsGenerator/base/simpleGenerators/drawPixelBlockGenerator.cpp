#include "drawPixelBlockGenerator.h"

using namespace robots::generator;

DrawPixelBlockGenerator::DrawPixelBlockGenerator()
{
}

void DrawPixelBlockGenerator::drawBmp(NxtOSEKRobotGenerator *nxtGen, qReal::Id const logicElementId, QPainter *p)
{
	int const x = nxtGen->api()->stringProperty(logicElementId, "XCoordinatePix").toInt();
	int const y = nxtGen->api()->stringProperty(logicElementId, "YCoordinatePix").toInt();

	p->drawPoint(x, y);
}

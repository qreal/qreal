#include "drawRectBlockGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"
#include "../listsmartline.h"

using namespace robots::generator;

DrawRectBlockGenerator::DrawRectBlockGenerator()
{
}

void DrawRectBlockGenerator::drawBmp(NxtOSEKRobotGenerator *nxtGen, qReal::Id const logicElementId, QPainter *p)
{
	int const x = nxtGen->api()->stringProperty(logicElementId, "XCoordinateRect").toInt();
	int const y = nxtGen->api()->stringProperty(logicElementId, "YCoordinateRect").toInt();
	int const width = nxtGen->api()->stringProperty(logicElementId, "WidthRect").toInt();
	int const height = nxtGen->api()->stringProperty(logicElementId, "HeightRect").toInt();

	p->drawRect(x, y, width, height);
}

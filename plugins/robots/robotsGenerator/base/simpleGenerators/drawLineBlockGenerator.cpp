#include "drawLineBlockGenerator.h"

using namespace qReal::robots::generators::simple;

DrawLineBlockGenerator::DrawLineBlockGenerator(LogicalModelAssistInterface const &model
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: DrawBlockGenerator(model, customizer, id, parent)
{
}

//void DrawLineBlockGenerator::drawBmp(NxtOSEKRobotGenerator *nxtGen, qReal::Id const logicElementId, QPainter *p)
//{
//	int const x1 = nxtGen->api()->stringProperty(logicElementId, "X1CoordinateLine").toInt();
//	int const y1 = nxtGen->api()->stringProperty(logicElementId, "Y1CoordinateLine").toInt();
//	int const x2 = nxtGen->api()->stringProperty(logicElementId, "X2CoordinateLine").toInt();
//	int const y2 = nxtGen->api()->stringProperty(logicElementId, "Y2CoordinateLine").toInt();

//	p->drawLine(x1, y1, x2, y2);
//}

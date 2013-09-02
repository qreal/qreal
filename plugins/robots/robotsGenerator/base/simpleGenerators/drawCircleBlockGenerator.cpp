#include "drawCircleBlockGenerator.h"

using namespace qReal::robots::generators::simple;

DrawCircleBlockGenerator::DrawCircleBlockGenerator(LogicalModelAssistInterface const &model
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: DrawBlockGenerator(model, customizer, id, parent)
{
}

//void DrawCircleBlockGenerator::drawBmp(NxtOSEKRobotGenerator *nxtGen, qReal::Id const logicElementId, QPainter *p)
//{
//	int const x = nxtGen->api()->stringProperty(logicElementId, "XCoordinateCircle").toInt();
//	int const y = nxtGen->api()->stringProperty(logicElementId, "YCoordinateCircle").toInt();
//	int const radius = nxtGen->api()->stringProperty(logicElementId, "CircleRadius").toInt();

//	p->drawEllipse(x - radius, y - radius, radius * 2, radius * 2);
//}

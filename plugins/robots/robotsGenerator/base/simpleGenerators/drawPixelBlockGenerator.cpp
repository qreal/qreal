#include "drawPixelBlockGenerator.h"

using namespace qReal::robots::generators::simple;

DrawPixelBlockGenerator::DrawPixelBlockGenerator(LogicalModelAssistInterface const &model
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: DrawBlockGenerator(model, customizer, id, parent)
{
}

//void DrawPixelBlockGenerator::drawBmp(NxtOSEKRobotGenerator *nxtGen, qReal::Id const logicElementId, QPainter *p)
//{
//	int const x = nxtGen->api()->stringProperty(logicElementId, "XCoordinatePix").toInt();
//	int const y = nxtGen->api()->stringProperty(logicElementId, "YCoordinatePix").toInt();

//	p->drawPoint(x, y);
//}

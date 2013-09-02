#include "drawRectBlockGenerator.h"

using namespace qReal::robots::generators::simple;

DrawRectBlockGenerator::DrawRectBlockGenerator(LogicalModelAssistInterface const &model
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: DrawBlockGenerator(model, customizer, id, parent)
{
}

//void DrawRectBlockGenerator::drawBmp(NxtOSEKRobotGenerator *nxtGen, qReal::Id const logicElementId, QPainter *p)
//{
//	int const x = nxtGen->api()->stringProperty(logicElementId, "XCoordinateRect").toInt();
//	int const y = nxtGen->api()->stringProperty(logicElementId, "YCoordinateRect").toInt();
//	int const width = nxtGen->api()->stringProperty(logicElementId, "WidthRect").toInt();
//	int const height = nxtGen->api()->stringProperty(logicElementId, "HeightRect").toInt();

//	p->drawRect(x, y, width, height);
//}

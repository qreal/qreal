#include "drawPixelBlockGenerator.h"

using namespace qReal::robots::generators::simple;

DrawPixelBlockGenerator::DrawPixelBlockGenerator(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer
		, Id const &id
		, QObject *parent)
	: DrawBlockGenerator(repo, customizer, id, parent)
{
}

//void DrawPixelBlockGenerator::drawBmp(NxtOSEKRobotGenerator *nxtGen, qReal::Id const logicElementId, QPainter *p)
//{
//	int const x = nxtGen->api()->stringProperty(logicElementId, "XCoordinatePix").toInt();
//	int const y = nxtGen->api()->stringProperty(logicElementId, "YCoordinatePix").toInt();

//	p->drawPoint(x, y);
//}

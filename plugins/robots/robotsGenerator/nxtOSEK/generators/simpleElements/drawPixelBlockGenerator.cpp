#include "drawPixelBlockGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"
#include "../listsmartline.h"

using namespace robots::generator;

DrawPixelBlockGenerator::DrawPixelBlockGenerator()
{
}

void DrawPixelBlockGenerator::generateBmpFile(NxtOSEKRobotGenerator *nxtGen, QString name, qReal::Id const logicElementId)
{
	int x = nxtGen->api()->stringProperty(logicElementId, "XCoordinatePix").toInt();
	int y = nxtGen->api()->stringProperty(logicElementId, "YCoordinatePix").toInt();

	QImage *im = new QImage(100, 64, QImage::Format_Mono);
	QPainter *p = new QPainter(im);
	p->eraseRect(0,0,100,64);
	p->drawPoint(x, y);

	im->save("nxt-tools/example0/" + name + ".bmp", "BMP", -1);
}

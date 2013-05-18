#include "drawLineBlockGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"
#include "../listsmartline.h"

using namespace robots::generator;

DrawLineBlockGenerator::DrawLineBlockGenerator()
{
}

void DrawLineBlockGenerator::generateBmpFile(NxtOSEKRobotGenerator *nxtGen, QString name, qReal::Id const logicElementId)
{
	int x1 = nxtGen->api()->stringProperty(logicElementId, "X1CoordinateLine").toInt();
	int y1 = nxtGen->api()->stringProperty(logicElementId, "Y1CoordinateLine").toInt();
	int x2 = nxtGen->api()->stringProperty(logicElementId, "X2CoordinateLine").toInt();
	int y2 = nxtGen->api()->stringProperty(logicElementId, "Y2CoordinateLine").toInt();

	QImage *im = new QImage(100, 64, QImage::Format_Mono);
	QPainter *p = new QPainter(im);
	p->eraseRect(0,0,100,64);
	p->drawLine(x1,y1,x2,y2);

	im->save("nxt-tools/example0/" + name + ".bmp", "BMP", -1);
}

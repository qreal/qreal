#include "drawCircleBlockGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"
#include "../listsmartline.h"

using namespace robots::generator;

DrawCircleBlockGenerator::DrawCircleBlockGenerator()
{
}

void DrawCircleBlockGenerator::generateBmpFile(NxtOSEKRobotGenerator *nxtGen, QString name, qReal::Id const logicElementId)
{
	int const x = nxtGen->api()->stringProperty(logicElementId, "XCoordinateCircle").toInt();
	int const y = nxtGen->api()->stringProperty(logicElementId, "YCoordinateCircle").toInt();
	int const radius = nxtGen->api()->stringProperty(logicElementId, "CircleRadius").toInt();

	QImage *im = new QImage(100, 64, QImage::Format_Mono);
	QPainter *p = new QPainter(im);
	p->eraseRect(0,0,100,64);
	p->drawEllipse(x-radius, y-radius, radius*2, radius*2);
	im->save("nxt-tools/example0/" + name + ".bmp", "BMP", -1);
}

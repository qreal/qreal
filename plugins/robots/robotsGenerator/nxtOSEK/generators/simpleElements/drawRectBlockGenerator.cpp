#include "drawRectBlockGenerator.h"
#include "../../nxtOSEKRobotGenerator.h"
#include "../listsmartline.h"

using namespace robots::generator;

DrawRectBlockGenerator::DrawRectBlockGenerator()
{
}

void DrawRectBlockGenerator::generateBmpFile(NxtOSEKRobotGenerator *nxtGen, QString name, qReal::Id const logicElementId)
{
	int x = nxtGen->api()->stringProperty(logicElementId, "XCoordinateRect").toInt();
	int y = nxtGen->api()->stringProperty(logicElementId, "YCoordinateRect").toInt();
	int width = nxtGen->api()->stringProperty(logicElementId, "WidthRect").toInt();
	int height = nxtGen->api()->stringProperty(logicElementId, "HeightRect").toInt();

	QImage *im = new QImage(100, 64, QImage::Format_Mono);
	QPainter *p = new QPainter(im);
	p->eraseRect(0,0,100,64);
	p->drawRect(x, y, width, height);
	im->save("nxt-tools/example0/" + name + ".bmp", "BMP", -1);
}

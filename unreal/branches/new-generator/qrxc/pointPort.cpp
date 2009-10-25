#include "pointPort.h"
#include "outFile.h"

bool PointPort::init(QDomElement const &element, int width, int height)
{
	mX = ((qreal) element.attribute("x").toInt()) / width;
	mY = ((qreal) element.attribute("y").toInt()) / height;
	return true;
}

void PointPort::generateCode(OutFile &out)
{
	out() << QString("\t\t\tmPointPorts << QPointF(%1, %2);\n").arg(mX).arg(mY);
}
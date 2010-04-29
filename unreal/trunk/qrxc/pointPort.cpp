#include "pointPort.h"
#include "../utils/outFile.h"

using namespace utils;

bool PointPort::init(QDomElement const &element, int width, int height)
{
	mX = static_cast<qreal>(element.attribute("x").toInt()) / width;
	mY = static_cast<qreal>(element.attribute("y").toInt()) / height;
	return true;
}

void PointPort::generateCode(OutFile &out)
{
	out() << QString("\t\t\tpointPorts << QPointF(%1, %2);\n").arg(mX).arg(mY);
}

Port* PointPort::clone() const
{
	PointPort *result = new PointPort();
	result->mX = mX;
	result->mY = mY;
	return result;
}

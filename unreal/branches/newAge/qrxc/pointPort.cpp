#include "pointPort.h"
#include "../utils/outFile.h"

using namespace utils;

bool PointPort::init(QDomElement const &element, int width, int height)
{
	mX = initCoordinate(element.attribute("x"), width);
	mY = initCoordinate(element.attribute("y"), height);
	return true;
}

void PointPort::generateCode(OutFile &out)
{
	out() << QString("\t\t\tpointPorts << QPointF(%1, %2);\n").arg(mX.value()).arg(mY.value());
}

Port* PointPort::clone() const
{
	PointPort *result = new PointPort();
	result->mX = mX;
	result->mY = mY;
	return result;
}

#include "pointPort.h"

#include <qrutils/outFile.h>

using namespace utils;

bool PointPort::init(const QDomElement &element, int width, int height)
{
	mX = initCoordinate(element.attribute("x"), width);
	mY = initCoordinate(element.attribute("y"), height);
	mInitWidth = width;
	mInitHeight = height;
	mType = element.attribute("type", "NonTyped");
	return true;
}

void PointPort::generateCode(OutFile &out, const QStringList &portTypes)
{
	if (!portTypes.contains(mType)) {
		mType = "NonTyped";
	}

	out() << QString("\t\t\tports << portFactory.createPort(QPointF(%1, %2), %3, %4, %5, %6, new %7());\n")
			.arg(mX.value()).arg(mY.value())
			.arg(mX.isScalable() ? "true" : "false").arg(mY.isScalable() ? "true" : "false")
			.arg(mInitWidth).arg(mInitHeight).arg(mType);
}

Port* PointPort::clone() const
{
	PointPort *result = new PointPort();
	result->mX = mX;
	result->mY = mY;
	result->mType = mType;
	result->mInitWidth = mInitWidth;
	result->mInitHeight = mInitHeight;
	return result;
}

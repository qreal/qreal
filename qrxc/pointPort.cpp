#include "pointPort.h"
#include "../qrutils/outFile.h"

using namespace utils;

bool PointPort::init(QDomElement const &element, int width, int height)
{
	mX = initCoordinate(element.attribute("x"), width);
	mY = initCoordinate(element.attribute("y"), height);
	mInitWidth = width;
	mInitHeight = height;
	mType = element.attribute("type", "NonTyped");
	return true;
}

void PointPort::generateCode(OutFile &out, QStringList const &portTypes)
{
	out() <<"\t\t\t{\n"
		<< "\t\t\t\tStatPoint pt;\n"
		<< QString("\t\t\t\tpt.point = QPointF(%1, %2);\n").arg(mX.value()).arg(mY.value())
		<< "\t\t\t\tpt.prop_x = "
		<< ((mX.isScalable()) ? "true;\n" : "false;\n")
		<< "\t\t\t\tpt.prop_y = "
		<< ((mY.isScalable()) ? "true; \n" : "false; \n")
		<< QString("\t\t\t\tpt.initWidth = %1;\n").arg(mInitWidth)
		<< QString("\t\t\t\tpt.initHeight = %1;\n").arg(mInitHeight);

	if (!portTypes.contains(mType)) {
		mType = "NonTyped";
	}

	out() << QString("\t\t\t\tpt.impl = QSharedPointer<PortImpl>(new %1());\n").arg(mType)
		<< "\t\t\t\tpointPorts << pt;\n"
		<< "\t\t\t};\n";
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

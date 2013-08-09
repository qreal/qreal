#include "linePort.h"
#include "../qrutils/outFile.h"

using namespace utils;

bool LinePort::init(QDomElement const &element, int width, int height)
{
	QDomElement portStartElement = element.firstChildElement("start");
	QDomElement portEndElement = element.firstChildElement("end");

	mStartX = initCoordinate(portStartElement.attribute("startx"), width);
	mStartY = initCoordinate(portStartElement.attribute("starty"), height);
	mEndX = initCoordinate(portEndElement.attribute("endx"), width);
	mEndY = initCoordinate(portEndElement.attribute("endy"), height);
	mInitWidth = width;
	mInitHeight = height;
	return true;
}

void LinePort::generateCode(OutFile &out)
{
	out() <<"\t\t\t{\n"
		<< "\t\t\t\tqReal::StatLine ln;\n"
		<< QString("\t\t\t\tln.line = QLineF(%1, %2, %3, %4);\n")
		.arg(mStartX.value()).arg(mStartY.value()).arg(mEndX.value()).arg(mEndY.value())
		<< "\t\t\t\tln.prop_x1 = "
		<< ((mStartX.isScalable()) ? "true;\n" : "false;\n")
		<< "\t\t\t\tln.prop_y1 = "
		<< ((mStartY.isScalable()) ? "true; \n" : "false; \n")
		<< "\t\t\t\tln.prop_x2 = "
		<< ((mEndX.isScalable()) ? "true; \n" : "false; \n")
		<< "\t\t\t\tln.prop_y2 = "
		<< ((mEndY.isScalable()) ? "true; \n" : "false; \n")
		<< QString("\t\t\t\tln.initWidth = %1;\n").arg(mInitWidth)
		<< QString("\t\t\t\tln.initHeight = %1;\n").arg(mInitHeight)
		<< "\t\t\t\tlinePorts << ln;\n"
		<< "\t\t\t};\n";
}

Port *LinePort::clone() const
{
	LinePort *result = new LinePort();
	result->mEndX = mEndX;
	result->mEndY = mEndY;
	result->mStartX = mStartX;
	result->mStartY = mStartY;
	result->mInitWidth = mInitWidth;
	result->mInitHeight = mInitHeight;
	return result;
}


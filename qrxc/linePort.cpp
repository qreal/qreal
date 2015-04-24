/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "linePort.h"

#include <qrutils/outFile.h>

using namespace utils;

bool LinePort::init(const QDomElement &element, int width, int height)
{
	QDomElement portStartElement = element.firstChildElement("start");
	QDomElement portEndElement = element.firstChildElement("end");

	mStartX = initCoordinate(portStartElement.attribute("startx"), width);
	mStartY = initCoordinate(portStartElement.attribute("starty"), height);
	mEndX = initCoordinate(portEndElement.attribute("endx"), width);
	mEndY = initCoordinate(portEndElement.attribute("endy"), height);
	mType = element.attribute("type", "NonTyped");
	mInitWidth = width;
	mInitHeight = height;

	return true;
}

void LinePort::generateCode(OutFile &out, const QStringList &portTypes)
{
	QString line = QString("QLineF(%1, %2, %3, %4)").arg(mStartX.value()).arg(mStartY.value())
			.arg(mEndX.value()).arg(mEndY.value());

	if (!portTypes.contains(mType)) {
		mType = "NonTyped";
	}

	out() << QString("\t\t\tports << portFactory.createPort(%1, %2, %3, %4, %5, %6, %7, new %8());\n").arg(line)
			.arg(mStartX.isScalable() ? "true" : "false").arg(mStartY.isScalable() ? "true" : "false")
			.arg(mEndX.isScalable() ? "true" : "false").arg(mEndY.isScalable() ? "true" : "false")
			.arg(mInitWidth).arg(mInitHeight).arg(mType);
}

Port *LinePort::clone() const
{
	LinePort *result = new LinePort();
	result->mEndX = mEndX;
	result->mEndY = mEndY;
	result->mStartX = mStartX;
	result->mStartY = mStartY;
	result->mType = mType;
	result->mInitWidth = mInitWidth;
	result->mInitHeight = mInitHeight;
	return result;
}


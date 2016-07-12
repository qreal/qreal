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

void PointPort::generateCode(OutFile &out)
{
	if (mType.isNull() || mType.isEmpty()) {
		mType = "NonTyped";
	}

	out() << QString("qReal::PointPortInfo(QPointF(%1, %2), %3, %4, %5, %6, \"%7\")").arg(
			QString::number(mX.value())
			, QString::number(mY.value())
			, mX.isScalable() ? "true" : "false"
			, mY.isScalable() ? "true" : "false"
			, QString::number(mInitWidth)
			, QString::number(mInitHeight)
			, mType);
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

/* Copyright 2016 Polina Tarasova
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

#include "circularPort.h"

#include <qrutils/outFile.h>

using namespace utils;

bool CircularPort::init(const QDomElement &element, int width, int height)
{
	mX = initCoordinate(element.attribute("x"), width);
	mY = initCoordinate(element.attribute("y"), height);
	mR = initCoordinate(element.attribute("r"), qMax(width, height));
	mInitWidth = width;
	mInitHeight = height;
	mType = element.attribute("type", "NonTyped");

	return true;
}

void CircularPort::generateCode(OutFile &out, const QStringList &portTypes)
{
	if (!portTypes.contains(mType)) {
		mType = "NonTyped";
	}

	out() << QString("qReal::CircularPortInfo(QPointF(%1, %2), %3, %4, %5, %6, %7, \"%8\")")
			.arg(mX.value()).arg(mY.value()).arg(mR.value())
			.arg(mX.isScalable() ? "true" : "false").arg(mY.isScalable() ? "true" : "false")
			.arg(mInitWidth).arg(mInitHeight).arg(mType);
}

Port *CircularPort::clone() const
{
	CircularPort *result = new CircularPort();
	result->mX = mX;
	result->mY = mY;
	result->mR = mR;
	result->mType = mType;
	result->mInitWidth = mInitWidth;
	result->mInitHeight = mInitHeight;
	return result;
}


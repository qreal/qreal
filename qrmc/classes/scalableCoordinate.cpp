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

#include "scalableCoordinate.h"

using namespace qrmc;

ScalableCoordinate::ScalableCoordinate()
	: mValue(0), mWidth(0), mIsScalable(false)
{}

ScalableCoordinate::ScalableCoordinate(qreal value, int width, bool isScalable)
	: mValue(value), mWidth(width), mIsScalable(isScalable)
{}

qreal ScalableCoordinate::value() const
{
	return mValue;
}

bool ScalableCoordinate::isScalable() const
{
	return mIsScalable;
}

QString ScalableCoordinate::toString(bool isScaled) const
{
	qreal value = isScaled ? mValue * mWidth : mValue;
	QString result = QString::number(value);
	// uncomment this when it's supported
//	result += mIsScalable ? "a" : "%";
	return result;
}

QString ScalableCoordinate::getScalability() const
{
	if (mIsScalable)
		return "true";

	return "false";
}

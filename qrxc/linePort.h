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

#pragma once

#include <QtXml/QDomElement>

#include <qrutils/scalableCoordinate.h>

#include "port.h"

namespace utils {
	class OutFile;
}

class LinePort : public Port
{
public:
	virtual bool init(const QDomElement &element, int width, int height);
	virtual void generateCode(utils::OutFile &out, const QStringList &portTypes);
	virtual Port* clone() const;

private:
	utils::ScalableCoordinate mStartX;
	utils::ScalableCoordinate mStartY;
	utils::ScalableCoordinate mEndX;
	utils::ScalableCoordinate mEndY;
	int mInitWidth;
	int mInitHeight;
};

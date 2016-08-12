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

#pragma once

#include <QtXml/QDomElement>

#include <qrutils/scalableCoordinate.h>

#include "port.h"

namespace utils {
	class OutFile;
}

/// Represents circular port.
class CircularPort : public Port
{
public:
	bool init(const QDomElement &element, int width, int height) override;
	void generateCode(utils::OutFile &out) override;
	Port* clone() const override;

private:
	/// X coordinate of a center of port.
	utils::ScalableCoordinate mX;

	/// Y coordinate of a center of port.
	utils::ScalableCoordinate mY;

	/// Radius of a port.
	utils::ScalableCoordinate mR;

	/// Width of an initial bounding rect of a port.
	int mInitWidth;

	/// Height of an initial bounding rect of a port.
	int mInitHeight;
};

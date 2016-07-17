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

#include "device.h"
#include "kitBase/kitBaseDeclSpec.h"

namespace kitBase {
namespace robotModel {
namespace robotParts {

/// Output device representing abstract robot display.
class ROBOTS_KIT_BASE_EXPORT Display : public Device
{
	Q_OBJECT
	Q_CLASSINFO("name", "display")
	Q_CLASSINFO("friendlyName", tr("Display"))
	Q_CLASSINFO("direction", "output")

public:
	/// Constructor, takes device type info and predefined port on which display is configured.
	Display(const DeviceInfo &info, const PortInfo &port);

	/// Prints the given text on the robot`s screen in the given coordinates.
	/// The collision behaviour depends on concrete kit: the text may be replaced
	/// (like in TRIK) or intersect (like in NXT).
	virtual void printText(int x, int y, const QString &text) = 0;

	/// Clears everything on a display without redrawing it.
	virtual void clearScreen() = 0;

	/// Draw point on the widget without redrawing display.
	/// @param x - x coordinate.
	/// @param y - y coordinate.
	virtual void drawPixel(int x, int y) = 0;

	/// Draw line on the widget without redrawing display.
	/// @param x1 - first point's x coordinate.
	/// @param y1 - first point's y coordinate.
	/// @param x1 - second point's x coordinate.
	/// @param y1 - second point's y coordinate.
	virtual void drawLine(int x1, int y1, int x2, int y2) = 0;

	/// Draw rect on the widget without redrawing display.
	/// @param x - x coordinate.
	/// @param y - y coordinate.
	/// @param width - rect's width.
	/// @param height - rect's height.
	virtual void drawRect(int x, int y, int width, int height) = 0;

	/// Redraws all the contents on the screen.
	/// @warning This opeartion is pretty slow, so it shouldn`t be called without need.
	virtual void redraw() = 0;
};

}
}
}

#pragma once

#include "device.h"
#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {
namespace robotParts {

/// Output device representing abstract robot display.
class ROBOTS_INTERPRETER_BASE_EXPORT Display : public Device
{
	Q_OBJECT
	Q_CLASSINFO("name", "display")
	Q_CLASSINFO("friendlyName", tr("Display"))
	Q_CLASSINFO("direction", "output")

public:
	/// Constructor, takes device type info and predefined port on which display is configured.
	Display(DeviceInfo const &info, PortInfo const &port);

	/// Prints the given text on the robot`s screen in the given coordinates.
	/// The collision behaviour depends on concrete kit: the text may be replaced
	/// (like in TRIK) or intersect (like in NXT).
	virtual void printText(int x, int y, QString const &text) = 0;

	/// Clears everything on a display.
	virtual void clearScreen() = 0;

	/// Draw point on the widget.
	/// @param x - x coordinate.
	/// @param y - y coordinate.
	virtual void drawPixel(int x, int y) = 0;

	/// Draw line on the widget.
	/// @param x1 - first point's x coordinate.
	/// @param y1 - first point's y coordinate.
	/// @param x1 - second point's x coordinate.
	/// @param y1 - second point's y coordinate.
	virtual void drawLine(int x1, int y1, int x2, int y2) = 0;

	/// Draw rect on the widget.
	/// @param x - x coordinate.
	/// @param y - y coordinate.
	/// @param width - rect's width.
	/// @param height - rect's height.
	virtual void drawRect(int x, int y, int width, int height) = 0;
};

}
}
}

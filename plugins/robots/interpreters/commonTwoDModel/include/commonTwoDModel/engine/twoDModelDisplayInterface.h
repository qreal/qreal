#pragma once

#include <qrutils/graphicsUtils/painterInterface.h>

namespace twoDModel {
namespace engine {

/// An interface that each 2D model display emulator must implement.
class TwoDModelDisplayInterface
{
public:
	/// Accepts the given painter and draws on the display area with it.
	virtual void setPainter(graphicsUtils::PainterInterface *painter) = 0;

	/// Returns true if the given button is pressed in 2D model.
	/// @todo Supposes polling, which is not very good for buttons.
	virtual bool buttonIsDown(const QString &buttonPort) const = 0;

	/// Updates picture on the display. Must be called each time when somethig
	/// is beeing changed on the display.
	virtual void repaintDisplay() = 0;

	/// Returns the drawable area`s width in px.
	virtual int displayWidth() const = 0;

	/// Returns the drawable area`s height in px.
	virtual int displayHeight() const = 0;

	/// Clears the display erasing everything on it.
	virtual void clear() const = 0;
};

}
}

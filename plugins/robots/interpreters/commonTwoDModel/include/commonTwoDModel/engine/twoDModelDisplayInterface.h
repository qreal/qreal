#pragma once

#include <qrutils/graphicsUtils/painterInterface.h>

namespace twoDModel {
namespace engine {

class TwoDModelDisplayInterface
{
public:
	virtual void setPainter(graphicsUtils::PainterInterface *painter) = 0;

	/// @todo Supposes polling, which is not very good for buttons.
	virtual bool buttonIsDown(QString const &buttonId) const = 0;

	virtual void repaintDisplay() = 0;
	virtual int displayWidth() const = 0;
	virtual int displayHeight() const = 0;

	virtual void clear() const = 0;
};

}
}

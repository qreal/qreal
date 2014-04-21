#pragma once

#include <qrutils/graphicsUtils/painterInterface.h>

namespace twoDModel {
namespace engine {

class TwoDModelDisplayInterface
{
public:
	/// @todo wt*???
	virtual void setPainter(graphicsUtils::PainterInterface *p) = 0;

	/// @todo Not good for anything except NXT
	virtual bool leftButtonIsDown() const = 0;
	virtual bool rightButtonIsDown() const = 0;
	virtual bool centralButtonIsDown() const = 0;
	virtual bool bottomButtonIsDown() const = 0;

	virtual void repaintDisplay() = 0;
	virtual int displayWidth() const = 0;
	virtual int displayHeight() const = 0;
};

}
}

#pragma once

#include <interpreterBase/robotModel/robotParts/display.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace parts {

class TrikDisplay : public interpreterBase::robotModel::robotParts::Display
{
	Q_OBJECT

public:
	TrikDisplay(interpreterBase::robotModel::DeviceInfo const &info
			, interpreterBase::robotModel::PortInfo const &port);

	virtual void drawSmile(bool sad) = 0;

	/// @todo: color enum here?
	virtual void setBackground(QColor const &color) = 0;

	virtual void drawEllipse(int x, int y, int width, int height) = 0;
	virtual void drawArc(int x, int y, int width, int height, int startAngle, int spanAngle) = 0;
};

}
}
}

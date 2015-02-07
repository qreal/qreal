#pragma once

#include <interpreterBase/robotModel/robotParts/display.h>

namespace trikKitInterpreter {
namespace robotModel {
namespace parts {

class TrikDisplay : public interpreterBase::robotModel::robotParts::Display
{
	Q_OBJECT

public:
	TrikDisplay(const interpreterBase::robotModel::DeviceInfo &info
			, const interpreterBase::robotModel::PortInfo &port);

	/// Show happy or sad smile.
	/// @param sad - if sad is true, then show sad smile.
	virtual void drawSmile(bool sad) = 0;

	/// @todo: color enum here?
	virtual void setBackground(const QColor &color) = 0;

	/// Draw ellipse.
	/// @param x - x coordinate.
	/// @param y - y coordinate.
	/// @param width - width of ellipse.
	/// @param height - height of ellipse.
	virtual void drawEllipse(int x, int y, int width, int height) = 0;

	/// Draw arc on the widget.
	/// @param x - x coordinate.
	/// @param y - y coordinate.
	/// @param width - width rect forming an arc.
	/// @param height - height rect forming an arc.
	/// @param startAngle - start angle.
	/// @param spanAngle - end angle.
	virtual void drawArc(int x, int y, int width, int height, int startAngle, int spanAngle) = 0;

	/// Set painter width.
	virtual void setPainterWidth(int penWidth) = 0;

	/// Set painter color.
	/// @param color - color of painter.
	virtual void setPainterColor(const QColor &color) = 0;
};

}
}
}

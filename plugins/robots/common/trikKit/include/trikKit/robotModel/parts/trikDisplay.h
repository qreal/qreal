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

#include <kitBase/robotModel/robotParts/display.h>

namespace trik {
namespace robotModel {
namespace parts {

class TrikDisplay : public kitBase::robotModel::robotParts::Display
{
	Q_OBJECT

public:
	TrikDisplay(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port);

	/// Show happy or sad smile.
	/// @param sad - if sad is true, then show sad smile.
	virtual void drawSmile(bool sad) = 0;

	/// @todo: color enum here?
	virtual void setBackground(const QColor &color) = 0;

	/// Draw ellipse without redrawing display.
	/// @param x - x coordinate.
	/// @param y - y coordinate.
	/// @param width - width of ellipse.
	/// @param height - height of ellipse.
	virtual void drawEllipse(int x, int y, int width, int height) = 0;

	/// Draw arc on the widget without redrawing it.
	/// @param x - x coordinate.
	/// @param y - y coordinate.
	/// @param width - width rect forming an arc.
	/// @param height - height rect forming an arc.
	/// @param startAngle - start angle.
	/// @param spanAngle - end angle.
	virtual void drawArc(int x, int y, int width, int height, int startAngle, int spanAngle) = 0;

	/// Set painter pen width. The \a penWidth will be used for painting of all next shapes till the next
	/// call of setPainterColor.
	virtual void setPainterWidth(int penWidth) = 0;

	/// Sets painter`s pen \a color. The \a color will be used for painting of all next shapes till the next
	/// call of setPainterColor.
	/// @param color - color of painter.
	virtual void setPainterColor(const QColor &color) = 0;
};

}
}
}

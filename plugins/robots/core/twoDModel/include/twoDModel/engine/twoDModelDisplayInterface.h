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

#include <qrutils/widgets/painterInterface.h>

namespace twoDModel {
namespace engine {

/// An interface that each 2D model display emulator must implement.
class TwoDModelDisplayInterface
{
public:
	virtual ~TwoDModelDisplayInterface() {}

	/// Accepts the given painter and draws on the display area with it.
	virtual void setPainter(qReal::ui::PainterInterface *painter) = 0;

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

	/// Must implement the behavior of display of simulated robot model in the end of interpretation.
	virtual void reset() = 0;
};

}
}

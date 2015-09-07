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

#include <kitBase/robotModel/robotParts/device.h>

#include "twoDModel/twoDModelDeclSpec.h"

namespace twoDModel {

namespace engine {
class TwoDModelEngineInterface;
}

namespace robotModel {
namespace parts {

/// A marker that draws robot`s trace in 2D model.
/// Configuration is perfomed immediately.
class TWO_D_MODEL_EXPORT Marker : public kitBase::robotModel::robotParts::Device
{
	Q_OBJECT
	Q_CLASSINFO("direction", "output")
	Q_CLASSINFO("name", "marker")
	Q_CLASSINFO("friendlyName", tr("Marker"))
	Q_PROPERTY(bool isDown READ isDown WRITE setDown NOTIFY isDownChanged)

public:
	Marker(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port
			, engine::TwoDModelEngineInterface &engine);

	/// Moves the marker of the 2D model robot down to the floor.
	/// The robot will draw its trace on the floor after that.
	/// If the marker of another color is already drawing at the moment it will be replaced.
	void down(const QColor &color);

	/// Lifts the marker of the 2D model robot up.
	/// The robot stops drawing its trace on the floor after that.
	void up();

	/// Returns true if marker is currently active or false if not.
	bool isDown() const;

	/// Calls down() with black color if \a isDown is true or up() otherwise.
	void setDown(bool isDown);

signals:
	/// Emitted with 'true' parameter when marker activated and 'false' when it was lifted.
	void isDownChanged(bool isDown);

	/// Emitted when marker color has changed.
	void colorChanged(const QColor &color);

private:
	engine::TwoDModelEngineInterface &mEngine;
};

}
}
}

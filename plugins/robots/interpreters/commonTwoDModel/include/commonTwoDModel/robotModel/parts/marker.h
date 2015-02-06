#pragma once

#include <interpreterBase/robotModel/robotParts/device.h>

#include "commonTwoDModel/commonTwoDModelDeclSpec.h"

namespace twoDModel {

namespace engine {
class TwoDModelEngineInterface;
}

namespace robotModel {
namespace parts {

/// A marker that draws robot`s trace in 2D model.
/// Configuration is perfomed immediately.
class COMMON_TWO_D_MODEL_EXPORT Marker : public interpreterBase::robotModel::robotParts::Device
{
	Q_OBJECT
	Q_CLASSINFO("direction", "output")
	Q_CLASSINFO("name", "marker")
	Q_CLASSINFO("friendlyName", tr("Marker"))

public:
	Marker(const interpreterBase::robotModel::DeviceInfo &info
			, const interpreterBase::robotModel::PortInfo &port
			, engine::TwoDModelEngineInterface &engine);

	/// Moves the marker of the 2D model robot down to the floor.
	/// The robot will draw its trace on the floor after that.
	/// If the marker of another color is already drawing at the moment it will be replaced.
	void down(QColor const &color);

	/// Lifts the marker of the 2D model robot up.
	/// The robot stops drawing its trace on the floor after that.
	void up();

private:
	engine::TwoDModelEngineInterface &mEngine;
};

}
}
}

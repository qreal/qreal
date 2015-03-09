#pragma once

#include "scalarSensor.h"
#include "interpreterBase/interpreterBaseDeclSpec.h"

namespace interpreterBase {
namespace robotModel {
namespace robotParts {

/// Base class for a sensor that can detect color intensity or differentiate colors. Color sensors are modeled as in NXT
/// for historical reasons and because 2D model currently supports NXT-style color sensors. It may not be appropriate
/// for other kits, but is needed to be able to conveniently work with NXT and 2D model. TRIK does not have color sensor
/// yet, so it is fine by now.
class ROBOTS_INTERPRETER_BASE_EXPORT ColorSensor : public ScalarSensor
{
	Q_OBJECT
	Q_CLASSINFO("name", "color")
	Q_CLASSINFO("friendlyName", tr("Color sensor"))

public:
	/// Constructor, takes device type info and port on which this sensor is configured.
	ColorSensor(const DeviceInfo &info, const PortInfo &port);
};

}
}
}

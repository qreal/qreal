#pragma once

#include <kitBase/robotModel/robotParts/vectorSensor.h>

namespace trik {
namespace robotModel {
namespace parts {

/// Device representing TRIK camera color detector. Returns color from middle of a camera field of view as three
/// integers: red, green and blue components of a color.
class TrikColorSensor : public kitBase::robotModel::robotParts::VectorSensor
{
	Q_OBJECT
	Q_CLASSINFO("name", "trikColorSensor")
	Q_CLASSINFO("friendlyName", tr("Color Sensor"))

public:
	TrikColorSensor(const kitBase::robotModel::DeviceInfo &info
			, const kitBase::robotModel::PortInfo &port);

	/// Turns camera on and prepares a sensor.
	virtual void init() = 0;

	/// @todo Support real interface of color sensor, now read() from AbstractSensor does not match the interface
	///       of trikControl::ColorSensor::read.
};

}
}
}

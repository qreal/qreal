#pragma once

#include <QtCore/QString>

namespace robotsInterpreterCore {
namespace robotModel {

/// Unique sensor identifier. Contains name of a kit and name of a sensor.
class SensorId
{
public:
	/// Constructor.
	/// @param kitName - name of the kit to which sensor belongs to.
	/// @param sensorName - name of the sensor,
	SensorId(QString const &kitName, QString const &sensorName);
	QString const &kitName() const;
	QString const &sensorName() const;

private:
	QString mKitName;
	QString mSensorName;
};

}
}

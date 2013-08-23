#pragma once

#include <QtCore/QString>

namespace robotsInterpreterCore {
namespace robotModel {

class SensorId
{
public:
	SensorId(QString const &kitName, QString const &sensorName);
	QString const &kitName() const;
	QString const &sensorName() const;

private:
	QString mKitName;
	QString mSensorName;
};

}
}

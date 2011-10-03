#pragma once

#include <QtCore/QObject>
#include <QtCore/QVector>

#include "sensorImplementations/abstractSensorImplementation.h"

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {
namespace robotImplementations {

class SensorsConfigurer : public QObject
{
	Q_OBJECT

public:
	SensorsConfigurer();
	~SensorsConfigurer();
	void configureSensor(sensorImplementations::AbstractSensorImplementation *sensor, inputPort::InputPortEnum const &port);
	void lockConfiguring();
	void unlockConfiguring();

	sensorImplementations::AbstractSensorImplementation * sensor(inputPort::InputPortEnum const &port) const;

signals:
	void allSensorsConfigured();
	void sensorConfigured(sensorImplementations::AbstractSensorImplementation const * const sensor);

private slots:
	void sensorConfiguredSlot();

private:
	QVector<sensorImplementations::AbstractSensorImplementation *> mConfiguredSensors;  // Has ownership.
	QVector<sensorImplementations::AbstractSensorImplementation *> mPendingSensors;  // Has ownership.
	bool mLocked;
	int mSensorsToConfigure;

	void reconfigureSensors();
};

}
}
}
}
}

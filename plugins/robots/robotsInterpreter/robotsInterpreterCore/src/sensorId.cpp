#include <robotsInterpreterCore/sensorId.h>

using namespace robotsInterpreterCore;

SensorId::SensorId(QString const &kitName, QString const &sensorName)
		: mKitName(kitName), mSensorName(sensorName)
{
}

QString const &SensorId::kitName() const
{
	return mKitName;
}

QString const &SensorId::sensorName() const
{
	return mSensorName;
}

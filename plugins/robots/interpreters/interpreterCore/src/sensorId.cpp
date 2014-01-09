#include <interpreterBase/robotModel/sensorId.h>

using namespace interpreterBase::robotModel;

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

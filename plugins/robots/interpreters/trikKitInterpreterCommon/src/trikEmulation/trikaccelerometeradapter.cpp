#include <trikKitInterpreterCommon/trikEmulation/trikaccelerometeradapter.h>


TrikAccelerometerAdapter::TrikAccelerometerAdapter(kitBase::robotModel::robotParts::AccelerometerSensor *acc)
	: mAccelerometer(acc)
{
}

QVector<int> TrikAccelerometerAdapter::read() const
{
	QMetaObject::invokeMethod(mAccelerometer, "read");
	return mAccelerometer->lastData(); // hopefully the same format as on real brick
}

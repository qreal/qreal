#include "trikKitInterpreterCommon/trikEmulation/triksensoremu.h"

using namespace trik;

TrikSensorEmu::TrikSensorEmu(kitBase::robotModel::robotParts::ScalarSensor *sensor) : mSensor(sensor)
{
//	connect(sensor, &kitBase::robotModel::robotParts::ScalarSensor::newData)
}

int TrikSensorEmu::read()
{
	//mSensor->read(); test crash fixes
	QMetaObject::invokeMethod(mSensor, "read");
	return mSensor->lastData(); // not best, race conditions?
}

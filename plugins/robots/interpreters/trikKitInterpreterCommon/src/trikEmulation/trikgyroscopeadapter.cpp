#include <trikKitInterpreterCommon/trikEmulation/trikgyroscopeadapter.h>

TrikGyroscopeAdapter::TrikGyroscopeAdapter(kitBase::robotModel::robotParts::GyroscopeSensor *g)
	: mGyroscope(g)
{
}

QVector<int> TrikGyroscopeAdapter::read() const
{
	QMetaObject::invokeMethod(mGyroscope, "read");
	return mGyroscope->lastData(); // hopefully the same format as on real brick
}

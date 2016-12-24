#include <trikKitInterpreterCommon/trikEmulation/triklinesensoradapter.h>

TrikLineSensorAdapter::TrikLineSensorAdapter(trik::robotModel::parts::TrikLineSensor *sensor)
	: mLineSensor(sensor)
{
}

trikControl::DeviceInterface::Status TrikLineSensorAdapter::status() const
{
	return Status::ready;
}

void TrikLineSensorAdapter::init(bool showOnDisplay)
{
	Q_UNUSED(showOnDisplay);
	mLineSensor->init();
}

void TrikLineSensorAdapter::detect()
{
	mLineSensor->detectLine(); // hopefully the same
}

QVector<int> TrikLineSensorAdapter::read()
{
	QMetaObject::invokeMethod(mLineSensor, "read");
	return mLineSensor->lastData(); // hopefully the same format
}

void TrikLineSensorAdapter::stop()
{
}

QVector<int> TrikLineSensorAdapter::getDetectParameters() const
{
	return mLineSensor->lastData(); // i guess?
}

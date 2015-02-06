#include "commonTwoDModel/robotModel/parts/encoderSensor.h"

#include "commonTwoDModel/engine/twoDModelEngineInterface.h"

using namespace twoDModel::robotModel::parts;
using namespace interpreterBase::robotModel;

EncoderSensor::EncoderSensor(const DeviceInfo &info
		, const PortInfo &port
		, engine::TwoDModelEngineInterface &engine)
	: robotParts::EncoderSensor(info, port)
	, mEngine(engine)
{
}

void EncoderSensor::read()
{
	emit newData(mEngine.readEncoder(port()));
}

void EncoderSensor::nullify()
{
	mEngine.resetEncoder(port());
}

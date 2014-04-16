#include "commonTwoDModel/robotModel/parts/encoderSensor.h"

#include "commonTwoDModel/engine/twoDModelEngineInterface.h"

using namespace twoDModel::robotModel::parts;
using namespace interpreterBase::robotModel;

EncoderSensor::EncoderSensor(DeviceInfo const &info
		, PortInfo const &port
		, engine::TwoDModelEngineInterface &engine)
	: robotParts::EncoderSensor(info, port)
	, mEngine(engine)
{
}

void EncoderSensor::read()
{
	/// @todo Implement.
}

void EncoderSensor::nullificate()
{
	/// @todo Implement.
}

void EncoderSensor::doConfiguration()
{
}

#include "twoDInfraredSensor.h"

#include <commonTwoDModel/engine/twoDModelEngineInterface.h>

using namespace trikKitInterpreter::robotModel::twoD::parts;

TwoDInfraredSensor::TwoDInfraredSensor(interpreterBase::robotModel::DeviceInfo const &info
		, interpreterBase::robotModel::PortInfo const &port
		, twoDModel::engine::TwoDModelEngineInterface &engine)
	: twoDModel::robotModel::parts::RangeSensor(info, port, engine)
{
}

void TwoDInfraredSensor::read()
{
	int const distanceValue = mEngine.readSonarSensor(port());
	/// @todo: Know how real IR sensor works and model something similar to it.
	emit newData(255 - distanceValue);
}

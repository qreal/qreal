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
	int const sonarDistanceValue = mEngine.readSonarSensor(port());
	int const linearDistance = 100 - sonarDistanceValue * 100 / 255;
	/// @todo: Know how real IR sensor works and model something similar to it.
	emit newData(linearDistance);
}

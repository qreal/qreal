#include "twoDInfraredSensor.h"

#include <commonTwoDModel/engine/twoDModelEngineInterface.h>

using namespace trikKitInterpreter::robotModel::twoD::parts;

/// The maximal distance that returned by IR sensor; when the object is closer
/// IR sensor starts to return less values non-lineary.
int const maxDistance = 93;

TwoDInfraredSensor::TwoDInfraredSensor(interpreterBase::robotModel::DeviceInfo const &info
		, interpreterBase::robotModel::PortInfo const &port
		, twoDModel::engine::TwoDModelEngineInterface &engine)
	: twoDModel::robotModel::parts::RangeSensor(info, port, engine)
{
}

void TwoDInfraredSensor::read()
{
	int const sonarDistanceValue = mEngine.readSonarSensor(port());
	int const linearDistance = 100 - (sonarDistanceValue < 100 ? sonarDistanceValue : 100);
	int const spoiledDistance = linearDistance <= maxDistance
			? linearDistance
			// On small distances IR sensor starts to work non-lineary
			: 100 - (linearDistance - maxDistance) * 10;
	emit newData(spoiledDistance);
}

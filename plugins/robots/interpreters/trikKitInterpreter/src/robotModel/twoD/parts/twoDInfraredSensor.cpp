#include "twoDInfraredSensor.h"

#include <twoDModel/engine/twoDModelEngineInterface.h>

using namespace trik::robotModel::twoD::parts;

/// The maximal distance that returned by IR sensor; when the object is closer
/// IR sensor starts to return less values non-lineary.
const int maxDistance = 93;

TwoDInfraredSensor::TwoDInfraredSensor(const kitBase::robotModel::DeviceInfo &info
		, const kitBase::robotModel::PortInfo &port
		, twoDModel::engine::TwoDModelEngineInterface &engine)
	: twoDModel::robotModel::parts::RangeSensor(info, port, engine)
{
}

void TwoDInfraredSensor::read()
{
	const int sonarDistanceValue = mEngine.readSonarSensor(port());
	const int linearDistance = 100 - (sonarDistanceValue < 100 ? sonarDistanceValue : 100);
	const int spoiledDistance = linearDistance <= maxDistance
			? linearDistance
			// On small distances IR sensor starts to work non-lineary
			: 100 - (linearDistance - maxDistance) * 10;
	emit newData(spoiledDistance);
}

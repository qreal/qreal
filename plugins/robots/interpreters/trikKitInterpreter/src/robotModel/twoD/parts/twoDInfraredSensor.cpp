/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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

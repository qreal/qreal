/* Copyright 2016-2017 CyberTech Labs Ltd.
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

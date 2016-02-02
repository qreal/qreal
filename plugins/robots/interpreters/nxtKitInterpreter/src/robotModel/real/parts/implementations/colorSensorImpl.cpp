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

#include "colorSensorImpl.h"

using namespace nxt::robotModel::real::parts;
using namespace kitBase::robotModel;

const int maxColorValue = 1023;

ColorSensorImpl::ColorSensorImpl(const PortInfo &port
		, utils::robotCommunication::RobotCommunicator &robotCommunicator
		, enums::lowLevelSensorType::SensorTypeEnum lowLevelType)
	: mImplementation(robotCommunicator, port, lowLevelType, enums::sensorMode::RAWMODE)
	, mLowLevelType(lowLevelType)
{
	connect(&mImplementation, &NxtInputDevice::sensorSpecificProcessResponse
			, this, &ColorSensorImpl::sensorSpecificProcessResponse);
	connect(&mImplementation, &NxtInputDevice::configured, this, &ColorSensorImpl::configurationCompleted);
}

void ColorSensorImpl::read()
{
	if (!mImplementation.isConfigured()) {
		// If sensor is not configured, report failure and return immediately.
		// It is not an error, it shall be possible to reconfigure sensor "on the fly",
		// but when it is reconfiguring it shall not be available.
		emit failure();
		return;
	}

	if (mImplementation.state() == NxtInputDevice::pending) {
		return;
	}

	mImplementation.setState(NxtInputDevice::pending);

	QByteArray command(5, 0);
	command[0] = 0x03;  //command length
	command[1] = 0x00;
	command[2] = enums::telegramType::directCommandResponseRequired;
	command[3] = enums::commandCode::GETINPUTVALUES;
	command[4] = mImplementation.lowLevelPort();
	mImplementation.send(command, 18);
}

void ColorSensorImpl::doConfiguration()
{
	mImplementation.configure();
}

void ColorSensorImpl::sensorSpecificProcessResponse(const QByteArray &reading)
{
	if (reading.isEmpty()) {
		/// @todo: log trace error?
	} else {
		mImplementation.setState(NxtInputDevice::idle);
		if (mLowLevelType == enums::lowLevelSensorType::COLORFULL) {
			emit newData(0xff & reading[14]);  // Scaled value, used in ColorFull mode.
		} else {
			emit newData(((0xff & reading[10]) | ((0xff & reading[11]) << 8)) * 100 / maxColorValue);
		}
	}
}

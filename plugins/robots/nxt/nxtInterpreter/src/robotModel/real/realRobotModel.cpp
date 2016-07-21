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

#include "realRobotModel.h"

#include <qrkernel/settingsManager.h>

#include "parts/display.h"
#include "parts/speaker.h"
#include "parts/button.h"
#include "parts/motor.h"
#include "parts/encoderSensor.h"
#include "parts/touchSensor.h"
#include "parts/sonarSensor.h"
#include "parts/lightSensor.h"
#include "parts/colorSensorFull.h"
#include "parts/colorSensorRed.h"
#include "parts/colorSensorGreen.h"
#include "parts/colorSensorBlue.h"
#include "parts/colorSensorPassive.h"
#include "parts/soundSensor.h"
#include "parts/gyroscopeSensor.h"

using namespace nxt::robotModel::real;
using namespace utils::robotCommunication;
using namespace kitBase::robotModel;

RealRobotModel::RealRobotModel(const QString &kitId, const QString &robotId
		, utils::robotCommunication::RobotCommunicationThreadInterface *communicationThread)
	: NxtRobotModelBase(kitId, robotId)
	, mRobotCommunicator(new RobotCommunicator(this))
{
	connect(mRobotCommunicator, &RobotCommunicator::connected, this, &RealRobotModel::connected);
	connect(mRobotCommunicator, &RobotCommunicator::disconnected, this, &RealRobotModel::disconnected);
	connect(mRobotCommunicator, &RobotCommunicator::errorOccured, this, &RealRobotModel::errorOccured);
	mRobotCommunicator->setRobotCommunicationThreadObject(communicationThread);
}

bool RealRobotModel::needsConnection() const
{
	return true;
}

void RealRobotModel::connectToRobot()
{
	mRobotCommunicator->connect();
}

void RealRobotModel::disconnectFromRobot()
{
	mRobotCommunicator->disconnect();
}

void RealRobotModel::checkConnection()
{
	mRobotCommunicator->checkConsistency();
}

robotParts::Device *RealRobotModel::createDevice(const PortInfo &port, const DeviceInfo &deviceInfo)
{
	if (deviceInfo.isA(displayInfo())) {
		return new parts::Display(displayInfo(), port);
	}

	if (deviceInfo.isA(speakerInfo())) {
		return new parts::Speaker(speakerInfo(), port, *mRobotCommunicator);
	}

	if (deviceInfo.isA(buttonInfo())) {
		return new parts::Button(buttonInfo(), port);
	}

	if (deviceInfo.isA(motorInfo())) {
		return new parts::Motor(motorInfo(), port, *mRobotCommunicator);
	}

	if (deviceInfo.isA(encoderInfo())) {
		return new parts::EncoderSensor(encoderInfo(), port, *mRobotCommunicator);
	}

	if (deviceInfo.isA(touchSensorInfo())) {
		return new parts::TouchSensor(touchSensorInfo(), port, *mRobotCommunicator);
	}

	if (deviceInfo.isA(sonarSensorInfo())) {
		return new parts::SonarSensor(sonarSensorInfo(), port, *mRobotCommunicator);
	}

	if (deviceInfo.isA(lightSensorInfo())) {
		return new parts::LightSensor(lightSensorInfo(), port, *mRobotCommunicator);
	}

	if (deviceInfo.isA(colorFullSensorInfo())) {
		return new parts::ColorSensorFull(colorFullSensorInfo(), port, *mRobotCommunicator);
	}

	if (deviceInfo.isA(colorRedSensorInfo())) {
		return new parts::ColorSensorRed(colorRedSensorInfo(), port, *mRobotCommunicator);
	}

	if (deviceInfo.isA(colorGreenSensorInfo())) {
		return new parts::ColorSensorGreen(colorGreenSensorInfo(), port, *mRobotCommunicator);
	}

	if (deviceInfo.isA(colorBlueSensorInfo())) {
		return new parts::ColorSensorBlue(colorBlueSensorInfo(), port, *mRobotCommunicator);
	}

	if (deviceInfo.isA(colorPassiveSensorInfo())) {
		return new parts::ColorSensorPassive(colorPassiveSensorInfo(), port, *mRobotCommunicator);
	}

	if (deviceInfo.isA(soundSensorInfo())) {
		return new parts::SoundSensor(soundSensorInfo(), port, *mRobotCommunicator);
	}

	if (deviceInfo.isA(gyroscopeSensorInfo())) {
		return new parts::GyroscopeSensor(gyroscopeSensorInfo(), port, *mRobotCommunicator);
	}

	/// @todo: Support accelerometer
	//if (deviceInfo.isA(accelerometerSensorInfo())) {
	//	return new parts::TouchSensor(accelerometerSensorInfo(), port, *mRobotCommunicator);
	//}

	return NxtRobotModelBase::createDevice(port, deviceInfo);
}

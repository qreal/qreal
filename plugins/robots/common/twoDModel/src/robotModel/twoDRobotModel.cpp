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

#include "twoDModel/robotModel/twoDRobotModel.h"

#include <qrkernel/exception/exception.h>

#include "twoDModel/robotModel/parts/button.h"
#include "twoDModel/robotModel/parts/colorSensorFull.h"
#include "twoDModel/robotModel/parts/colorSensorPassive.h"
#include "twoDModel/robotModel/parts/colorSensorRed.h"
#include "twoDModel/robotModel/parts/colorSensorGreen.h"
#include "twoDModel/robotModel/parts/colorSensorBlue.h"
#include "twoDModel/robotModel/parts/display.h"
#include "twoDModel/robotModel/parts/encoderSensor.h"
#include "twoDModel/robotModel/parts/lightSensor.h"
#include "twoDModel/robotModel/parts/motor.h"
#include "twoDModel/robotModel/parts/rangeSensor.h"
#include "twoDModel/robotModel/parts/touchSensor.h"
#include "twoDModel/robotModel/parts/gyroscope.h"
#include "twoDModel/robotModel/parts/accelerometer.h"
#include "twoDModel/robotModel/parts/marker.h"

#include "twoDModel/engine/twoDModelEngineInterface.h"

using namespace twoDModel::robotModel;
using namespace kitBase::robotModel;

TwoDRobotModel::TwoDRobotModel(const RobotModelInterface &realModel)
	: CommonRobotModel(realModel.kitId(), realModel.robotId())
	, mRealModel(&realModel)
	, mEngine(nullptr)
{
	for (const PortInfo &port : realModel.availablePorts()) {
		if (!port.name().toLower().contains("gamepad")) {
			addAllowedConnection(port, realModel.allowedDevices(port));
		}
	}

	addAllowedConnection(PortInfo("MarkerPort", output), { markerInfo() });
}

TwoDRobotModel::TwoDRobotModel(const QString &robotId)
	: CommonRobotModel("", robotId)
	, mRealModel(nullptr)
	, mEngine(nullptr)
{
}

QString TwoDRobotModel::name() const
{
	return "TwoDRobotModelFor" + mRealModel->name();
}

QString TwoDRobotModel::friendlyName() const
{
	QRegExp versionRegExp("\\(v.*\\)");
	const int pos = versionRegExp.indexIn(mRealModel->friendlyName());
	if (pos == -1) {
		return tr("2D Model");
	}

	return tr("2D Model") + " " + versionRegExp.capturedTexts().at(0);
}

bool TwoDRobotModel::needsConnection() const
{
	return false;
}

utils::TimelineInterface &TwoDRobotModel::timeline()
{
	return mEngine->modelTimeline();
}

QList<PortInfo> TwoDRobotModel::configurablePorts() const
{
	return mRealModel->configurablePorts();
}

QList<DeviceInfo> TwoDRobotModel::convertibleBases() const
{
	return mRealModel->convertibleBases();
}

int TwoDRobotModel::priority() const
{
	return 10;  // The highest priority in all kits.
}

twoDModel::engine::TwoDModelEngineInterface *TwoDRobotModel::engine()
{
	return mEngine;
}

void TwoDRobotModel::setEngine(engine::TwoDModelEngineInterface &engine)
{
	mEngine = &engine;
}

robotParts::Device *TwoDRobotModel::createDevice(const PortInfo &port, const DeviceInfo &deviceInfo)
{
	if (deviceInfo.isA<robotParts::Button>()) {
		return new parts::Button(deviceInfo, port, buttonCodes()[port.name() + "Button"], *mEngine);
	}

	if (deviceInfo.isA<robotParts::Motor>()) {
		return new parts::Motor(deviceInfo, port, *mEngine);
	}

	if (deviceInfo.isA<robotParts::EncoderSensor>()) {
		return new parts::EncoderSensor(deviceInfo, port, *mEngine);
	}

	if (deviceInfo.isA<robotParts::TouchSensor>()) {
		return new parts::TouchSensor(deviceInfo, port, *mEngine);
	}

	if (deviceInfo.isA<robotParts::RangeSensor>()) {
		return new parts::RangeSensor(deviceInfo, port, *mEngine);
	}

	if (deviceInfo.isA<robotParts::LightSensor>()) {
		return new parts::LightSensor(deviceInfo, port, *mEngine);
	}

	if (deviceInfo.isA<robotParts::ColorSensorPassive>()) {
		return new parts::ColorSensorPassive(deviceInfo, port, *mEngine);
	}

	if (deviceInfo.isA<robotParts::ColorSensorFull>()) {
		return new parts::ColorSensorFull(deviceInfo, port, *mEngine);
	}

	if (deviceInfo.isA<robotParts::ColorSensorRed>()) {
		return new parts::ColorSensorRed(deviceInfo, port, *mEngine);
	}

	if (deviceInfo.isA<robotParts::ColorSensorGreen>()) {
		return new parts::ColorSensorGreen(deviceInfo, port, *mEngine);
	}

	if (deviceInfo.isA<robotParts::ColorSensorBlue>()) {
		return new parts::ColorSensorBlue(deviceInfo, port, *mEngine);
	}

	if (deviceInfo.isA<robotParts::GyroscopeSensor>()) {
		return new parts::Gyroscope(deviceInfo, port, *mEngine);
	}

	if (deviceInfo.isA<robotParts::AccelerometerSensor>()) {
		return new parts::Accelerometer(deviceInfo, port, *mEngine);
	}

	if (deviceInfo.isA(markerInfo())) {
		return new parts::Marker(deviceInfo, port, *mEngine);
	}

	return CommonRobotModel::createDevice(port, deviceInfo);
}

DeviceInfo TwoDRobotModel::markerInfo() const
{
	return DeviceInfo::create<parts::Marker>();
}

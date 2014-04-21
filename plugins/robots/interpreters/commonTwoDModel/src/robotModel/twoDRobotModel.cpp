#include "commonTwoDModel/robotModel/twoDRobotModel.h"

#include <qrkernel/exception/exception.h>

#include "commonTwoDModel/robotModel/parts/buttons.h"
#include "commonTwoDModel/robotModel/parts/colorSensorFull.h"
#include "commonTwoDModel/robotModel/parts/colorSensorPassive.h"
#include "commonTwoDModel/robotModel/parts/colorSensorRed.h"
#include "commonTwoDModel/robotModel/parts/colorSensorGreen.h"
#include "commonTwoDModel/robotModel/parts/colorSensorBlue.h"
#include "commonTwoDModel/robotModel/parts/display.h"
#include "commonTwoDModel/robotModel/parts/encoderSensor.h"
#include "commonTwoDModel/robotModel/parts/lightSensor.h"
#include "commonTwoDModel/robotModel/parts/motor.h"
#include "commonTwoDModel/robotModel/parts/rangeSensor.h"
#include "commonTwoDModel/robotModel/parts/touchSensor.h"

#include "commonTwoDModel/engine/twoDModelEngineInterface.h"

#include <QtCore/QDebug>

using namespace twoDModel::robotModel;
using namespace interpreterBase::robotModel;

TwoDRobotModel::TwoDRobotModel()
	: mEngine(nullptr)
{
}

bool TwoDRobotModel::needsConnection() const
{
	return false;
}

QList<DeviceInfo> TwoDRobotModel::convertibleBases() const
{
//	return { DeviceInfo::create<robotParts::TouchSensor>()
//			, DeviceInfo::create<robotParts::RangeSensor>()
//			, DeviceInfo::create<robotParts::LightSensor>()
//			, DeviceInfo::create<robotParts::ColorSensor>()
//			, DeviceInfo::create<robotParts::SoundSensor>()
//			, DeviceInfo::create<robotParts::GyroscopeSensor>()
//			, DeviceInfo::create<robotParts::AccelerometerSensor>()
//	};
	return {};
}

void TwoDRobotModel::setEngine(engine::TwoDModelEngineInterface &engine)
{
	mEngine = &engine;
}

utils::AbstractTimer *TwoDRobotModel::produceTimer()
{
	return mEngine->modelTimer();
}

robotParts::Device *TwoDRobotModel::createDevice(PortInfo const &port, DeviceInfo const &deviceInfo)
{
	if (deviceInfo.isA<robotParts::Display>()) {
		return new parts::Display(deviceInfo, port, *mEngine);
	}

	if (deviceInfo.isA<robotParts::Buttons>()) {
		return new parts::Buttons(deviceInfo, port, *mEngine);
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

	if (deviceInfo.isA<robotModel::parts::ColorSensorPassive>()) {
		return new parts::ColorSensorPassive(deviceInfo, port, *mEngine);
	}

	if (deviceInfo.isA<robotModel::parts::ColorSensorFull>()) {
		return new parts::ColorSensorFull(deviceInfo, port, *mEngine);
	}

	if (deviceInfo.isA<robotModel::parts::ColorSensorRed>()) {
		return new parts::ColorSensorRed(deviceInfo, port, *mEngine);
	}

	if (deviceInfo.isA<robotModel::parts::ColorSensorGreen>()) {
		return new parts::ColorSensorGreen(deviceInfo, port, *mEngine);
	}

	if (deviceInfo.isA<robotModel::parts::ColorSensorBlue>()) {
		return new parts::ColorSensorBlue(deviceInfo, port, *mEngine);
	}

	qDebug() << "Unknown device " + deviceInfo.toString() + " requested on port " + port.name();
	//	throw qReal::Exception("Unknown device " + deviceInfo.toString() + " requested on port " + port.name());
	return nullptr;
}

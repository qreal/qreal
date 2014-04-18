#include "commonTwoDModel/robotModel/twoDRobotModel.h"

#include <qrkernel/exception/exception.h>

#include "commonTwoDModel/robotModel/parts/buttons.h"
#include "commonTwoDModel/robotModel/parts/colorSensor.h"
#include "commonTwoDModel/robotModel/parts/display.h"
#include "commonTwoDModel/robotModel/parts/encoderSensor.h"
#include "commonTwoDModel/robotModel/parts/lightSensor.h"
#include "commonTwoDModel/robotModel/parts/motor.h"
#include "commonTwoDModel/robotModel/parts/rangeSensor.h"
#include "commonTwoDModel/robotModel/parts/touchSensor.h"

#include "commonTwoDModel/engine/twoDModelEngineInterface.h"

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

	if (deviceInfo.isA<robotParts::ColorSensor>()) {
		/// @todo: support different colors
		return new parts::ColorSensor(deviceInfo, port, *mEngine);
	}

//	throw qReal::Exception("Unknown device " + deviceInfo.toString() + " requested on port " + port.name());
	return nullptr;
}

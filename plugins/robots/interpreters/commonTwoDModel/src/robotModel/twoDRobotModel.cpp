#include "commonTwoDModel/robotModel/twoDRobotModel.h"

#include <qrkernel/exception/exception.h>

#include "commonTwoDModel/robotModel/parts/motor.h"

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
//	if (deviceInfo.isA(DeviceInfo::create<robotParts::Display>())) {
//		return new parts::Display(displayInfo(), port);
//	}

//	if (deviceInfo.isA(speakerInfo())) {
//		return new parts::Speaker(speakerInfo(), port, mRobotCommunicator);
//	}

//	if (deviceInfo.isA(buttonsInfo())) {
//		return new parts::Buttons(buttonsInfo(), port);
//	}

	if (deviceInfo.isA(DeviceInfo::create<robotParts::Motor>())) {
		return new parts::Motor(deviceInfo, port, *mEngine);
	}

//	if (deviceInfo.isA(encoderInfo())) {
//		return new parts::EncoderSensor(encoderInfo(), port, mRobotCommunicator);
//	}

//	if (deviceInfo.isA(touchSensorInfo())) {
//		return new parts::TouchSensor(touchSensorInfo(), port, mRobotCommunicator);
//	}

//	if (deviceInfo.isA(sonarSensorInfo())) {
//		return new parts::SonarSensor(sonarSensorInfo(), port, mRobotCommunicator);
//	}

//	if (deviceInfo.isA(lightSensorInfo())) {
//		return new parts::LightSensor(lightSensorInfo(), port, mRobotCommunicator);
//	}

//	if (deviceInfo.isA(colorSensorInfo())) {
//		/// @todo: support different colors
//		return new parts::ColorSensor(colorSensorInfo(), port
//				, mRobotCommunicator, enums::lowLevelSensorType::COLORBLUE);
//	}

//	if (deviceInfo.isA(soundSensorInfo())) {
//		return new parts::SoundSensor(soundSensorInfo(), port, mRobotCommunicator);
//	}

//	if (deviceInfo.isA(gyroscopeSensorInfo())) {
//		return new parts::GyroscopeSensor(gyroscopeSensorInfo(), port, mRobotCommunicator);
//	}

//	if (deviceInfo.isA(accelerometerSensorInfo())) {
//		return new parts::TouchSensor(accelerometerSensorInfo(), port, mRobotCommunicator);
//	}

//	throw qReal::Exception("Unknown device " + deviceInfo.toString() + " requested on port " + port.name());
	return nullptr;
}

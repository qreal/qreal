#include "realRobotModel.h"

#include <qrkernel/settingsManager.h>
#include <qrkernel/exception/exception.h>

#include "communication/bluetoothRobotCommunicationThread.h"
#include "communication/usbRobotCommunicationThread.h"

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

using namespace nxtKitInterpreter::robotModel::real;
using namespace utils::robotCommunication;
using namespace interpreterBase::robotModel;

RealRobotModel::RealRobotModel(QString const &kitId)
	: NxtRobotModelBase(kitId)
	, mRobotCommunicator(new RobotCommunicator(this))
{
	connect(mRobotCommunicator, &RobotCommunicator::connected, this, &RealRobotModel::connected);
	connect(mRobotCommunicator, &RobotCommunicator::disconnected, this, &RealRobotModel::disconnected);
}

QString RealRobotModel::name() const
{
	return "NxtRealRobotModel";
}

QString RealRobotModel::friendlyName() const
{
	return tr("Real Robot");
}

bool RealRobotModel::needsConnection() const
{
	return true;
}

void RealRobotModel::rereadSettings()
{
	QString const valueOfCommunication = qReal::SettingsManager::value("NxtValueOfCommunication").toString();
	if (valueOfCommunication == mLastCommunicationValue) {
		return;
	}

	mLastCommunicationValue = valueOfCommunication;
	utils::robotCommunication::RobotCommunicationThreadInterface *communicator = nullptr;
	if (valueOfCommunication == "bluetooth") {
		communicator = new communication::BluetoothRobotCommunicationThread;
	} else if (valueOfCommunication == "usb") {
		communicator = new communication::UsbRobotCommunicationThread;
	}

	mRobotCommunicator->setRobotCommunicationThreadObject(communicator);
}

void RealRobotModel::connectToRobot()
{
	mRobotCommunicator->connect();
}

void RealRobotModel::disconnectFromRobot()
{
	mRobotCommunicator->disconnect();
}

robotParts::Device *RealRobotModel::createDevice(PortInfo const &port, DeviceInfo const &deviceInfo)
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

	throw qReal::Exception("Unknown device " + deviceInfo.toString() + " requested on port " + port.name());
}

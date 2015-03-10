#include "realRobotModel.h"

#include <qrkernel/settingsManager.h>
#include <qrkernel/exception/exception.h>

#include "src/communication/bluetoothRobotCommunicationThread.h"

#include "parts/display.h"
#include "parts/speaker.h"
#include "parts/button.h"
#include "parts/motor.h"
#include "parts/touchSensor.h"
#include "parts/lightSensor.h"
#include "parts/rangeSensor.h"
#include "parts/colorSensorFull.h"

using namespace ev3::robotModel::real;
using namespace utils::robotCommunication;
using namespace kitBase::robotModel;

RealRobotModel::RealRobotModel(const QString &kitId, const QString &robotId)
	: Ev3RobotModelBase(kitId, robotId)
	, mRobotCommunicator(new RobotCommunicator(this))
{
	connect(mRobotCommunicator, &RobotCommunicator::connected, this, &RealRobotModel::connected);
	connect(mRobotCommunicator, &RobotCommunicator::disconnected, this, &RealRobotModel::disconnected);
}

QString RealRobotModel::name() const
{
	return "Ev3RealRobotModel";
}

QString RealRobotModel::friendlyName() const
{
	return tr("Interpretation (USB, Bluetooth)");
}

bool RealRobotModel::needsConnection() const
{
	return true;
}

int RealRobotModel::priority() const
{
	return 9;  /* After 2D model */
}

void RealRobotModel::rereadSettings()
{
	const QString valueOfCommunication = qReal::SettingsManager::value("Ev3ValueOfCommunication").toString();
	if (valueOfCommunication == mLastCommunicationValue) {
		return;
	}

	mLastCommunicationValue = valueOfCommunication;
	utils::robotCommunication::RobotCommunicationThreadInterface *communicator = nullptr;
	if (valueOfCommunication == "bluetooth") {
		communicator = new communication::BluetoothRobotCommunicationThread;
	} else if (valueOfCommunication == "usb") {
		communicator = new communication::BluetoothRobotCommunicationThread; // TODO usb
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

robotParts::Device *RealRobotModel::createDevice(const PortInfo &port, const DeviceInfo &deviceInfo)
{
	if (deviceInfo.isA(speakerInfo())) {
		return new parts::Speaker(speakerInfo(), port, *mRobotCommunicator);
	}

	if (deviceInfo.isA(buttonInfo())) {
		return new parts::Button(buttonInfo(), port, *mRobotCommunicator);
	}

	if (deviceInfo.isA(displayInfo())) {
		return new parts::Display(displayInfo(), port, *mRobotCommunicator);
	}

	if (deviceInfo.isA(motorInfo())) {
		return new parts::Motor(motorInfo(), port, *mRobotCommunicator);
	}

	if (deviceInfo.isA(touchSensorInfo())) {
		return new parts::TouchSensor(touchSensorInfo(), port, *mRobotCommunicator);
	}

	if (deviceInfo.isA(lightSensorInfo())) {
		return new parts::LightSensor(lightSensorInfo(), port, *mRobotCommunicator);
	}

	if (deviceInfo.isA(rangeSensorInfo())) {
		return new parts::RangeSensor(rangeSensorInfo(), port, *mRobotCommunicator);
	}

	if (deviceInfo.isA(colorFullSensorInfo())) {
		return new parts::ColorSensorFull(colorFullSensorInfo(), port, *mRobotCommunicator);
	}

	throw qReal::Exception("Unknown device " + deviceInfo.toString() + " requested on port " + port.name());
}

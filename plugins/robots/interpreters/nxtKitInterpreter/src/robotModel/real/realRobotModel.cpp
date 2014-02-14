#include "realRobotModel.h"

#include <qrkernel/settingsManager.h>
#include <qrkernel/exception/exception.h>

#include "communication/bluetoothRobotCommunicationThread.h"
#include "communication/usbRobotCommunicationThread.h"

#include "parts/motor.h"
#include "parts/touchSensor.h"

using namespace nxtKitInterpreter::robotModel::real;
using namespace utils::robotCommunication;
using namespace interpreterBase::robotModel;

RealRobotModel::RealRobotModel()
{
	/// @todo: replace onConnected() with connected() signal
	connect(&mRobotCommunicator, &RobotCommunicator::connected, this, &RealRobotModel::onConnected);
	connect(&mRobotCommunicator, &RobotCommunicator::disconnected, this, &RealRobotModel::disconnected);
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

	mRobotCommunicator.setRobotCommunicationThreadObject(communicator);
}

void RealRobotModel::doConnectToRobot()
{
	mRobotCommunicator.connect();
}

void RealRobotModel::disconnectFromRobot()
{
	mRobotCommunicator.disconnect();
}

void RealRobotModel::configureKnownDevices()
{
	PluggableDeviceInfo motorInfo = PluggableDeviceInfo::create<parts::Motor>(tr("Motor"));

	mutableConfiguration().configureDevice(new parts::Motor(motorInfo, PortInfo("A"), &mRobotCommunicator));
	mutableConfiguration().configureDevice(new parts::Motor(motorInfo, PortInfo("B"), &mRobotCommunicator));
	mutableConfiguration().configureDevice(new parts::Motor(motorInfo, PortInfo("C"), &mRobotCommunicator));
}

robotParts::PluggableDevice *RealRobotModel::createDevice(PortInfo const &port, PluggableDeviceInfo const &deviceInfo)
{
	if (deviceInfo.isA(touchSensorInfo())) {
		return new parts::TouchSensor(touchSensorInfo(), port, mRobotCommunicator);
	}

	throw qReal::Exception("Unknown device " + deviceInfo.toString() + " requested on port " + port.name());
}

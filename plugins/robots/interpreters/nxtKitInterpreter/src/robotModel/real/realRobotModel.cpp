#include "realRobotModel.h"

#include <qrkernel/settingsManager.h>

#include "communication/bluetoothRobotCommunicationThread.h"
#include "communication/usbRobotCommunicationThread.h"

#include "parts/motor.h"

using namespace nxtKitInterpreter::robotModel::real;
using namespace interpreterBase::robotModel;

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

void RealRobotModel::configureKnownDevices()
{
	PluggableDeviceInfo motorInfo = PluggableDeviceInfo::create<parts::Motor>(tr("Motor"));

	mutableConfiguration().configureDevice(new parts::Motor(motorInfo, PortInfo("A"), &mRobotCommunicator));
	mutableConfiguration().configureDevice(new parts::Motor(motorInfo, PortInfo("B"), &mRobotCommunicator));
	mutableConfiguration().configureDevice(new parts::Motor(motorInfo, PortInfo("C"), &mRobotCommunicator));
}

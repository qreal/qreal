#include "realRobotModel.h"

#include <qrkernel/settingsManager.h>

#include "communication/bluetoothRobotCommunicationThread.h"
#include "communication/usbRobotCommunicationThread.h"

using namespace nxtKitInterpreter::robotModel::real;

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

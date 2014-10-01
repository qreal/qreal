#include "realRobotModel.h"

#include <qrkernel/settingsManager.h>
#include <qrkernel/exception/exception.h>

#include "communication/bluetoothRobotCommunicationThread.h"

//#include "parts/display.h"
#include "parts/speaker.h"
#include "parts/motor.h"

using namespace ev3KitInterpreter::robotModel::real;
using namespace utils::robotCommunication;
using namespace interpreterBase::robotModel;

RealRobotModel::RealRobotModel(QString const &kitId)
    : Ev3RobotModelBase(kitId)
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
	return tr("Real Robot");
}

bool RealRobotModel::needsConnection() const
{
	return true;
}

void RealRobotModel::rereadSettings()
{
    QString const valueOfCommunication = qReal::SettingsManager::value("Ev3ValueOfCommunication").toString();
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

robotParts::Device *RealRobotModel::createDevice(PortInfo const &port, DeviceInfo const &deviceInfo)
{
    if (deviceInfo.isA(speakerInfo())) {
        return new parts::Speaker(speakerInfo(), port, *mRobotCommunicator);
    }


    if (deviceInfo.isA(motorInfo())) {
        return new parts::Motor(motorInfo(), port, *mRobotCommunicator);
    }

	throw qReal::Exception("Unknown device " + deviceInfo.toString() + " requested on port " + port.name());
}

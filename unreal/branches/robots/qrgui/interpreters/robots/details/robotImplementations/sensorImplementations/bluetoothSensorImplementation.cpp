#include "bluetoothSensorImplementation.h"
#include "../../robotParts/robotModel.h"
using namespace qReal::interpreters::robots;
using namespace details::robotImplementations::sensorImplementations;

BluetoothSensorImplementation::BluetoothSensorImplementation(RobotCommunicationInterface *const robotCommunicationInterface
		, lowLevelSensorType::SensorTypeEnum const &sensorType
		, sensorMode::SensorModeEnum const &sensorMode
		, inputPort::InputPortEnum const &port)
	: AbstractSensorImplementation(port)
	, mRobotCommunicationInterface(robotCommunicationInterface)
	, mSensorType(sensorType)
	, mSensorMode(sensorMode)
	, mIsConfigured(false)
	, mResetDone(false)
{
	connect(mRobotCommunicationInterface, SIGNAL(response(QObject*,QByteArray))
			, this, SLOT(readingDone(QObject*,QByteArray)));
}

BluetoothSensorImplementation::~BluetoothSensorImplementation()
{
	delete mRobotCommunicationInterface;
}

void BluetoothSensorImplementation::readingDone(QObject *addressee, QByteArray const &reading)
{
	if (addressee != this)
		return;
	processResponse(reading);
}

void BluetoothSensorImplementation::processResponse(QByteArray const &reading)
{
	if (reading.isEmpty()) {
		mState = idle;
		if (mResetDone) {
			qDebug() << "Response is empty, seems to be a connection failure";
			// Just ignore connection failures for now
//			emit failure();
		} else {
			if (mIsConfigured) {
				mResetDone = true;
				emit configured();
			}
			else {
				QByteArray command(5, 0);
				command[0] = 0x03;
				command[1] = 0x00;
				command[2] = telegramType::directCommandNoResponse;
				command[3] = commandCode::RESETINPUTSCALEDVALUE;
				command[4] = mPort;
				mRobotCommunicationInterface->send(this, command, 0);
			}
			mIsConfigured = true;
		}
	} else if (reading.size() >= 4 && reading[3] == commandCode::SETINPUTMODE) {
		mState = idle;
		qDebug() << "Response is a configuration response package";
		qDebug() << "Status byte is:" << static_cast<int>(reading[4]);
		mIsConfigured = true;
		emit configured();
	} else {
		sensorSpecificProcessResponse(reading);
	}
}

void BluetoothSensorImplementation::configure()
{
	QByteArray command(7, 0);
	command[0] = 0x05;  //command length
	command[1] = 0x00;
	command[2] = telegramType::directCommandNoResponse;
	command[3] = commandCode::SETINPUTMODE;
	command[4] = mPort;
	command[5] = mSensorType;
	command[6] = mSensorMode;
	mRobotCommunicationInterface->send(this, command, 0);
}

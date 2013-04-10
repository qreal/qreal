#include "waitForSensorBlock.h"

using namespace qReal::interpreters::robots::details::blocks;

WaitForSensorBlock::WaitForSensorBlock(RobotModel * const robotModel, sensorType::SensorTypeEnum type)
	: WaitBlock(robotModel), mType(type)
{
}

void WaitForSensorBlock::run()
{
	mPort = static_cast<inputPort::InputPortEnum>(intProperty("Port") - 1);
	robotParts::Sensor * const sensorInstance = sensor();

	if (!sensorInstance) {
		mActiveWaitingTimer.stop();
		error(tr("%1 is not configured on port %2").arg(name(), QString::number(static_cast<int>(mPort) + 1)));
		return;
	}

	connect(sensorInstance->sensorImpl(), SIGNAL(response(int)), this, SLOT(responseSlot(int)), Qt::UniqueConnection);
	connect(sensorInstance->sensorImpl(), SIGNAL(failure()), this, SLOT(failureSlot()), Qt::UniqueConnection);

	sensorInstance->read();
	mActiveWaitingTimer.start();
}

QList<Block::SensorPortPair> WaitForSensorBlock::usedSensors() const
{
	if (mType == sensorType::unused) {
		return QList<SensorPortPair>();
	}
	inputPort::InputPortEnum const port = static_cast<inputPort::InputPortEnum>(intProperty("Port") - 1);
	return QList<SensorPortPair>() << qMakePair(mType, static_cast<int>(port));
}

void WaitForSensorBlock::timerTimeout()
{
	robotParts::Sensor *sensorInstance = sensor();
	if (sensorInstance) {
		sensorInstance->read();
	}
}

void WaitForSensorBlock::stop()
{
	disconnect(this, SLOT(responseSlot(int)));
	disconnect(this, SLOT(failureSlot()));
	WaitBlock::stop();
}

void WaitForSensorBlock::stopActiveTimerInBlock()
{
	disconnect(this, SLOT(responseSlot(int)));
	disconnect(this, SLOT(failureSlot()));
	WaitBlock::stopActiveTimerInBlock();
}

#include "interpreterBase/blocksBase/common/waitBlock.h"

#include "interpreterBase/robotModel/robotModelUtils.h"

using namespace interpreterBase;
using namespace blocksBase::common;
using namespace robotModel;

WaitBlock::WaitBlock(RobotModelInterface &robotModel)
	: mRobotModel(robotModel)
{
	mActiveWaitingTimer.setInterval(20);
	connect(&mActiveWaitingTimer, &QTimer::timeout, this, &WaitBlock::timerTimeout);
}

WaitBlock::~WaitBlock()
{
}

void WaitBlock::setFailedStatus()
{
	Block::setFailedStatus();
	stopActiveTimerInBlock();
}

void WaitBlock::processResponce(int reading, int targetValue)
{
	const QString sign = stringProperty("Sign");
	if (sign == "equals" && reading == targetValue) {
		stop();
	} else if (sign == "greater" && reading > targetValue) {
		stop();
	} else if (sign == "less" && reading < targetValue) {
		stop();
	} else if (sign == "notLess" && reading >= targetValue) {
		stop();
	} else if (sign == "notGreater" && reading <= targetValue) {
		stop();
	}
}

void WaitBlock::stop()
{
	mActiveWaitingTimer.stop();
	emit done(mNextBlockId);
}

void WaitBlock::failureSlot()
{
	mActiveWaitingTimer.stop();
	emit failure();
}

void WaitBlock::stopActiveTimerInBlock()
{
	mActiveWaitingTimer.stop();
}

QMap<PortInfo, DeviceInfo> WaitBlock::usedDevices()
{
	const DeviceInfo deviceInfo = device();
	const PortInfo portInfo = RobotModelUtils::findPort(mRobotModel, port(), deviceInfo.direction());
	QMap<PortInfo, DeviceInfo> result;
	if (!deviceInfo.isNull() && portInfo.isValid()) {
		result[portInfo] = deviceInfo;
	}

	return result;
}

QString WaitBlock::port()
{
	return eval<QString>("Port");
}

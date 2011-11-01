#include "waitForColorIntensityBlock.h"

#include "../../sensorConstants.h"

using namespace qReal;
using namespace interpreters::robots;
using namespace interpreters::robots::details::blocks;

WaitForColorIntensityBlock::WaitForColorIntensityBlock(details::RobotModel const * const robotModel)
	: mColorSensor(NULL)
	, mRobotModel(robotModel)
{
	// There is about 30 ms latency within robot bluetooth chip, so it is useless to
	// read sensor too frequently.
	mActiveWaitingTimer.setInterval(100);

	connect(&mActiveWaitingTimer, SIGNAL(timeout()), this, SLOT(timerTimeout()));
}

void WaitForColorIntensityBlock::run()
{
	inputPort::InputPortEnum const port = static_cast<inputPort::InputPortEnum>(intProperty("Port") - 1);
	mColorSensor = mRobotModel->colorSensor(port);

	if (!mColorSensor) {
		mActiveWaitingTimer.stop();
		error(tr("Color sensor is not configured on this port or it is configured in a wrong mode (not \"Full color\" mode needed)"));
		return;
	}

	connect(mColorSensor->sensorImpl(), SIGNAL(response(int)), this, SLOT(responseSlot(int)));
	connect(mColorSensor->sensorImpl(), SIGNAL(failure()), this, SLOT(failureSlot()));

	mColorSensor->read();
	mActiveWaitingTimer.start();
}

void WaitForColorIntensityBlock::stop()
{
	mActiveWaitingTimer.stop();
	emit done(mNextBlock);
}

void WaitForColorIntensityBlock::responseSlot(int reading)
{
	int const targetIntensity = evaluate("Intensity").toInt();

	QString const sign = stringProperty("Sign");
	if (sign == "=")
		if (reading != targetIntensity)
			stop();
	if (sign == ">")
		if (reading <= targetIntensity)
			stop();
	if (sign == "<")
		if (reading >= targetIntensity)
			stop();
	if (sign == ">=")
		if (reading < targetIntensity)
			stop();
	if (sign == "<=")
		if (reading > targetIntensity)
			stop();

	if (targetIntensity < reading)
		stop();
}

void WaitForColorIntensityBlock::failureSlot()
{
	mActiveWaitingTimer.stop();
	emit failure();
}

void WaitForColorIntensityBlock::timerTimeout()
{
	mColorSensor->read();
}

QList<Block::SensorPortPair> WaitForColorIntensityBlock::usedSensors() const
{
	return QList<SensorPortPair>();  // Unchecked for now since it needs one of some different color sensor modes.
}

void WaitForColorIntensityBlock::stopActiveTimerInBlock()
{
	mActiveWaitingTimer.stop();
}

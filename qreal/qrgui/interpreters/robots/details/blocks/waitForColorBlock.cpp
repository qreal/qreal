#include "waitForColorBlock.h"

#include "../../sensorConstants.h"

using namespace qReal;
using namespace interpreters::robots;
using namespace details::blocks;

WaitForColorBlock::WaitForColorBlock(details::RobotModel const * const robotModel)
	: mColorSensor(NULL)
	, mRobotModel(robotModel)
{
	// There is about 30 ms latency within robot bluetooth chip, so it is useless to
	// read sensor too frequently.
	mActiveWaitingTimer.setInterval(100);

	connect(&mActiveWaitingTimer, SIGNAL(timeout()), this, SLOT(timerTimeout()));
}

void WaitForColorBlock::run()
{
	inputPort::InputPortEnum const port = static_cast<inputPort::InputPortEnum>(intProperty("Port") - 1);
	mColorSensor = mRobotModel->colorSensor(port);

	if (!mColorSensor) {
		mActiveWaitingTimer.stop();
		error(tr("Color sensor is not configured on this port or it is configured in a wrong mode (\"Full color\" mode needed)"));
		return;
	}

	connect(mColorSensor->sensorImpl(), SIGNAL(response(int)), this, SLOT(responseSlot(int)));
	connect(mColorSensor->sensorImpl(), SIGNAL(failure()), this, SLOT(failureSlot()));

	mColorSensor->read();
	mActiveWaitingTimer.start();
}

void WaitForColorBlock::responseSlot(int reading)
{
	QString const targetColor = stringProperty("Color");
	QString color;
	switch (reading) {
	case 1: color = tr("Black");
		break;
	case 2: color = tr("Blue");
		break;
	case 3: color = tr("Green");
		break;
	case 4: color = tr("Yellow");
		break;
	case 5: color = tr("Red");
		break;
	case 6: color = tr("White");
		break;
	default:
		return;
	}

	if (targetColor == color) {
		mActiveWaitingTimer.stop();
		emit done(mNextBlock);
	}
}

void WaitForColorBlock::failureSlot()
{
	mActiveWaitingTimer.stop();
	emit failure();
}

void WaitForColorBlock::timerTimeout()
{
	mColorSensor->read();
}

QList<Block::SensorPortPair> WaitForColorBlock::usedSensors() const
{
	inputPort::InputPortEnum const port = static_cast<inputPort::InputPortEnum>(intProperty("Port") - 1);
	return QList<SensorPortPair>() << qMakePair(sensorType::colorFull, static_cast<int>(port));
}

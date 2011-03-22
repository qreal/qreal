#include "timerBlock.h"

#include <QtCore/QDebug>

using namespace qReal;
using namespace interpreters::robots::details::blocks;

void TimerBlock::run()
{
	int const interval = evaluate("Delay").toInt();
	qDebug() << "interval=" << interval;

	mTimer.setInterval(interval);
	mTimer.setSingleShot(true);
	connect(&mTimer, SIGNAL(timeout()), this, SLOT(timeout()));
	mTimer.start();
}

void TimerBlock::timeout()
{
	qDebug() << "emit done(mNextBlock)";
	emit done(mNextBlock);
}

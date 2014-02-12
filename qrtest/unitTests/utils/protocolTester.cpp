#include "protocolTester.h"

#include <algorithm>

using namespace qrTest;

ProtocolTester::ProtocolTester()
{
	mTimer.setSingleShot(true);
	connect(&mMapper, static_cast<void (QSignalMapper::*)(QString const &)>(&QSignalMapper::mapped)
			, this, &ProtocolTester::onSignal);

	connect(&mTimer, &QTimer::timeout, this, &ProtocolTester::onTimeout);
}

ProtocolTester::~ProtocolTester()
{
	qDeleteAll(mSenders);
}

void ProtocolTester::onSignal(QString const &signalName)
{
	if (mSignals.contains(signalName)) {
		++mSignals[signalName];
	}
}

bool ProtocolTester::isSignalEmitted(QString const &signalName)
{
	return mSignals.contains(signalName) && mSignals[signalName] > 0;
}

bool ProtocolTester::allIsGood() const
{
	return std::all_of(mSignals.constBegin(), mSignals.constEnd(), [] (int count) { return count > 0; } );
}

void ProtocolTester::wait(int timeout)
{
	mTimer.setInterval(timeout);
	mTimer.start();
	mEventLoop.exec();
}

void ProtocolTester::onTimeout()
{
	mEventLoop.exit();
}

#include "signalsTester.h"

#include <algorithm>

#include <QtCore/QDebug>

using namespace qrTest;

SignalsTester::SignalsTester(OrderEnum order)
	: mInOrder(order)
{
	mTimer.setSingleShot(true);
	connect(&mMapper, static_cast<void (QSignalMapper::*)(const QString &)>(&QSignalMapper::mapped)
			, this, &SignalsTester::onSignal);

	connect(&mTimer, &QTimer::timeout, this, &SignalsTester::onTimeout);
}

SignalsTester::~SignalsTester()
{
	qDeleteAll(mSenders);
}

bool SignalsTester::isSignalEmitted(const QString &signalName) const
{
	return mSignals.contains(signalName) && mSignals.value(signalName) > 0;
}

void SignalsTester::onSignal(const QString &signalName)
{
	if (mSignals.contains(signalName)) {
		if (mInOrder == inOrder) {
			for (const QString &signal : mSignalsOrder) {
				if (signal == signalName) {
					break;
				} else if (mSignals.value(signal) == 0) {
					qDebug() << "Signal" << signal << "is not called before signal" << signalName;
					mEventLoop.exit();

					return;
				}
			}
		}

		++mSignals[signalName];
	}

	if (allIsGood()) {
		mEventLoop.exit();
	}
}

bool SignalsTester::allIsGood() const
{
	return std::all_of(mSignals.constBegin(), mSignals.constEnd(), [] (int count) { return count > 0; } );
}

void SignalsTester::wait(int timeout)
{
	mTimer.setInterval(timeout);
	mTimer.start();
	mEventLoop.exec();
}

void SignalsTester::onTimeout()
{
	mEventLoop.exit();
}

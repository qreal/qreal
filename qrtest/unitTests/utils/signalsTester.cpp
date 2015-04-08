/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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

/* Copyright 2016 CyberTech Labs Ltd.
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

#pragma once

#include <QtCore/QObject>
#include <QtCore/QEventLoop>

#include "declSpec.h"

namespace qrTest {

/// Utility class that provides event loop based waiting for given timeout and signals.
/// Typical usage pattern is as follows:
/// Wait waiter(1000);
/// waiter.stopAt(someObject, &SomeObject::someSignal);
/// waiter.wait();
class TEST_UTILS_EXPORT Wait
{
public:
	/// Constructor.
	/// @param timeout - timeout in milliseconds that is a maximum time to wait for a signal to appear.
	Wait(int timeout);

	/// Stop waiting upon receiving a given signal from a given sender. This method can be called as many times as
	/// needed, and has "or" semantics, so the waiting will stop once any signal is received.
	template <typename Func>
	void stopAt(const typename QtPrivate::FunctionPointer<Func>::Object *object, Func signal)
	{
		QObject::connect(object, signal, &mLoop, &QEventLoop::quit);
	}

	/// Starts waiting event loop.
	void wait();

	/// Helper method that waits for a given amount of milliseconds. Equivalent to using Wait object without any
	/// signals connected.
	static TEST_UTILS_EXPORT void wait(int msecs);

private:
	/// Event loop that processes events during wait.
	QEventLoop mLoop;

	/// Timeout value in milliseconds.
	int mTimeout;
};

}

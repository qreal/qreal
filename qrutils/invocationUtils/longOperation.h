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

#pragma once


#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtCore/QTimer>

#include "progress.h"
#include "qrutils/utilsDeclSpec.h"

namespace invocation
{

/// @brief Representing operation invocation state
enum InvocationState
{
	/// Operation is invocing
	Invocing = 0
	/// Operation finished its execution itself
	, FinishedNormally
	/// Operation was canceled by user or timeout occured
	, Canceled
};

/// Base class for all operations that can execute for a
/// rather long time and must be invoced in another thread.
class QRUTILS_EXPORT LongOperation : public QObject
{
	Q_OBJECT

public:
	/// Returns if operation is running at the moment
	bool isRunning() const;
	/// Returns operation invocation state
	InvocationState invocationResult() const;

	/// Returns operation timeout. NOTE: timeout works only
	/// in asynchronious case
	int timeout() const;
	/// Returns if operation provides its progress.
	/// Progress can be obtained using progress() method
	bool hasProgress() const;
	/// If operation provides its progress returns operation
	/// progress wrapped in special class. Else returns nullptr
	Progress *progress() const;

	/// Starts operation in another thread and blocks
	/// the caller thread till operation finished.
	/// NOTE: timeout doesn`t work in this case
	/// @param priority The OS sheduller parameter
	void invoceSync(QThread::Priority priority = QThread::NormalPriority);
	/// Starts operation in another thread. When operation is finished
	/// finished(...) signal is emitted
	/// @param priority The OS sheduller parameter
	void invoceAsync(QThread::Priority priority = QThread::NormalPriority);
	/// Terminates operation thread
	void cancel();

signals:
	/// Emitted just before operation start
	void beforeStarted();
	/// Emitted after operation thread started and control returned to
	/// caller thread
	void afterStarted();
	/// Emitted when operation finished itself or was canceled
	void finished(invocation::InvocationState invocationResult);

protected:
	// This class itself mustn`t be instantiated because of
	// thread initialization lack. Must be determined in child classes
	explicit LongOperation(int timeout = INT_MAX);
	virtual ~LongOperation() {}

	virtual void startInvocation(QThread::Priority priority = QThread::NormalPriority);

	QThread *mThread;
	bool mIsOperationWithProgress;
	Progress *mProgress;

protected slots:
	virtual void onThreadFinished();
	virtual void onThreadTerminated();

private:
	void stopTimer();

	int mTimeout;
	QTimer *mTimer;
	InvocationState mInvocationResult;
};

}

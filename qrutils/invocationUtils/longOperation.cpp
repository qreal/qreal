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

#include "longOperation.h"

using namespace invocation;

LongOperation::LongOperation(int timeout)
	: mThread(nullptr)
	, mIsOperationWithProgress(false)
	, mProgress(new Progress)
	, mTimeout(timeout)
	, mTimer(nullptr)
{
}

int LongOperation::timeout() const
{
	return mTimeout;
}

bool LongOperation::hasProgress() const
{
	return mIsOperationWithProgress;
}

invocation::Progress *LongOperation::progress() const
{
	return (mIsOperationWithProgress) ? mProgress : nullptr;
}

bool LongOperation::isRunning() const
{
	return mInvocationResult == Invocing;
}

invocation::InvocationState LongOperation::invocationResult() const
{
	return mInvocationResult;
}

void LongOperation::invoceSync(QThread::Priority priority)
{
	// TODO: start timer in another thread to get timeouts work
	startInvocation(priority);
	mThread->wait();
}

void LongOperation::invoceAsync(QThread::Priority priority)
{
	startInvocation(priority);
}

void LongOperation::cancel()
{
	mThread->terminate();
}

void LongOperation::onThreadFinished()
{
	if (mInvocationResult == Canceled) {
		return; // We already emited terminated signal
	}
	mInvocationResult = FinishedNormally;
	stopTimer();
	emit finished(mInvocationResult);
}

void LongOperation::onThreadTerminated()
{
	mInvocationResult = Canceled;
	stopTimer();
	emit finished(mInvocationResult);
}

void LongOperation::stopTimer()
{
	if (!mTimer) {
		return;
	}
	if (mTimer->isActive()) {
		mTimer->stop();
	}
}

void LongOperation::startInvocation(QThread::Priority priority)
{
	connect(mThread, SIGNAL(finished()), this, SLOT(onThreadFinished()));
	connect(mThread, SIGNAL(terminated()), this, SLOT(onThreadTerminated()));

	mTimer = new QTimer;
	mTimer->connect(mTimer, SIGNAL(timeout()), mThread, SLOT(terminate()));

	mInvocationResult = Invocing;
	emit beforeStarted();

	mTimer->start(mTimeout);
	mThread->start(priority);

	emit afterStarted();
}

#include "longOperation.h"

using namespace invocation;

LongOperation::LongOperation(int timeout)
	: mThread(NULL)
	, mIsOperationWithProgress(false)
	, mProgress(new Progress)
	, mTimeout(timeout)
	, mTimer(NULL)
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
	return (mIsOperationWithProgress) ? mProgress : NULL;
}

bool LongOperation::isRunning() const
{
	return mInvocationState == Invocing;
}

invocation::InvocationState LongOperation::invocationState() const
{
	return mInvocationState;
}

void LongOperation::invokeSync(QThread::Priority priority)
{
	// TODO: start timer in another thread to get timeouts work
	startInvocation(priority);
	mThread->wait();
}

void LongOperation::invokeAsync(QThread::Priority priority)
{
	startInvocation(priority);
}

void LongOperation::cancel()
{
	mThread->terminate();
}

void LongOperation::onThreadFinished()
{
	if (mInvocationState == Canceled) {
		return; // We already emited terminated signal
	}
	mInvocationState = FinishedNormally;
	stopTimer();
	emit finished(mInvocationState);
	emit finished(this);
}

void LongOperation::onThreadTerminated()
{
	mInvocationState = Canceled;
	stopTimer();
	emit finished(mInvocationState);
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

	mInvocationState = Invocing;
	emit beforeStarted();

	mTimer->start(mTimeout);
	mThread->start(priority);

	emit afterStarted();
}

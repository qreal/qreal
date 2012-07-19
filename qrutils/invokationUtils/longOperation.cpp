#include "longOperation.h"

using namespace invokation;

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

invokation::Progress *LongOperation::progress() const
{
	return (mIsOperationWithProgress) ? mProgress : NULL;
}

bool LongOperation::isRunning() const
{
	return mInvokationResult == Invoking;
}

invokation::InvokationState LongOperation::invokationResult() const
{
	return mInvokationResult;
}

void LongOperation::invokeSync(QThread::Priority priority)
{
  // TODO: start timer in another thread to get timeouts work
	startInvokation(priority);
	mThread->wait();
}

void LongOperation::invokeAsync(QThread::Priority priority)
{
	startInvokation(priority);
}

void LongOperation::cancel()
{
	mThread->terminate();
}

void LongOperation::onThreadFinished()
{
	if (mInvokationResult == Canceled) {
		return; // We already emited terminated signal
	}
	mInvokationResult = FinishedNormally;
	stopTimer();
	emit finished(mInvokationResult);
}

void LongOperation::onThreadTerminated()
{
	mInvokationResult = Canceled;
	stopTimer();
	emit finished(mInvokationResult);
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

void LongOperation::startInvokation(QThread::Priority priority)
{
	connect(mThread, SIGNAL(finished()), this, SLOT(onThreadFinished()));
	connect(mThread, SIGNAL(terminated()), this, SLOT(onThreadTerminated()));

	mTimer = new QTimer;
	mTimer->connect(mTimer, SIGNAL(timeout()), mThread, SLOT(terminate()));

	mInvokationResult = Invoking;
	emit beforeStarted();

	mTimer->start(mTimeout);
	mThread->start(priority);

	emit afterStarted();
}

#include "progressBar.h"

using namespace qReal;

ProgressBar::ProgressBar(QWidget *parent)
	: QProgressBar(parent), mOperation(NULL)
{
	reset();
}

invocation::LongOperation *ProgressBar::operation() const
{
	return mOperation;
}

bool ProgressBar::isOperationConnected() const
{
	return mOperation != NULL;
}

void ProgressBar::reset()
{
	QProgressBar::reset();
	setFormat("%p%");
}

void ProgressBar::connectOperation(invocation::LongOperation *operation)
{
	reset();
	if (mOperation) {
		disconnectOperation();
	}
	mOperation = operation;
	connectOperation();
}

void ProgressBar::connectOperation()
{
	connect(mOperation, SIGNAL(beforeStarted()), this, SLOT(onBeforeStart()));
	connect(mOperation, SIGNAL(finished(invocation::InvocationState)), this, SLOT(onOperationComplete(invocation::InvocationState)));
	setTextVisible(mOperation->hasProgress());
	if (mOperation->hasProgress()) {
		invocation::Progress *progress = mOperation->progress();
		setMinimum(progress->minimum());
		setMaximum(progress->maximum());
		setValue(progress->value());
		connect(progress, SIGNAL(minimumChanged(int)), this, SLOT(setMinimum(int)));
		connect(progress, SIGNAL(maximumChanged(int)), this, SLOT(setMaximum(int)));
		connect(progress, SIGNAL(valueChanged(int)), this, SLOT(setValue(int)));
	}
}

void ProgressBar::disconnectOperation()
{
	mOperation->disconnect(this, SLOT(reset()));
	mOperation->disconnect(this, SLOT(onOperationComplete(invocation::InvocationState)));
	if (mOperation->hasProgress()) {
		invocation::Progress *progress = mOperation->progress();
		progress->disconnect(this, SLOT(setMinimum(int)));
		progress->disconnect(this, SLOT(setMaximum(int)));
		progress->disconnect(this, SLOT(setValue(int)));
	}
}

void ProgressBar::onBeforeStart()
{
	reset();
	if (!mOperation->hasProgress()) {
		setMinimum(0);
		setMaximum(0);
	}
}

void ProgressBar::onOperationComplete(invocation::InvocationState result)
{
	if (!mOperation->hasProgress()) {
		setMaximum(100); // Stop indicator
	}
	switch(result) {
	case invocation::FinishedNormally:
		onOperationFinishedNormally();
		break;
	case invocation::Canceled:
		onOperationCanceled();
		break;
	default:
		break;
	}
	mOperation = NULL;
}

void ProgressBar::onOperationFinishedNormally()
{
	if (mOperation->hasProgress()) {
		invocation::Progress *progress = mOperation->progress();
		progress->setValue(progress->maximum());
	}
}

void ProgressBar::onOperationCanceled()
{
	QString const format = (mOperation->hasProgress())
			? tr("%p% - canceled")
			: tr("canceled");
	setTextVisible(true);
	setFormat(format);
}

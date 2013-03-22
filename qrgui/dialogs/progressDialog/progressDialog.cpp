#include "progressDialog.h"

using namespace qReal;

ProgressDialog::ProgressDialog(QWidget *parent)
	: QProgressDialog(parent), mProgressBar(new ProgressBar(this))
{
	setWindowTitle("");
	setBar(mProgressBar);
	connect(this, SIGNAL(canceled()), this, SLOT(onCanceled()));

	QString const text = tr("Please wait...");
	setLabelText(text);
}

invocation::LongOperation *ProgressDialog::operation() const
{
	return mProgressBar->operation();
}

bool ProgressDialog::isOperationConnected() const
{
	return mProgressBar->isOperationConnected();
}

void ProgressDialog::connectOperation(invocation::LongOperation *operation)
{
	mProgressBar->connectOperation(operation);
	connect(operation, SIGNAL(afterStarted()), this, SLOT(exec()));
	connect(operation, SIGNAL(finished(invocation::InvocationState))
			, this, SLOT(close()));
}

void ProgressDialog::onCanceled()
{
	if (isOperationConnected()) {
		operation()->cancel();
	}
}

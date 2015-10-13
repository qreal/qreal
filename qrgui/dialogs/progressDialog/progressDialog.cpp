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

#include "progressDialog.h"

using namespace qReal;

ProgressDialog::ProgressDialog(QWidget *parent)
	: QProgressDialog(parent), mProgressBar(new ProgressBar(this))
{
	setWindowTitle("");
	setBar(mProgressBar);
	connect(this, SIGNAL(canceled()), this, SLOT(onCanceled()));

	const QString text = tr("Please wait...");
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

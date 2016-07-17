/* Copyright 2012-2016 Dmitry Mordvinov
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
	: QProgressDialog(parent)
	, mProgressBar(new ProgressBar(this))
{
	setWindowTitle(QString());
	setBar(mProgressBar);

	const QString text = tr("Please wait...");
	setLabelText(text);
}

void ProgressDialog::reportOperation(const QFuture<void> &operation, const QString &description)
{
	mProgressBar->reportOperation(operation, description);
	connect(&mProgressBar->currentOperation(), &QFutureWatcher<void>::started, this, &QDialog::show);
	connect(&mProgressBar->currentOperation(), &QFutureWatcher<void>::finished, this, &QDialog::hide);
	connect(&mProgressBar->currentOperation(), &QFutureWatcher<void>::canceled, this, &QDialog::hide);
	connect(this, &QProgressDialog::canceled, &mProgressBar->currentOperation(), &QFutureWatcher<void>::cancel);
	if (!description.isEmpty()) {
		setLabelText(description);
	}
}

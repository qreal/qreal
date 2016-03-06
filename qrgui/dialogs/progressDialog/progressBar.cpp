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

#include "progressBar.h"

using namespace qReal;

ProgressBar::ProgressBar(QWidget *parent)
	: QProgressBar(parent)
	, mCurrentWatcher(nullptr)
{
	reset();
}

void ProgressBar::reportOperation(const QFuture<void> &operation, const QString &description)
{
	Q_UNUSED(description)
	QFutureWatcher<void> * const watcher = new QFutureWatcher<void>(this);
	watcher->setFuture(operation);
	connect(watcher, &QFutureWatcher<void>::started, this, &QProgressBar::reset);
	connect(watcher, &QFutureWatcher<void>::progressRangeChanged, this, &QProgressBar::setRange);
	connect(watcher, &QFutureWatcher<void>::progressValueChanged, this, &QProgressBar::setValue);
	connect(watcher, &QFutureWatcher<void>::progressTextChanged, this, &QProgressBar::setFormat);
	mCurrentWatcher = watcher;
}

const QFutureWatcher<void> &ProgressBar::currentOperation() const
{
	return *mCurrentWatcher;
}

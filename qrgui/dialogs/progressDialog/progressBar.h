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

#include <QtWidgets/QProgressBar>

#include <qrutils/invocationUtils/longOperation.h>

namespace qReal
{

/// @brief Progress dialog capable with invocation utils
class ProgressBar : public QProgressBar
{
	Q_OBJECT

public:
	explicit ProgressBar(QWidget *parent = 0);

	/// Returns last connected operation
	invocation::LongOperation *operation() const;
	/// Returns if some running operation connected at the moment
	bool isOperationConnected() const;

	/// Binds current progress bar with some operation from
	/// invocation utils. If operation provides progress info
	/// progress bar shows that progress. Else progress bar
	/// just shows that something is invocing at the moment
	void connectOperation(invocation::LongOperation *operation);
	/// Reinits progress bar
	void reset();

private slots:
	void onBeforeStart();
	void onOperationComplete(invocation::InvocationState result);

private:
	void connectOperation();
	void disconnectOperation();

	void onOperationFinishedNormally();
	void onOperationCanceled();

	invocation::LongOperation* mOperation;
};

}

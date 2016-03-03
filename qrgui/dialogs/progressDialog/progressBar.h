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

#pragma once

#include <QtCore/QFuture>
#include <QtCore/QFutureWatcher>
#include <QtWidgets/QProgressBar>

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/progressReporterInterface.h>

#include "qrgui/dialogs/dialogsDeclSpec.h"

namespace qReal {

/// @brief Progress dialog capable with QtConcurrent system.
class QRGUI_DIALOGS_EXPORT ProgressBar : public QProgressBar, public ProgressReporterInterface
{
	Q_OBJECT

public:
	explicit ProgressBar(QWidget *parent = 0);

	void reportOperation(const QFuture<void> &operation, const QString &description = QString()) override;

	/// Returns component watching the state of currently tracked long operation.
	const QFutureWatcher<void> &currentOperation() const;

private:
	const QFutureWatcher<void> *mCurrentWatcher;  // Takes ownership via QObject parentship.
};

}

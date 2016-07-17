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

#include <QtWidgets/QProgressDialog>

#include "qrgui/dialogs/progressDialog/progressBar.h"

namespace qReal {

/// @brief Progress dialog capable with QtConcurrent system. Uses @see ProgressBar
class QRGUI_DIALOGS_EXPORT ProgressDialog : public QProgressDialog, public ProgressReporterInterface
{
	Q_OBJECT

public:
	explicit ProgressDialog(QWidget *parent = 0);

	void reportOperation(const QFuture<void> &operation, const QString &description = QString()) override;

private:
	ProgressBar *mProgressBar;  // Has ownership
};

}

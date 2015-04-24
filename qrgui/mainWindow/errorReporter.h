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

#include <QtWidgets/QDockWidget>

#include <qrkernel/ids.h>
#include <qrkernel/definitions.h>

#include "qrgui/mainWindow/error.h"
#include "qrgui/mainWindow/errorListWidget.h"
#include "qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h"

namespace qReal {
namespace gui {

class ErrorReporter : public QObject, public ErrorReporterInterface
{
	Q_OBJECT

public:
	ErrorReporter();
	ErrorReporter(ErrorListWidget * const errorListWidget, QDockWidget * const errorList);

	virtual void addInformation(const QString &message, const Id &position = Id::rootId());
	virtual void addWarning(const QString &message, const Id &position = Id::rootId());
	virtual void addError(const QString &message, const Id &position = Id::rootId());
	virtual void addCritical(const QString &message, const Id &position = Id::rootId());
	virtual bool wereErrors();

	bool showErrors(ErrorListWidget * const errorListWidget, QDockWidget * const errorList) const;
	void updateVisibility(bool isVisible);

signals:
	/// Emitted when new message with level 'Info' added to error reporter.
	void informationAdded(const QString &message, const Id &position);

	/// Emitted when new message with level 'Warning' added to error reporter.
	void warningAdded(const QString &message, const Id &position);

	/// Emitted when new message with level 'Error' added to error reporter.
	void errorAdded(const QString &message, const Id &position);

	/// Emitted when new message with level 'Critical' added to error reporter.
	void criticalAdded(const QString &message, const Id &position);

public slots:
	virtual void clear();
	virtual void clearErrors();

private:
	static QString severityMessage(const Error &error);
	void showError(const Error &error, ErrorListWidget * const errorListWidget) const;

	QList<Error> mErrors;

	ErrorListWidget * const mErrorListWidget;  // Doesn't have ownership
	QDockWidget * const mErrorList;  // Doesn't have ownership

	/// Should error window be shown or not
	bool mIsVisible;
};

}
}

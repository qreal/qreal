#pragma once

#include <QtWidgets/QDockWidget>

#include "mainWindow/error.h"
#include "mainWindow/errorListWidget.h"
#include "plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h"

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

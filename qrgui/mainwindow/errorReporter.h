#pragma once

#include <QtCore/QString>
#include <QtCore/QList>

#include "mainWindow.h"
#include "../../qrkernel/ids.h"
#include "../../qrkernel/definitions.h"
#include "error.h"
#include "errorListWidget.h"
#include "../toolPluginInterface/usedInterfaces/errorReporterInterface.h"

namespace qReal {
namespace gui {

class ErrorReporter : public QObject, public ErrorReporterInterface {
	Q_OBJECT

public:
	ErrorReporter();
	ErrorReporter(ErrorListWidget* const errorListWidget, QDockWidget* const errorList);

	void addInformation(QString const &message, Id const &position = Id::rootId());
	void addWarning(QString const &message, Id const &position = Id::rootId());
	void addError(QString const &message, Id const &position = Id::rootId());
	void addCritical(QString const &message, Id const &position = Id::rootId());
	bool showErrors(ErrorListWidget* const errorListWidget, QDockWidget* const errorList) const;

	virtual void clear();
	virtual void clearErrors();

	virtual bool wereErrors();

public slots:
	void updateVisibility(bool isVisible);

private:
	static QString severityMessage(Error const &error);
	void showError(Error const &error, ErrorListWidget* const errorListWidget) const;

	QList<Error> mErrors;

	ErrorListWidget* const mErrorListWidget;  // Doesn't have ownership
	QDockWidget* const mErrorList;  // Doesn't have ownership

	/// Should error window be shown or not
	bool mIsVisible;
};

}
}

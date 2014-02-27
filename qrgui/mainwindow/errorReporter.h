#pragma once

#include <QtCore/QString>
#include <QtCore/QList>

#include <qrkernel/ids.h>
#include <qrkernel/definitions.h>

#include "mainwindow/mainWindow.h"
#include "mainwindow/error.h"
#include "mainwindow/hint.h"
#include "mainwindow/errorListWidget.h"
#include "toolPluginInterface/usedInterfaces/errorReporterInterface.h"
#include "toolPluginInterface/usedInterfaces/hintInterface.h"


namespace qReal {
namespace gui {

class ErrorReporter : public QObject, public ErrorReporterInterface, public HintInterface {
	Q_OBJECT

public:
	ErrorReporter();
	ErrorReporter(ErrorListWidget* const errorListWidget, QDockWidget* const errorList);

	virtual void addInformation(QString const &message, Id const &position = Id::rootId());
	virtual void addWarning(QString const &message, Id const &position = Id::rootId());
	virtual void addError(QString const &message, Id const &position = Id::rootId());
	virtual void addCritical(QString const &message, Id const &position = Id::rootId());

	virtual void addHint(QString const &message, Id const &position = Id::rootId());

    virtual bool wereErrors();

	bool showErrors(ErrorListWidget* const errorListWidget, QDockWidget* const errorList) const;
	void updateVisibility(bool isVisible);

public slots:
	virtual void clear();
	virtual void clearErrors();

private:
	static QString severityMessage(Error const &error);
	void showError(Error const &error, ErrorListWidget* const errorListWidget) const;
	void showHint(Hint const &hint, ErrorListWidget* const errorListWidget) const;

	QList<Error> mErrors;
	QList<Hint> mHints;

	ErrorListWidget* const mErrorListWidget;  // Doesn't have ownership
	QDockWidget* const mErrorList;  // Doesn't have ownership

	/// Should error window be shown or not
	bool mIsVisible;
};

}
}

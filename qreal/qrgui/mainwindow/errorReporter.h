#pragma once

#include <QtCore/QString>
#include <QtCore/QList>

#include"mainwindow.h"
#include "../kernel/ids.h"
#include "../kernel/definitions.h"
#include "error.h"
#include "errorlistwidget.h"

namespace qReal {
namespace gui {

class ErrorReporter : public QObject {
	Q_OBJECT

public:
	ErrorReporter();
	ErrorReporter(ErrorListWidget* const errorListWidget, QDockWidget* const errorList);

	void addInformation(QString const &message, Id const &position = Id::rootId());
	void addWarning(QString const &message, Id const &position = Id::rootId());
	void addError(QString const &message, Id const &position = Id::rootId());
	void addCritical(QString const &message, Id const &position = Id::rootId());
	bool showErrors(ErrorListWidget* const errorListWidget, QDockWidget* const errorList) const;

	void clear();
private:
	QList<Error> mErrors;

	ErrorListWidget* const mErrorListWidget;  // Doesn't have ownership
	QDockWidget* const mErrorList;  // Doesn't have ownership

	static QString severityMessage(Error const &error);
	void showError(Error const &error, ErrorListWidget* const errorListWidget) const;

};

}
}

#include "errorReporter.h"
#include <QtGui/QMessageBox>
#include "errorlistwidget.h"
#include "../kernel/exception/exception.h"

using namespace qReal;
using namespace gui;

void ErrorReporter::addInformation(QString const &message, NewType const &position)
{
	Error error(message, Error::information, position);
	mErrors.append(error);
}

void ErrorReporter::addWarning(QString const &message, NewType const &position)
{
	Error error(message, Error::warning, position);
	mErrors.append(error);
}

void ErrorReporter::addError(QString const &message, NewType const &position)
{
	Error error(message, Error::error, position);
	mErrors.append(error);
}

void ErrorReporter::addCritical(QString const &message, NewType const &position)
{
	Error error(message, Error::critical, position);
	mErrors.append(error);
}

bool ErrorReporter::showErrors(ErrorListWidget* const errorListWidget, QDockWidget* const errorList) const
{
	errorListWidget->clear();

	if (mErrors.isEmpty()) {
		errorList->setVisible(false);
		return true;
	}

	QString message;
	errorList->setVisible(true);
	foreach (Error error, mErrors) {
		QListWidgetItem* item = new QListWidgetItem(errorListWidget);
		message = severityMessage(error) + " ";
		message += error.message();
		switch (error.severity()) {
		case Error::information:
			item->setIcon(QIcon(":/icons/information.png"));
			break;
		case Error::warning:
			item->setIcon(QIcon(":/icons/warning.png"));
			break;
		case Error::error:
			item->setIcon(QIcon(":/icons/error.png"));
			break;
		case Error::critical:
			item->setIcon(QIcon(":/icons/critical.png"));
			break;
		default:
			throw new Exception("Incorrect total severity");
		}
		item->setText(" " + message.trimmed());
		item->setTextAlignment(Qt::AlignVCenter);
		item->setToolTip(error.position().toString());
		errorListWidget->addItem(item);
	}
	return false;
}

QString ErrorReporter::severityMessage(Error const &error)
{
	switch (error.severity()) {
	case Error::information:
		return "INFORMATION:";
	case Error::warning:
		return "WARNING:";
	case Error::error:
		return "ERROR:";
	case Error::critical:
		return "CRITICAL:";
	default:
		throw new Exception("Incorrect severity of an error");
	}
}



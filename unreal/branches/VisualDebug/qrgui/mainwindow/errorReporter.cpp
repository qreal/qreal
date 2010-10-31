#include "errorReporter.h"
#include <QtGui/QMessageBox>

#include "../kernel/exception/exception.h"

using namespace qReal;
using namespace gui;

Error::Error(QString const &message, Severity const &severity, Id const &position)
	: mMessage(message), mSeverity(severity), mPosition(position)
{
}

Error::Severity Error::severity() const
{
	return mSeverity;
}

QString Error::message() const
{
	return mMessage;
}

Id Error::position() const
{
	return mPosition;
}


void ErrorReporter::addInformation(QString const &message, Id const &position)
{
	Error error(message, Error::information, position);
	mErrors.append(error);
}

void ErrorReporter::addWarning(QString const &message, Id const &position)
{
	Error error(message, Error::warning, position);
	mErrors.append(error);
}

void ErrorReporter::addError(QString const &message, Id const &position)
{
	Error error(message, Error::error, position);
	mErrors.append(error);
}

void ErrorReporter::addCritical(QString const &message, Id const &position)
{
	Error error(message, Error::critical, position);
	mErrors.append(error);
}

bool ErrorReporter::showErrors(QString const &successMessage, QListWidget* const errorListWidget) const
{
	QString const windowTitle = "Results";

	if (mErrors.isEmpty()) {
		QString labelText = windowTitle + ": " + successMessage;
		errorListWidget->addItem(labelText);
		return true;
	}

	QString message;

	Error::Severity const totalSeverity = maxSeverity();
	foreach (Error error, mErrors) {
		message = severityMessage(error) + " ";
		message += error.message() + "\n";
		if (error.position() != ROOT_ID)
			message += "    at " + error.position().toString();
		switch (totalSeverity) {
		case Error::information:
			errorListWidget->addItem(" " + message);
			break;
		case Error::warning:
			errorListWidget->addItem(" " + message);
			break;
		case Error::error:
			errorListWidget->addItem(" " + message);
			break;
		case Error::critical:
			errorListWidget->addItem(" " + message);
			break;
		default:
			throw new Exception("Incorrect total severity");
		}
	}
	return false;
}

Error::Severity ErrorReporter::maxSeverity() const
{
	Error::Severity currentSeverity = Error::information;
	foreach (Error error, mErrors)
		if (error.severity() > currentSeverity)
			currentSeverity = error.severity();
	return currentSeverity;
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

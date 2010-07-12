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

void ErrorReporter::showErrors(QString const &successMessage) const
{
	QString const windowTitle = "Results";

	if (mErrors.isEmpty()) {
		QMessageBox::information(NULL, windowTitle, successMessage);
		return;
	}

	QString message;
	foreach (Error error, mErrors) {
		message += severityMessage(error) + " ";
		message += error.message() + "\n";
		if (error.position() != ROOT_ID)
			message += "    at " + error.position().toString() + "\n\n";
	}

	Error::Severity const totalSeverity = maxSeverity();
	switch (totalSeverity) {
	case Error::information:
		QMessageBox::information(NULL, windowTitle, message);
		break;
	case Error::warning:
		QMessageBox::warning(NULL, windowTitle, message);
		break;
	case Error::error:
		QMessageBox::warning(NULL, windowTitle, message);
		break;
	case Error::critical:
		QMessageBox::critical(NULL, windowTitle, message);
		break;
	default:
		throw new Exception("Incorrect total severity");
	}
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

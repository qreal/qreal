#include "errorReporter.h"
#include <QtGui/QMessageBox>
#include "errorlistwidget.h"
#include "../kernel/exception/exception.h"

using namespace qReal;
using namespace gui;

ErrorReporter::ErrorReporter()
	: mErrorListWidget(NULL)
	, mErrorList(NULL)
	, mIsVisible(true)
{
}

ErrorReporter::ErrorReporter(ErrorListWidget* const errorListWidget, QDockWidget* const errorList)
	: mErrorListWidget(errorListWidget)
	, mErrorList(errorList)
	, mIsVisible(true)
{
}

void ErrorReporter::updateVisibility(bool isVisible)
{
	if (mIsVisible == isVisible)
		return;

	mIsVisible = isVisible;

	if (!mIsVisible)
		mErrorList->hide();
	else if (mErrorListWidget->count() > 0)
		mErrorList->show();
}

void ErrorReporter::addInformation(QString const &message, Id const &position)
{
	Error error(message, Error::information, position);
	mErrors.append(error);
	showError(error, mErrorListWidget);
}

void ErrorReporter::addWarning(QString const &message, Id const &position)
{
	Error error(message, Error::warning, position);
	mErrors.append(error);
	showError(error, mErrorListWidget);
}

void ErrorReporter::addError(QString const &message, Id const &position)
{
	Error error(message, Error::error, position);
	mErrors.append(error);
	showError(error, mErrorListWidget);
}

void ErrorReporter::addCritical(QString const &message, Id const &position)
{
	Error error(message, Error::critical, position);
	mErrors.append(error);
	showError(error, mErrorListWidget);
}

bool ErrorReporter::showErrors(ErrorListWidget* const errorListWidget, QDockWidget* const errorList) const
{
	errorListWidget->clear();

	if (mErrors.isEmpty()) {
		errorList->setVisible(false);
		return true;
	}

	errorList->setVisible(true);
	foreach (Error error, mErrors)
		showError(error, errorListWidget);
	return false;
}

void ErrorReporter::clear()
{
	if (mErrorListWidget)
		mErrorListWidget->clear();
	if (mErrorList)
		mErrorList->setVisible(false);
}

void ErrorReporter::clearErrors() {
	mErrors.clear();
}

void ErrorReporter::showError(Error const &error, ErrorListWidget* const errorListWidget) const
{
	if (!errorListWidget)
		return;

	if (mErrorList && !mErrorList->isVisible() &&  mIsVisible)
		mErrorList->setVisible(true);

	QListWidgetItem* item = new QListWidgetItem(errorListWidget);
	QString message = severityMessage(error) + " ";
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

QString ErrorReporter::severityMessage(Error const &error)
{
	switch (error.severity()) {
	case Error::information:
		return tr("INFORMATION:");
	case Error::warning:
		return tr("WARNING:");
	case Error::error:
		return tr("ERROR:");
	case Error::critical:
		return tr("CRITICAL:");
	default:
		throw new Exception("Incorrect severity of an error");
	}
}

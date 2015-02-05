#include "errorReporter.h"

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QLabel>

#include <qrkernel/exception/exception.h>

#include "mainWindow/errorListWidget.h"

using namespace qReal;
using namespace gui;

ErrorReporter::ErrorReporter()
	: mErrorListWidget(nullptr)
	, mErrorList(nullptr)
	, mIsVisible(true)
{
}

ErrorReporter::ErrorReporter(ErrorListWidget* const errorListWidget, QDockWidget* const errorList)
	: mErrorListWidget(errorListWidget)
	, mErrorList(errorList)
	, mIsVisible(true)
{
	connect(mErrorListWidget, &ErrorListWidget::clearRequested, this, &ErrorReporter::clear);
}

void ErrorReporter::updateVisibility(bool isVisible)
{
	if (mIsVisible == isVisible) {
		return;
	}

	mIsVisible = isVisible;

	if (!mIsVisible) {
		mErrorList->hide();
	} else if (mErrorListWidget->count() > 0) {
		mErrorList->show();
	}
}

void ErrorReporter::addInformation(const QString &message, const Id &position)
{
	Error error(message, Error::information, position);
	mErrors.append(error);
	showError(error, mErrorListWidget);
	emit informationAdded(message, position);
}

void ErrorReporter::addWarning(const QString &message, const Id &position)
{
	Error error(message, Error::warning, position);
	mErrors.append(error);
	showError(error, mErrorListWidget);
	emit warningAdded(message, position);
}

void ErrorReporter::addError(const QString &message, const Id &position)
{
	Error error(message, Error::error, position);
	mErrors.append(error);
	showError(error, mErrorListWidget);
	emit errorAdded(message, position);
}

void ErrorReporter::addCritical(const QString &message, const Id &position)
{
	Error error(message, Error::critical, position);
	mErrors.append(error);
	showError(error, mErrorListWidget);
	emit criticalAdded(message, position);
}

bool ErrorReporter::showErrors(ErrorListWidget* const errorListWidget, QDockWidget* const errorList) const
{
	errorListWidget->clear();

	if (mErrors.isEmpty()) {
		errorList->setVisible(false);
		return true;
	}

	errorList->setVisible(true);
	for (const Error &error : mErrors) {
		showError(error, errorListWidget);
	}

	return false;
}

void ErrorReporter::clear()
{
	if (mErrorListWidget) {
		mErrorListWidget->clear();
	}
	if (mErrorList) {
		mErrorList->setVisible(false);
	}
}

void ErrorReporter::clearErrors()
{
	mErrors.clear();
}

bool ErrorReporter::wereErrors()
{
	for (const Error &error : mErrors) {
		if (error.severity() == Error::critical || error.severity() == Error::error) {
			return true;
		}
	}
	return false;
}

void ErrorReporter::showError(const Error &error, ErrorListWidget * const errorListWidget) const
{
	if (!errorListWidget) {
		return;
	}

	if (mErrorList && !mErrorList->isVisible() &&  mIsVisible) {
		mErrorList->setVisible(true);
	}

	QListWidgetItem *item = new QListWidgetItem(errorListWidget);
	QString const message = QString(" <font color='gray'>%1</font> <u>%2</u> %3").arg(
			error.timestamp(), severityMessage(error), error.message());
	switch (error.severity()) {
	case Error::information:
		item->setIcon(QIcon(":/mainWindow/images/information.png"));
		break;
	case Error::warning:
		item->setIcon(QIcon(":/mainWindow/images/warning.png"));
		break;
	case Error::error:
		item->setIcon(QIcon(":/mainWindow/images/error.png"));
		break;
	case Error::critical:
		item->setIcon(QIcon(":/mainWindow/images/critical.png"));
		break;
	default:
		throw new Exception("Incorrect total severity");
	}

	QLabel *label = new QLabel(message.trimmed());
	label->setAlignment(Qt::AlignVCenter);
	label->setOpenExternalLinks(true);
	item->setToolTip(error.position().toString());
	errorListWidget->addItem(item);
	errorListWidget->setItemWidget(item, label);
	errorListWidget->setCurrentItem(item);
}

QString ErrorReporter::severityMessage(const Error &error)
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

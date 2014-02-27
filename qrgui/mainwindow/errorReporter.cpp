#include "errorReporter.h"

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QDockWidget>

#include <qrkernel/exception/exception.h>

#include "mainwindow/errorListWidget.h"
#include "qrutils/uxInfo/uxInfo.h"

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
	connect(mErrorListWidget, SIGNAL(clearRequested()), this, SLOT(clear()));
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

void ErrorReporter::addInformation(QString const &message, Id const &position)
{
	utils::UXInfo::reportErrors("information", position.editor(), position.element(), message);
	Error error(message, Error::information, position);
    mErrors.append(error);
	showError(error, mErrorListWidget);
}

void ErrorReporter::addWarning(QString const &message, Id const &position)
{
	utils::UXInfo::reportErrors("warning", position.editor(), position.element(), message);
	Error error(message, Error::warning, position);
	mErrors.append(error);
	showError(error, mErrorListWidget);
}

void ErrorReporter::addError(QString const &message, Id const &position)
{
	utils::UXInfo::reportErrors("error", position.editor(), position.element(), message);
	Error error(message, Error::error, position);
	mErrors.append(error);
	showError(error, mErrorListWidget);
}

void ErrorReporter::addCritical(QString const &message, Id const &position)
{
	utils::UXInfo::reportErrors("critical", position.editor(), position.element(), message);
	Error error(message, Error::critical, position);
	mErrors.append(error);
	showError(error, mErrorListWidget);
}

void ErrorReporter::addHint(QString const &message, Id const &position)
{
    utils::UXInfo::reportErrors("hint", position.editor(), position.element(), message);
    Hint hint(message, position);
    mHints.append(hint);
    showHint(hint, mErrorListWidget);
}


bool ErrorReporter::showErrors(ErrorListWidget* const errorListWidget, QDockWidget* const errorList) const
{
	errorListWidget->clear();

	if (mErrors.isEmpty()) {
		errorList->setVisible(false);
		return true;
	}

	errorList->setVisible(true);
	foreach (Error error, mErrors) {
		showError(error, errorListWidget);
	}
    foreach (Hint hint, mHints) {
        showHint(hint, errorListWidget);
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
	foreach (Error const &error, mErrors) {
		if (error.severity() == Error::critical || error.severity() == Error::error) {
			return true;
		}
	}
	return false;
}

void ErrorReporter::showError(Error const &error, ErrorListWidget* const errorListWidget) const
{
	if (!errorListWidget) {
		return;
	}

	if (mErrorList && !mErrorList->isVisible() &&  mIsVisible) {
		mErrorList->setVisible(true);
	}

	QListWidgetItem* item = new QListWidgetItem(errorListWidget);
	QString const message = QString(" <font color='gray'>%1</font> <u>%2</u> %3").arg(
			error.timestamp(), severityMessage(error), error.message());
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
	QLabel *label = new QLabel(message.trimmed());
	label->setAlignment(Qt::AlignVCenter);
	label->setOpenExternalLinks(true);
	item->setToolTip(error.position().toString());
	errorListWidget->addItem(item);
	errorListWidget->setItemWidget(item, label);
	errorListWidget->setCurrentItem(item);
}

 void ErrorReporter::showHint(Hint const &hint, ErrorListWidget* const errorListWidget) const
 {
     if (!errorListWidget) {
         return;
     }

     if (mErrorList && !mErrorList->isVisible() &&  mIsVisible) {
         mErrorList->setVisible(true);
     }

     QListWidgetItem* item = new QListWidgetItem(errorListWidget);
     QString const message = QString(" <font color='gray'>%1</font> <u>%2</u> %3").arg(
             hint.timestamp(), tr("HINT:"), hint.message());
        item->setIcon(QIcon(":/icons/information.png"));
     QLabel *label = new QLabel(message.trimmed());
     label->setAlignment(Qt::AlignVCenter);
     label->setOpenExternalLinks(true);
     item->setToolTip(hint.position().toString());
     errorListWidget->addItem(item);
     errorListWidget->setItemWidget(item, label);
     errorListWidget->setCurrentItem(item);
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

/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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
{
}

ErrorReporter::ErrorReporter(ErrorListWidget* const errorListWidget)
	: mErrorListWidget(errorListWidget)
{
	connect(mErrorListWidget, &ErrorListWidget::clearRequested, this, &ErrorReporter::clear);
}

void ErrorReporter::addInformation(const QString &message, const Id &position)
{
	Error error(message, Error::information, position);
	mErrors.append(error);
	showError(error);
	emit informationAdded(message, position);
}

void ErrorReporter::addWarning(const QString &message, const Id &position)
{
	Error error(message, Error::warning, position);
	mErrors.append(error);
	showError(error);
	emit warningAdded(message, position);
}

void ErrorReporter::addError(const QString &message, const Id &position)
{
	Error error(message, Error::error, position);
	mErrors.append(error);
	showError(error);
	emit errorAdded(message, position);
}

void ErrorReporter::addCritical(const QString &message, const Id &position)
{
	Error error(message, Error::critical, position);
	mErrors.append(error);
	showError(error);
	emit criticalAdded(message, position);
}

void ErrorReporter::clear()
{
	if (mErrorListWidget) {
		mErrorListWidget->clear();
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

void ErrorReporter::showError(const Error &error)
{
	if (mErrorListWidget) {
		mErrorListWidget->addError(error);
	}
}

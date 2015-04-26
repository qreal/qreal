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

#include <QtCore/QTextCodec>
#include <QtWidgets/QApplication>

#include "externalClientPluginBase.h"
#include "invocationUtils/functorOperation.h"

using namespace qReal::versioning;

ExternalClientPluginBase::ExternalClientPluginBase()
	: mWorkingCopyManager(nullptr)
	, mPathToClient(QString())
	, mClientProcess(nullptr)
{
	connect(this, SIGNAL(errorOccured(QString)), this, SLOT(onErrorOccured(QString)));
}

void ExternalClientPluginBase::setWorkingCopyManager(qrRepo::WorkingCopyManagementInterface *workingCopyManager)
{
	mWorkingCopyManager = workingCopyManager;
}

void ExternalClientPluginBase::init(const PluginConfigurator &configurator)
{
	mMainWindow = &(configurator.mainWindowInterpretersInterface());
	mErrorReporter = mMainWindow->errorReporter();
}

QString ExternalClientPluginBase::pathToClient() const
{
	return mPathToClient;
}

void ExternalClientPluginBase::setPathToClient(const QString &pathToClient)
{
	mPathToClient = pathToClient;
}

void ExternalClientPluginBase::onErrorOccured(QString const &errorMessage)
{
	mErrorReporter->addError(errorMessage);
}

void ExternalClientPluginBase::onErrorsOccured(QStringList const &errorMessages)
{
	foreach (QString const &errorMessage, errorMessages) {
		onErrorOccured(errorMessage);
	}
}

bool ExternalClientPluginBase::invokeOperation(
	const QStringList &args
	, bool needPreparation
	, QString const &workingDir
	, bool const checkWorkingDir
	, bool needProcessing
	, QString const &targetProject
	, QString const &sourceProject
	, bool reportErrors
)
{
	if (needPreparation) {
		prepareWorkingCopy(workingDir, sourceProject);
	}
	bool const result = startAndWait(args, reportErrors, workingDir, checkWorkingDir);
	if (needProcessing) {
		processWorkingCopy(targetProject);
	}
	return result;
}

invocation::LongOperation* ExternalClientPluginBase::invokeOperationAsync(
		QStringList const &args
		, QVariant const &tag
		, bool needPreparation
		, QString const &workingDir
		, QString const &sourceProject
		, bool const checkWorkingDir
		, bool reportErrors
)
{
	if (needPreparation) {
		prepareWorkingCopy(workingDir, sourceProject);
	}
	invocation::details::FunctorInterface<bool> *functor =
		new invocation::details::Functor<bool, ExternalClientPluginBase
			, bool(ExternalClientPluginBase::*)(const QStringList&, bool, const QString&, const bool)
			,const QStringList&, bool, const QString&, const bool>(
				this
				, &ExternalClientPluginBase::startAndWait
				, args
				, reportErrors
				, workingDir
				, checkWorkingDir
	);

	invocation::FunctorOperation<bool> *operation = new invocation::FunctorOperation<bool>(functor);
	// TODO: make it with progress
	connect(operation, SIGNAL(finished(invocation::LongOperation*))
			, this, SLOT(onOperationComplete(invocation::LongOperation*)));
	mRunningOperationsCallbacksMap.insert(operation, tag);
	mMainWindow->reportOperation(operation);
	operation->invokeAsync();
	return operation;
}

void ExternalClientPluginBase::prepareWorkingCopy(const QString &workingDir, QString const &sourceProject)
{
	if (mWorkingCopyManager) {
		mWorkingCopyManager->prepareWorkingCopy(workingDir.isEmpty() ? tempFolder() : workingDir, sourceProject);
	}
}

void ExternalClientPluginBase::processWorkingCopy(const QString &targetProject)
{
	if (mWorkingCopyManager) {
		mWorkingCopyManager->processWorkingCopy(tempFolder(), targetProject);
	}
}

QString ExternalClientPluginBase::standartOutput() const
{
	QString const output = mClientProcess->readAllStandardOutput();
	return QString::fromLocal8Bit(output.toStdString().c_str());
}

bool ExternalClientPluginBase::startAndWait(const QStringList &args
		, bool reportErrors, QString const &workingDir
		, const bool checkWorkingCopy)
{
	QString const workingCopyPath = workingDir.isEmpty() ? tempFolder() : workingDir;
	if (checkWorkingCopy && !isMyWorkingCopy(workingCopyPath)) {
		if (reportErrors) {
			emit errorOccured(workingCopyPath + tr(" is not a working copy"));
		}
		return false;
	}

	if (!startProcess(args, workingCopyPath, reportErrors)) {
		return false;
	}
	if (!waitForClient(reportErrors)) {
		return false;
	}
	return processErrors(reportErrors);
}

void ExternalClientPluginBase::onOperationComplete(invocation::LongOperation *operation)
{
	invocation::FunctorOperation<bool> *functor =
			dynamic_cast<invocation::FunctorOperation<bool> *>(operation);
	bool result = functor->invocationState() == invocation::FinishedNormally;
	result = result && functor->result();

	const QVariant tag = mRunningOperationsCallbacksMap[operation];
	emit operationIsFinished(tag, result);
}


bool ExternalClientPluginBase::startProcess(QStringList const &args, QString const &workingDir, bool reportErrors)

{
	if (!checkClientPath(reportErrors)) {
		return false;
	}
	mClientProcess = new QProcess;

	mClientProcess->setWorkingDirectory(workingDir);
	mClientProcess->start(mPathToClient, args);

	if (!mClientProcess->waitForStarted()) {
		if (reportErrors) {
			QString text = tr("An error occured while starting versioning client process (maybe path is not correct?)");
			emit errorOccured(text);
		}
		return false;
	}
	QCoreApplication::processEvents();
	return true;
}

bool ExternalClientPluginBase::checkClientPath(bool reportErrors)
{
	if (mPathToClient.isEmpty()) {
		if (reportErrors) {
			emit errorOccured(tr("Path to versioning client is empty"));
		}
		return false;
	}
	return true;
}

bool ExternalClientPluginBase::processErrors(bool reportErrors)
{
	QByteArray error = mClientProcess->readAllStandardError();
	if (error.size() > 0 && reportErrors) {
		emit errorOccured(QString(error));
	}
	return error.isEmpty();
}

bool ExternalClientPluginBase::waitForClient(bool reportErrors)
{
	int waitingTimeout = timeout();
	if (mClientProcess->state() == QProcess::Running) {
		if (!mClientProcess->waitForFinished(waitingTimeout)) {
			mClientProcess->kill();
			if (reportErrors) {
				emit errorOccured(tr("Versioning client timeout"));
			}
			processErrors();
			return false;
		}
	}
	return true;
}

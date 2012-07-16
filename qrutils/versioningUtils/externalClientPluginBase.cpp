#include "externalClientPluginBase.h"

#include <QtCore/QTextCodec>
#include <QtGui/QApplication>

using namespace qReal::versioning;

ExternalClientPluginBase::ExternalClientPluginBase()
	: mWorkingCopyManager(NULL),
	  mPathToClient(QString()),
	  mClientProcess(new QProcess(NULL))
{
}

void ExternalClientPluginBase::setWorkingCopyManager(qrRepo::versioning::WorkingCopyManagementInterface *workingCopyManager)
{
	mWorkingCopyManager = workingCopyManager;
}

void ExternalClientPluginBase::init(const PluginConfigurator &configurator)
{
	mErrorReporter = configurator.mainWindowInterpretersInterface().errorReporter();
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

bool ExternalClientPluginBase::invokeOperation(const QStringList &args
	, bool needPreparation, bool needProcessing
	, QString const &targetProject, bool reportErrors)
{
	if (needPreparation && mWorkingCopyManager) {
		mWorkingCopyManager->prepareWorkingCopy(tempFolder());
	}
	if (!startProcess(args, reportErrors)) {
		return false;
	}
	if (!waitForClient(reportErrors)) {
		return false;
	}
	bool result = processErrors(reportErrors);
	if (needProcessing && mWorkingCopyManager) {
		mWorkingCopyManager->processWorkingCopy(tempFolder(), targetProject);
	}
	return result;
}

QString ExternalClientPluginBase::standartOutput() const
{
	QString const output = mClientProcess->readAllStandardOutput();
	return QString::fromLocal8Bit(output.toStdString().c_str());
}

bool ExternalClientPluginBase::startProcess(const QStringList &args, bool reportErrors)
{
	if (!checkClientPath(reportErrors)) {
		return false;
	}
	mClientProcess->start(mPathToClient, args);
	if (!mClientProcess->waitForStarted()) {
		if (reportErrors) {
			onErrorOccured(tr("An error occured while starting versioning client process (maybe path is not correct?)"));
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
			onErrorOccured(tr("Path to versioning client is empty"));
		}
		return false;
	}
	return true;
}

bool ExternalClientPluginBase::processErrors(bool reportErrors)
{
	QByteArray error = mClientProcess->readAllStandardError();
	if (error.size() > 0 && reportErrors) {
		onErrorOccured(QString(error));
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
				onErrorOccured(tr("Versioning client timeout"));
			}
			processErrors();
			return false;
		}
	}
	return true;
}

#include "externalClientPluginBase.h"

#include <QTextCodec>
#include <QCoreApplication>
#include <QApplication>

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
	foreach (QString const &errorMessage, errorMessages)
	{
		onErrorOccured(errorMessage);
	}
}

bool ExternalClientPluginBase::doOperation(const QStringList &args, bool needPreparation,
	bool needProcessing, QString const &targetProject)
{
	if (needPreparation && mWorkingCopyManager) {
		mWorkingCopyManager->prepareWorkingCopy(tempFolder());
	}
	if (!startProcess(args)) {
		return false;
	}
	if (!waitForClient()) {
		return false;
	}
	bool result = processErrors();
	if (needProcessing && mWorkingCopyManager) {
		mWorkingCopyManager->processWorkingCopy(tempFolder(), targetProject);
	}
	return result;
}

QString ExternalClientPluginBase::standartOutput()
{
	QString output = mClientProcess->readAllStandardOutput();
	return QString::fromLocal8Bit(output.toStdString().c_str());
}

bool ExternalClientPluginBase::startProcess(const QStringList &args)
{
	if (!checkClientPath()) {
		return false;
	}
	mClientProcess->start(mPathToClient, args);
	if (!mClientProcess->waitForStarted()) {
		// TODO: translate
		onErrorOccured("An error occured while starting versioning client process (maybe path is not correct?)");
		return false;
	}
	QCoreApplication::processEvents();
	return true;
}

bool ExternalClientPluginBase::checkClientPath()
{
	if (mPathToClient.isEmpty()) {
		// TODO: translate
		onErrorOccured("Path to versioning client is empty");
		return false;
	}
	return true;
}

bool ExternalClientPluginBase::processErrors()
{
	QByteArray error = mClientProcess->readAllStandardError();
	if (error.size() > 0) {
		onErrorOccured(QString(error));
	}
	return error.isEmpty();
}

bool ExternalClientPluginBase::waitForClient()
{
	QCoreApplication::processEvents();
	int waitingTimeout = timeout();
	QCoreApplication::processEvents();
	if (mClientProcess->state() == QProcess::Running) {
		if (!mClientProcess->waitForFinished(waitingTimeout)) {
			mClientProcess->kill();
			// TODO: translate
			onErrorOccured("Versioning client timeout");
			processErrors();
			return false;
		}
	}
	return true;
}

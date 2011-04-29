#include "externalClient.h"

using namespace qrRepo::details;

ExternalClient::ExternalClient(const QString &pathToClient)
	: mPathToClient(pathToClient), mDiffProvider(pathToClient)
{
	initProcess();
}

ExternalClient::ExternalClient(const ExternalClient &other)
	: mPathToClient(other.mPathToClient), mDiffProvider(mPathToClient)
{
	initProcess();
}

void ExternalClient::setPathToClient(const QString &pathToClient)
{
	mPathToClient = pathToClient;
	mDiffProvider.setPathToClient(pathToClient);
}

bool ExternalClient::doCheckout(const QString &from, const QString &to)
{
	QStringList arguments;
	arguments << "checkout" << from << to;
	mClientProcess->start(mPathToClient, arguments);
	mClientProcess->waitForFinished();
	return processErrors();
}

bool ExternalClient::doUpdate(const QString &to)
{
	QStringList arguments;
	arguments << "update" << to;
	mClientProcess->start(mPathToClient, arguments);
	mClientProcess->waitForFinished();
	return processErrors();
}

bool ExternalClient::doCommit(const QString &from)
{
	QStringList arguments;
	arguments << "commit" << from << "-m" << "no_message";
	mClientProcess->start(mPathToClient, arguments);
	mClientProcess->waitForFinished();
	return processErrors();
}

bool ExternalClient::doAdd(const QString &what, bool force)
{
	QStringList arguments;
	arguments << "add" << what;
	if (force)
	{
		arguments << "--force";
	}
	mClientProcess->start(mPathToClient, arguments);
	mClientProcess->waitForFinished();
	return processErrors();
}

bool ExternalClient::doRemove(const QString &what, bool force)
{
	QStringList arguments;
	arguments << "remove" << what;
	if (force)
	{
		arguments << "--force";
	}
	mClientProcess->start(mPathToClient, arguments);
	mClientProcess->waitForFinished();
	return processErrors();
}

void ExternalClient::getDiff(QString const &workingCopy)
{
	mDiffProvider.getDifference(workingCopy);
}

QStringList ExternalClient::newErrors()
{
	QStringList result(mErrors);
	mErrors.clear();
	return result;
}

void ExternalClient::initProcess()
{
	mClientProcess = new QProcess(NULL);
	QStringList parts = mPathToClient.split('/');
	parts.removeLast();
	QString pathToDir = parts.join("/");
//	mClientProcess->setStandardOutputFile(pathToDir+"/qRealLog.log", QIODevice::Append);
//	mClientProcess->setStandardErrorFile(pathToDir+"/qRealLog.log", QIODevice::Append);
}

bool ExternalClient::processErrors()
{
	QByteArray error = mClientProcess->readAllStandardError();
	if (error.size() > 0)
		mErrors << QString(error);
	return error.size() == 0;
}

#include "externalClient.h"

using namespace qrRepo::details;

ExternalClient::ExternalClient(const QString &pathToClient)
	: mPathToClient(pathToClient)
{
	initProcess();
}

ExternalClient::ExternalClient(const ExternalClient &other)
	: mPathToClient(other.mPathToClient)
{
	initProcess();
}

void ExternalClient::setPathToClient(const QString &pathToClient)
{
	mPathToClient = pathToClient;
}

void ExternalClient::doCheckout(const QString &from, const QString &to) const
{
	QStringList arguments;
	arguments << "checkout" << from << to;
	mClientProcess->start(mPathToClient, arguments);
	mClientProcess->waitForReadyRead();
}

void ExternalClient::doUpdate(const QString &to) const
{
	QStringList arguments;
	arguments << "update" << to;
	mClientProcess->start(mPathToClient, arguments);
	mClientProcess->waitForReadyRead();
}

void ExternalClient::doCommit(const QString &from) const
{
	QStringList arguments;
	arguments << "commit" << from << "-m" << "no_message";
	mClientProcess->start(mPathToClient, arguments);
	mClientProcess->waitForReadyRead();
}

void ExternalClient::doAdd(const QString &what, bool force) const
{
	QStringList arguments;
	arguments << "add" << what;
	if (force)
	{
		arguments << "--force";
	}
	mClientProcess->start(mPathToClient, arguments);
	mClientProcess->waitForReadyRead();
}

void ExternalClient::doRemove(const QString &what, bool force) const
{
	QStringList arguments;
	arguments << "remove" << what;
	if (force)
	{
		arguments << "--force";
	}
	mClientProcess->start(mPathToClient, arguments);
	mClientProcess->waitForReadyRead();
}

QStringList ExternalClient::getNewErrors()
{
	QStringList result(mErrors);
	mErrors.clear();
	return result;
}

void ExternalClient::processErrors()
{
	QByteArray error = mClientProcess->readAllStandardError();
	if (error.size() > 0)
		mErrors << QString(error);
}

void ExternalClient::initProcess()
{
	mClientProcess = new QProcess(NULL);
//	connect(mClientProcess, SIGNAL(readyReadStandardError()), this, SLOT(processErrors()));
	QStringList parts = mPathToClient.split('/');
	parts.removeLast();
	QString pathToDir = parts.join("/");
	mClientProcess->setStandardOutputFile(pathToDir+"/qRealLog.log", QIODevice::Append);
	mClientProcess->setStandardErrorFile(pathToDir+"/qRealLog.log", QIODevice::Append);
}

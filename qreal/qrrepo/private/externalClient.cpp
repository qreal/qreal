#include "externalClient.h"

using namespace qrRepo::details;

ExternalClient::ExternalClient(const QString &pathToClient)
	: mPathToClient(pathToClient)
{
	mClientProcess = new QProcess(NULL);
	mClientProcess->setStandardOutputFile(mPathToClient+"qRealLog.log", QIODevice::Append);
	mClientProcess->setStandardErrorFile(mPathToClient+"qRealLog.log", QIODevice::Append);
}

void ExternalClient::setPathToClient(const QString &pathToClient)
{
	mPathToClient = pathToClient;
}

void ExternalClient::doCheckout(const QString &from, const QString &to) const
{
	QStringList arguments;
	arguments << "checkout" << from << to;
	mClientProcess->start(mPathToClient+"svn.exe", arguments);
	mClientProcess->waitForFinished();
}

void ExternalClient::doUpdate(const QString &to) const
{
	QStringList arguments;
	arguments << "update" << to;
	mClientProcess->start(mPathToClient+"svn.exe", arguments);
	mClientProcess->waitForFinished();
}

void ExternalClient::doCommit(const QString &from) const
{
	QStringList arguments;
	arguments << "commit" << from;
	mClientProcess->start(mPathToClient+"svn.exe", arguments);
	mClientProcess->waitForFinished();
}

void ExternalClient::doAdd(const QString &what, bool force) const
{
	QStringList arguments;
	arguments << "add" << what;
	if (force)
	{
		arguments << "--force";
	}
	mClientProcess->start(mPathToClient+"svn.exe", arguments);
	mClientProcess->waitForFinished();
}

void ExternalClient::doRemove(const QString &what, bool force) const
{
	QStringList arguments;
	arguments << "remove" << what;
	if (force)
	{
		arguments << "--force";
	}
	mClientProcess->start(mPathToClient+"svn.exe", arguments);
	mClientProcess->waitForFinished();
}

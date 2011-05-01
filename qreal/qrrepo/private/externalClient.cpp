#include "externalClient.h"

#include <QMessageBox>
#include <QTextCodec>

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

QString ExternalClient::info(const QString &workingDir)
{
	QStringList arguments;
	arguments << "info" << workingDir;
	mClientProcess->start(mPathToClient, arguments);
	mClientProcess->waitForFinished();
	QString info(mClientProcess->readAllStandardOutput());
	processErrors();
	return QString::fromLocal8Bit(info.toStdString().c_str());
}

QString ExternalClient::repoUrl(const QString &workingDir)
{
	QString repoInfo = info(workingDir);
	int ind = repoInfo.indexOf("Repository Root: ");
	if (ind == -1)
	{
		mErrors << "Can`t find repository root";
		return "";
	}
	repoInfo = repoInfo.mid(ind + QString("Repository Root: ").length());
	ind = repoInfo.indexOf("Repository UUID: ");
	if (ind == -1)
	{
		mErrors << "Can`t find repository UUID";
		return "";
	}
	repoInfo = repoInfo.mid(0,  ind-2);
	return repoInfo;
}

void ExternalClient::getDiff(QHash<qReal::Id, Object*> const &repoObjects, QHash<qReal::Id, Object*> const &workingCopyObjects)
{
	mDiffProvider.getDifference(repoObjects, workingCopyObjects);
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

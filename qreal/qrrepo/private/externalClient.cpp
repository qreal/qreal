#include "externalClient.h"

#include <QMessageBox>
#include <QTextCodec>
#include <QSettings>
#include <QCoreApplication>

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

bool ExternalClient::doCheckout(const QString &from, const QString &to)
{
	QStringList arguments;
	arguments << "checkout" << from << to;
	if (!startProcess(arguments))
	{
		return false;
	}
	if (!waitForClient())
	{
		return false;
	}
	return processErrors();
}

bool ExternalClient::doUpdate(const QString &to)
{
	QStringList arguments;
	arguments << "update" << to;
	if (!startProcess(arguments))
	{
		return false;
	}
	if (!waitForClient())
	{
		return false;
	}
	return processErrors();
}

bool ExternalClient::doCommit(const QString &from, const QString &message)
{
	if (!checkSvnClientPath())
	{
		return false;
	}
	QStringList arguments;
	arguments << "commit" << from << "-m" << message;
	if (!startProcess(arguments))
	{
		return false;
	}
	if (!waitForClient())
	{
		return false;
	}
	return processErrors();
}

bool ExternalClient::doAdd(const QString &what, bool force)
{
	if (!checkSvnClientPath())
	{
		return false;
	}
	QStringList arguments;
	arguments << "add" << what;
	if (force)
	{
		arguments << "--force";
	}
	if (!startProcess(arguments))
	{
		return false;
	}
	if (!waitForClient())
	{
		return false;
	}
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
	if (!startProcess(arguments))
	{
		return false;
	}
	if (!waitForClient())
	{
		return false;
	}
	return processErrors();
}

bool ExternalClient::doCleanUp(const QString &what)
{
	QStringList arguments;
	arguments << "cleanup" << what;
	if (!startProcess(arguments))
	{
		return false;
	}
	if (!waitForClient())
	{
		return false;
	}
	return processErrors();
}

QString ExternalClient::info(const QString &workingDir)
{
	QStringList arguments;
	arguments << "info" << workingDir;
	if (!startProcess(arguments))
	{
		return "";
	}
	if (!waitForClient())
	{
		return false;
	}
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
		mErrors << "Can`t find repository root in svn info";
		return "";
	}
	repoInfo = repoInfo.mid(ind + QString("Repository Root: ").length());
	ind = repoInfo.indexOf("Repository UUID: ");
	if (ind == -1)
	{
		mErrors << "Can`t find repository UUID in svn info";
		return "";
	}
	repoInfo = repoInfo.mid(0,  ind-2);
	return repoInfo;
}

int ExternalClient::currentRevision(const QString &workingDir)
{
	if (!checkSvnClientPath())
	{
		return -1;
	}
	QString repoInfo = info(workingDir);
	int ind = repoInfo.indexOf("Revision: ");
	if (ind == -1)
	{
		mErrors << "Can`t find revision number in svn info";
		return -1;
	}
	ind += QString("Revision: ").length();
	QString revision = "";
	for (ind; repoInfo[ind] < '9' && repoInfo[ind] > '0'; ++ind)
	{
		revision += repoInfo[ind];
	}
	return revision.toInt();
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

bool ExternalClient::startProcess(const QStringList &args)
{
	if (!checkSvnClientPath())
	{
		return false;
	}
	mClientProcess->start(mPathToClient, args);
	if (!mClientProcess->waitForStarted())
	{
		mErrors << "An error occured while starting svn client process (maybe path is not correct?)";
		return false;
	}
	QCoreApplication::processEvents();
	return true;
}

bool ExternalClient::checkSvnClientPath()
{
	if (mPathToClient.isEmpty())
	{
		mErrors << "Path to svn client is empty";
		return false;
	}
	return true;
}

bool ExternalClient::processErrors()
{
	QByteArray error = mClientProcess->readAllStandardError();
	if (error.size() > 0)
		mErrors << QString(error);
	return error.size() == 0;
}

bool ExternalClient::waitForClient()
{
	QCoreApplication::processEvents();
	QSettings settings("SPbSU", "QReal");
	int timeout = settings.value("svnClientTimeout", 30000).toInt();
	QCoreApplication::processEvents();
	if (mClientProcess->state() == QProcess::Running)
	{
		if (!mClientProcess->waitForFinished(timeout))
		{
			mClientProcess->kill();
			mErrors << "Svn client timeout";
			processErrors();
			return false;
		}
	}
	return true;
}

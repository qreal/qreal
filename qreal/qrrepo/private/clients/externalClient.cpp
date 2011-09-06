#include "externalClient.h"

#include <QMessageBox>
#include <QTextCodec>
#include <QSettings>
#include <QCoreApplication>

using namespace qrRepo::details;

ExternalClient::ExternalClient(const QString &pathToClient)
	: mPathToClient(pathToClient)
{
	mClientProcess = new QProcess(NULL);
}

ExternalClient::ExternalClient(const ExternalClient &other)
	: mPathToClient(other.mPathToClient)
{
	mClientProcess = new QProcess(NULL);
}

QString ExternalClient::repoUrl(const QString &workingDir)
{
	QString repoInfo = info(workingDir);
	if (repoInfo == "")
	{
		return "";
	}
	return infoToRepoUrl(repoInfo);
}

int ExternalClient::currentRevision(const QString &workingDir)
{
	QString repoInfo = info(workingDir);
	if (repoInfo == "")
	{
		return -1;
	}
	return infoToRevision(repoInfo);
}

QStringList ExternalClient::newErrors()
{
	QStringList result(mErrors);
	mErrors.clear();
	return result;
}

bool ExternalClient::doOperation(const QStringList &args)
{
	if (!startProcess(args))
	{
		return false;
	}
	if (!waitForClient())
	{
		return false;
	}
	return processErrors();
}

QString ExternalClient::standartOutput()
{
	QString output = mClientProcess->readAllStandardOutput();
	return QString::fromLocal8Bit(output.toStdString().c_str());
}

bool ExternalClient::startProcess(const QStringList &args)
{
	if (!checkClientPath())
	{
		return false;
	}
	mClientProcess->start(mPathToClient, args);
	if (!mClientProcess->waitForStarted())
	{
		mErrors << "An error occured while starting versioning client process (maybe path is not correct?)";
		return false;
	}
	QCoreApplication::processEvents();
	return true;
}

bool ExternalClient::checkClientPath()
{
	if (mPathToClient.isEmpty())
	{
		mErrors << "Path to versioning client is empty";
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
	int timeout = settings.value("clientTimeout", 30000).toInt();
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

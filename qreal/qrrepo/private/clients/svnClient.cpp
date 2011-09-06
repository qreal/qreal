#include "svnClient.h"

using namespace qrRepo::details;

SvnClient::SvnClient(const QString &pathToClient)
	: ExternalClient(pathToClient)
{
}

bool SvnClient::doCheckout(const QString &from, const QString &to)
{
	QStringList arguments;
	arguments << "checkout" << from << to;
	return doOperation(arguments);
}

bool SvnClient::doUpdate(const QString &to)
{
	QStringList arguments;
	arguments << "update" << to;
	return doOperation(arguments);
}

bool SvnClient::doCommit(const QString &from, const QString &message)
{
	QStringList arguments;
	arguments << "commit" << from << "-m" << message;
	return doOperation(arguments);
}

bool SvnClient::doCleanUp(const QString &what)
{
	QStringList arguments;
	arguments << "cleanup" << what;
	return doOperation(arguments);
}

bool SvnClient::doAdd(const QString &what, bool force)
{
	QStringList arguments;
	arguments << "add" << what;
	if (force)
	{
		arguments << "--force";
	}
	return doOperation(arguments);
}

bool SvnClient::doRemove(const QString &what, bool force)
{
	QStringList arguments;
	arguments << "remove" << what;
	if (force)
	{
		arguments << "--force";
	}
	return doOperation(arguments);
}

QString SvnClient::info(const QString &workingDir)
{
	QStringList arguments;
	arguments << "info" << workingDir;
	if (!doOperation(arguments))
	{
		return "";
	}
	return standartOutput();
}

QString SvnClient::infoToRepoUrl(QString &repoInfo)
{
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

int SvnClient::infoToRevision(QString &repoInfo)
{
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

#pragma once

#include "externalClient.h"

namespace qrRepo
{
namespace details
{

class SvnClient : public ExternalClient
{
public:
	SvnClient(QString const &pathToClient);
	bool doCheckout(const QString &from, const QString &to);
	bool doUpdate(const QString &to);
	bool doCommit(const QString &from, const QString &message);
	bool doCleanUp(const QString &what);
	bool doAdd(const QString &what, bool force);
	bool doRemove(const QString &what, bool force);
	QString info(const QString &workingDir);

private:
	QString infoToRepoUrl(QString &repoInfo);
	int infoToRevision(QString &repoInfo);
};

}
}

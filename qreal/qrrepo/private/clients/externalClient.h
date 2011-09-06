#pragma once

#include "../../versionControlSystemInterface.h"
#include "../serializerVersioningInterface.h"

#include <QProcess>

namespace qrRepo
{

namespace details
{

class ExternalClient : public VersionControlSystemInterface, public SerializerVersioningInterface
{

public:
	ExternalClient(QString const &pathToClient);
	ExternalClient(ExternalClient const &other);
	void setPathToClient(QString const &pathToClient);
	virtual bool doCheckout(QString const &from, QString const &to) = 0;
	virtual bool doUpdate(QString const &to) = 0;
	virtual bool doCommit(QString const &from, QString const &message) = 0;
	virtual bool doCleanUp(QString const &what) = 0;
	virtual QString info(QString const &workingDir) = 0;
	QString repoUrl(QString const &workingDir);
	int currentRevision(QString const &workingDir);

	QStringList newErrors();

	virtual bool doAdd(QString const &what, bool force = true) = 0;
	virtual bool doRemove(QString const &what, bool force = true) = 0;

protected:
	QStringList mErrors;

	bool doOperation(QStringList const &args);
	QString standartOutput();
	virtual QString infoToRepoUrl(QString &info) = 0;
	virtual int infoToRevision(QString &info) = 0;

private:
	QString mPathToClient;
	QProcess *mClientProcess;

	bool startProcess(QStringList const &args);
	bool checkClientPath();
	bool processErrors();
	bool waitForClient();
};
}
}

#pragma once

#include <QString>

namespace qrRepo {

class VersionControlSystemInterface
{
public:
	virtual ~VersionControlSystemInterface() {};

	virtual bool doCheckout(QString const &from, QString const &to) = 0;
	virtual bool doCommit(QString const &from, QString const &message) = 0;
	virtual bool doUpdate(QString const &to) = 0;
	virtual bool doCleanUp(QString const &workingDir) = 0;
	virtual QString info(QString const &workingDir) = 0;
	virtual QString repoUrl(QString const &workingDir) = 0;
	virtual int currentRevision(QString const &workingDir) = 0;

	virtual QStringList newErrors() = 0;
};

}

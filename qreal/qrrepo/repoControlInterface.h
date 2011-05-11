#pragma once

#include "../qrgui/kernel/roles.h"

namespace qrRepo {

class RepoControlInterface
{
public:
	virtual ~RepoControlInterface() {};

	virtual void exterminate() = 0;

	virtual bool saveAll() = 0;
	virtual bool save(qReal::IdList list) = 0;
	virtual bool saveTo(QString const &workingDir) = 0;

	virtual void open(QString const &workingDir) = 0;

	virtual bool doCheckout(QString const &from, QString const &to) = 0;
	virtual bool doCommit(QString const &from, QString const &message) = 0;
	virtual bool doUpdate(QString const &to) = 0;
	virtual QString svnInfo(QString const &workingDir) = 0;

	virtual QStringList newErrors() = 0;
};

}

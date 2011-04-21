#pragma once

#include "../qrgui/kernel/roles.h"

namespace qrRepo {

class RepoControlInterface
{
public:
	virtual ~RepoControlInterface() {};

	virtual void exterminate() = 0;

	virtual void saveAll() = 0;
	virtual void save(qReal::IdList list) = 0;
	virtual void saveTo(QString const &workingDir) = 0;

	virtual void open(QString const &workingDir) = 0;

	virtual void doCheckout(QString const &from, QString const &to) = 0;
	virtual void doCommit(QString const &from) = 0;
	virtual void doUpdate(QString const &to) = 0;
};

}

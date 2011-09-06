#pragma once

#include "../qrgui/kernel/roles.h"
#include "versionControlSystemInterface.h"

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
};

}

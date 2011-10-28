#pragma once

#include "../qrkernel/roles.h"

namespace qrRepo {

class RepoControlInterface
{
public:
	virtual ~RepoControlInterface() {}

	virtual void exterminate() = 0;

	virtual void saveAll() const = 0;
	virtual void save(qReal::IdList list) const = 0;
	virtual void saveTo(QString const &workingDir) = 0;

	virtual void open(QString const &workingDir) = 0;
};

}

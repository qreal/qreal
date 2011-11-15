#pragma once

#include "../qrkernel/roles.h"

namespace qrRepo {

class RepoControlInterface
{
public:
	virtual ~RepoControlInterface() {}

	virtual void exterminate() = 0;

	///virtual, for import *.qrs file into current project
	///@param importedFile - file to be imported
	virtual void importFromDisk(QString const &importedFile) = 0;
	virtual void saveAll() const = 0;
	virtual void save(qReal::IdList list) const = 0;
	virtual void saveTo(QString const &workingDir) = 0;

	virtual void open(QString const &workingDir) = 0;
};

}

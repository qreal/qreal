#pragma once

#include "../qrgui/kernel/roles.h"

namespace qrRepo {

class RepoControlInterface
{
public:
	virtual ~RepoControlInterface() {};

	virtual void exterminate() = 0;

	virtual void saveAll() const = 0;
	virtual void save(qReal::IdList list) const = 0;
	virtual void saveTo(QString const &workingDir) = 0;
	virtual void remove(qReal::IdList list) const = 0;

	virtual void open(QString const &workingDir) = 0;

	virtual qReal::IdList getOpenedDiagrams() const = 0;
	virtual qReal::IdList getChangedDiagrams() const = 0;
	virtual void resetChangedDiagrams() = 0;
	virtual void addOpenedDiagram(const qReal::Id &id) = 0;
	virtual void addChangedDiagram(const qReal::Id &id) = 0;
	virtual void resetChangedDiagrams(const qReal::IdList &list) = 0;

};
}

#pragma once

#include "../qrgui/kernel/roles.h"

namespace qrRepo {

class CommonRepoApi
{
public:
	virtual ~CommonRepoApi(){}

	virtual void setName(qReal::Id const &id, QString const &name) = 0;
	virtual QString name(qReal::Id const &id) const = 0;
	virtual qReal::IdList children(qReal::Id const &id) const = 0;
	virtual void removeChild(qReal::Id const &id, qReal::Id const &child) = 0;
	virtual void removeChildren(qReal::Id const &id) = 0;

	virtual bool exist(qReal::Id const &id) const = 0;
	virtual void removeElement(qReal::Id const &id) = 0;

	virtual qReal::Id from(qReal::Id const &id) const = 0;
	virtual qReal::Id to(qReal::Id const &id) const = 0;

	virtual void setFrom(qReal::Id const &id, qReal::Id const &from) = 0;
	virtual void setTo(qReal::Id const &id, qReal::Id const &to) = 0;

	virtual qReal::IdList parents(qReal::Id const &id) const = 0;
	virtual void addParent(qReal::Id const &id, qReal::Id const &parent) = 0;
	virtual void removeParent(qReal::Id const &id, qReal::Id const &parent) = 0;

	virtual QString typeName(qReal::Id const &id) const = 0;
};

}

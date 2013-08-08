#pragma once

#include "object.h"

namespace qrRepo {
namespace details {

class LogicalObject : public Object
{
public:
	explicit LogicalObject(qReal::Id const &id);
	LogicalObject(const qReal::Id &id, const qReal::Id &parent);

	virtual bool isLogicalObject() const;

protected:
	virtual Object *createClone() const;

private:
};

}

}

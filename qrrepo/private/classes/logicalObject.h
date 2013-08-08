#pragma once

#include "object.h"

namespace qrRepo {
namespace details {

class LogicalObject : public Object
{
public:
	explicit LogicalObject(qReal::Id const &id);
	explicit LogicalObject(QDomElement const &element);

	virtual bool isLogicalObject() const;

protected:
	virtual Object *createClone() const;

private:
};

}

}

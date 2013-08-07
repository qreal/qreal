#pragma once

#include "object.h"

namespace qrRepo {
namespace details {

class LogicalObject : public Object
{
public:
	explicit LogicalObject(qReal::Id const &id);

	virtual void f() {}

private:
};

}

}

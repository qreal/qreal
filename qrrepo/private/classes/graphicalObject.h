#pragma once

#include "object.h"

namespace qrRepo {
namespace details {

class GraphicalObject : public Object
{
public:
	explicit GraphicalObject(qReal::Id const &id);

	virtual void f() {}

private:
};

}

}

#pragma once

#include <qrkernel/ids.h>

namespace qReal {
namespace models {
namespace details {
namespace modelsImplementation {

class GraphicalPartModelItem
{
public:
	GraphicalPartModelItem(Id const &id, int index);
	Id id() const;
	int index() const;

private:
	Id const mId;
	int const mIndex;
};

}
}
}
}

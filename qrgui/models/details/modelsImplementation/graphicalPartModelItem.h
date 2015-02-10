#pragma once

#include <qrkernel/ids.h>

namespace qReal {
namespace models {
namespace details {
namespace modelsImplementation {

class GraphicalPartModelItem
{
public:
	GraphicalPartModelItem(const Id &id, int index);
	Id id() const;
	int index() const;

private:
	const Id mId;
	const int mIndex;
};

}
}
}
}

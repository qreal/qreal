#pragma once

#include "nonZoneNode.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

/// A base for all nodes with condition (like ifs and loops)
class ConditionalNode : public NonZoneNode
{
public:
	explicit ConditionalNode(Id const &idBinded, QObject *parent = 0);

	/// Remembers that binded condition must be inverted in the resulting code.
	/// Calling this method even number of times leaves condition without changes.
	void invertCondition();

protected:
	bool mAddNotToCondition;
};

}
}
}
}

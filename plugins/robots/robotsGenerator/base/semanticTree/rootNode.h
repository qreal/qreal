#pragma once

#include "zoneNode.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

/// A root node for all semantic tree
class RootNode : public SemanticNode
{
public:
	explicit RootNode(Id const &initialBlock, QObject *parent = 0);

	virtual QString toString(GeneratorCustomizer &customizer, int indent) const;

protected:
	virtual QLinkedList<SemanticNode *> children() const;

private:
	ZoneNode *mZone;  // Takes ownership
};

}
}
}
}

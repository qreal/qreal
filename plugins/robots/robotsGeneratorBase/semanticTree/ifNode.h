#pragma once

#include "conditionalNode.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

/// A semantic node for the block with if semantics
class IfNode : public ConditionalNode
{
public:
	explicit IfNode(Id const &idBinded, QObject *parent = 0);

	ZoneNode *parentZone() const;

	ZoneNode *thenZone();
	ZoneNode *elseZone();


protected:
	QLinkedList<SemanticNode *> children() const override;
	QString toStringImpl(GeneratorCustomizer &customizer, int indent) const override;

private:
	ZoneNode *mThenZone;  // Takes ownership
	ZoneNode *mElseZone;  // Takes ownership
};

}
}
}
}

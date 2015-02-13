#pragma once

#include "conditionalNode.h"

namespace generatorBase {
namespace semantics {

/// A semantic node for the block with if semantics
class IfNode : public ConditionalNode
{
public:
	explicit IfNode(const qReal::Id &idBinded, QObject *parent = 0);

	ZoneNode *thenZone();
	ZoneNode *elseZone();

	/// Will be called when both branches link to same block, making thus if statement unnesesary.
	void transformToSimple();

protected:
	QLinkedList<SemanticNode *> children() const override;
	QString toStringImpl(GeneratorCustomizer &customizer, int indent, const QString &indentString) const override;

private:
	ZoneNode *mThenZone;  // Takes ownership
	ZoneNode *mElseZone;  // Takes ownership
	bool mIsSimple;
};

}
}

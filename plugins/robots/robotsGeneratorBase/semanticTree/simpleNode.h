#pragma once

#include "nonZoneNode.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

/// A semantic node for the block with simple semantics
class SimpleNode : public NonZoneNode
{
public:
	/// Represents all options that may be added by control flow generator
	/// artificially for more smart generation
	enum SyntheticBlockType
	{
		noSytheticBinding = 0
		, breakNode
		, continueNode
		, gotoNode
	};

	explicit SimpleNode(Id const &idBinded, QObject *parent = 0);

	/// Binds this block to given artificial construction instead of binding to id.
	void bindToSyntheticConstruction(SyntheticBlockType type);

protected:
	QLinkedList<SemanticNode *> children() const override;

	QString toStringImpl(GeneratorCustomizer &customizer, int indent) const override;

private:
	SyntheticBlockType mSyntheticBinding;
};

}
}
}
}

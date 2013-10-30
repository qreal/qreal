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
	};

	explicit SimpleNode(Id const &idBinded, QObject *parent = 0);

	virtual QString toString(GeneratorCustomizer &customizer, int indent) const;

	/// Binds this block to given artificial construction instead of binding to id.
	void bindToSyntheticConstruction(SyntheticBlockType type);

protected:
	virtual QLinkedList<SemanticNode *> children() const;

private:
	SyntheticBlockType mSyntheticBinding;
};

}
}
}
}

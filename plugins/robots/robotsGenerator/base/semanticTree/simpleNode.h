#pragma once

#include "nonZoneNode.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

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

	SimpleNode(Id const &idBinded, QObject *parent = 0);

	virtual QString toString(GeneratorCustomizer &customizer) const;

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

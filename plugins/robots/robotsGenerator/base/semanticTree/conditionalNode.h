#pragma once

#include "nonZoneNode.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

class ConditionalNode : public NonZoneNode
{
public:
	ConditionalNode(Id const &idBinded, QObject *parent = 0);

	void invertCondition();

	virtual QString toString(GeneratorCustomizer &customizer) const;

protected:
	bool mAddNotToCondition;
};

}
}
}
}

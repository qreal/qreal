#pragma once

#include "nonZoneNode.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

class FinalNode : public NonZoneNode
{
public:
	FinalNode(Id const &idBinded, bool inMainDigram, QObject *parent = 0);

	virtual QString toString(GeneratorCustomizer &customizer, int indent) const;

protected:
	virtual QLinkedList<SemanticNode *> children() const;

private:
	bool const mInMainDiagram;
};

}
}
}
}

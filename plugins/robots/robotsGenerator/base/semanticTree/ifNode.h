#pragma once

#include "nonZoneNode.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

class IfNode : public NonZoneNode
{
public:
	IfNode(Id const &idBinded, QObject *parent = 0);

	ZoneNode *parentZone();

	virtual QString toString() const;

	void appendThenBranch(SemanticNode *node);
	void appendElseBranch(SemanticNode *node);

protected:
	virtual QLinkedList<SemanticNode *> children() const;

private:
	ZoneNode *mThenZone;  // Takes ownership
	ZoneNode *mElseZone;  // Takes ownership
};

}
}
}
}

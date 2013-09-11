#pragma once

#include <QtCore/QObject>

#include "rootNode.h"
#include "simpleNode.h"
#include "ifNode.h"
#include "loopNode.h"
#include "finalNode.h"
#include "../generatorCustomizer.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

class SemanticTree : public QObject
{
public:
	SemanticTree(GeneratorCustomizer &customizer, Id const &initialBlock
			, bool isMainTree, QObject *parent = 0);

	QString toString(int indent) const;

	SemanticNode *produceNodeFor(Id const &id);

	SimpleNode *produceSimple(Id const &id = Id());
	IfNode *produceConditional(Id const &id = Id());
	LoopNode *produceLoop(Id const &id = Id());
	FinalNode *produceFinal(Id const &id = Id());

	NonZoneNode *findNodeFor(Id const &id);

private:
	GeneratorCustomizer &mCustomizer;
	bool const mIsMainTree;
	RootNode *mRoot;  // Takes ownership
};

}
}
}
}

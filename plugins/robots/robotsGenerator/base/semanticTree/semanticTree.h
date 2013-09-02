#pragma once

#include <QtCore/QObject>

#include "rootNode.h"
#include "simpleNode.h"
#include "ifNode.h"
#include "loopNode.h"
#include "../generatorCustomizer.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

class SemanticTree : public QObject
{
public:
	class VisitorInterface
	{
	public:
		virtual ~VisitorInterface();

		virtual void visitRoot(RootNode *node) = 0;
		virtual void visitSimple(SimpleNode *node) = 0;
		virtual void visitConditional(ConditionalNode *node) = 0;
		virtual void visitLoop(LoopNode *node) = 0;
	};

	SemanticTree(GeneratorCustomizer &customizer, Id const &initialBlock
			, QObject *parent = 0);

	void debugPrint();

	SemanticNode *produceNodeFor(Id const &id);

	SimpleNode *produceSimple(Id const &id = Id());
	IfNode *produceConditional(Id const &id = Id());
	LoopNode *produceLoop(Id const &id = Id());

	NonZoneNode *findNodeFor(Id const &id);

private:
	GeneratorCustomizer &mCustomizer;
	RootNode *mRoot;  // Takes ownership
};

}
}
}
}

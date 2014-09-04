#pragma once

#include <QtCore/QObject>

#include "rootNode.h"
#include "simpleNode.h"
#include "ifNode.h"
#include "loopNode.h"
#include "finalNode.h"
#include "forkNode.h"
#include "generatorBase/generatorCustomizer.h"

namespace generatorBase {
namespace semantics {

/// A tree representing control flow of some language. Consists of semantic nodes.
/// Also provides some factory methods for nodes.
class SemanticTree : public QObject
{
public:
	SemanticTree(GeneratorCustomizer &customizer, qReal::Id const &initialBlock
			, bool isMainTree, QObject *parent = 0);

	/// Returns an id of the initial node in this semantic tree. Semantic trees are
	/// equal if their root ids are equal.
	qReal::Id initialBlock() const;

	/// Generates code by this tree. Target language is defined with customizer
	/// that was passed into constructor.
	QString toString(int indent) const;

	/// Produces new instance of semantic node binded to specified block
	/// autodetecting block`s semantics
	SemanticNode *produceNodeFor(qReal::Id const &id);

	/// Produces new instance of simple node binded to specified block
	SimpleNode *produceSimple(qReal::Id const &id = qReal::Id());

	/// Produces new instance of if node binded to specified block
	IfNode *produceConditional(qReal::Id const &id = qReal::Id());

	/// Produces new instance of loop node binded to specified block
	LoopNode *produceLoop(qReal::Id const &id = qReal::Id());

	/// Produces new instance of fork node binded to specified block
	ForkNode *produceFork(qReal::Id const &id = qReal::Id());

	/// Produces new instance of final node binded to specified block
	FinalNode *produceFinal(qReal::Id const &id = qReal::Id());

	/// Performs deep (recursive) search in this tree and returns
	/// a node with specified id binded if such was found or NULL otherwise.
	NonZoneNode *findNodeFor(qReal::Id const &id);

private:
	GeneratorCustomizer &mCustomizer;
	bool const mIsMainTree;
	RootNode *mRoot;  // Takes ownership
};

}
}

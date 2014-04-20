#pragma once

#include <QtCore/QObject>

#include <qrutils/graphUtils/deepFirstSearcher.h>
#include "generatorBase/semanticTree/semanticTree.h"

namespace generatorBase {
namespace semantics {

/// A base for all commands that perform semantic tree transformations.
class SemanticTransformationRule
{
public:
	typedef utils::DeepFirstSearcher::LinkInfo LinkInfo;

	/// Returns true if rule has been applied successfully; otherwise returns false
	virtual bool apply() = 0;

protected:
	SemanticTransformationRule(SemanticTree *tree, qReal::Id const &id);
	virtual ~SemanticTransformationRule();

	LoopNode *makeLoopStartingFrom(NonZoneNode *node);

	/// Returns true if semantic node for the given id was already created.
	bool alreadyCreated(qReal::Id const &id);

	/// Returns true if semantic node for the link dst node was already created.
	bool alreadyCreated(LinkInfo const &link);

	SemanticTree *mTree;  // Does not take ownership
	qReal::Id const mId;
};

}
}

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
	SemanticTransformationRule(SemanticTree *tree, const qReal::Id &id);
	virtual ~SemanticTransformationRule();

	LoopNode *makeLoopStartingFrom(NonZoneNode *node);

	/// Returns true if semantic node for the given id was already created.
	bool alreadyCreated(const qReal::Id &id);

	/// Returns true if semantic node for the link dst node was already created.
	bool alreadyCreated(const LinkInfo &link);

	SemanticTree *mTree;  // Does not take ownership
	const qReal::Id mId;
};

}
}

#pragma once

#include <QtCore/QObject>

#include <qrutils/graphUtils/deepFirstSearcher.h>
#include "../semanticTree/semanticTree.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

class SemanticTransformationRule
{
public:
	typedef utils::DeepFirstSearcher::LinkInfo LinkInfo;

	/// Returns true if rule has been applied successfully; otherwise returns false
	virtual bool apply() = 0;

protected:
	SemanticTransformationRule(SemanticTree *tree, Id const &id);
	virtual ~SemanticTransformationRule();

	LoopNode *makeLoopStartingFrom(NonZoneNode *node);

	SemanticTree *mTree;  // Does not take ownership
	Id const mId;
};

}
}
}
}

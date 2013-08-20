#pragma once

#include <QtCore/QObject>

#include <graphUtils/deepFirstSearcher.h>
#include "../semanticTree/semanticTree.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

class SemanticTransformationRule : public QObject
{
	Q_OBJECT

public:
	typedef utils::DeepFirstSearcher::LinkInfo LinkInfo;

	virtual bool canApply() = 0;
	virtual bool apply() = 0;

signals:
	SemanticTransformationRule(SemanticTree *tree, Id const &id, QObject *parent = 0);
	virtual ~SemanticTransformationRule();

	void errorOccured();

protected:
	SemanticTree *mTree;  // Does not take ownership
	Id const mId;
};

}
}
}
}

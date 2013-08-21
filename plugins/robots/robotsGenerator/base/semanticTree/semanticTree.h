#pragma once

#include <QtCore/QObject>

#include "rootNode.h"
#include "../generatorCustomizer.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

class SemanticTree : public QObject
{
public:
	SemanticTree(GeneratorCustomizer const &customizer, Id const &initialBlock
			, QObject *parent = 0);

	void debugPrint();

	SemanticNode *produceNodeFor(Id const &id);
	SemanticNode *findNodeFor(Id const &id);

private:
	GeneratorCustomizer const &mCustomizer;
	RootNode *mRoot;  // Takes ownership
};

}
}
}
}

#pragma once

#include <QtCore/QObject>

#include "rootNode.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

class SemanticTree : public QObject
{
public:
	SemanticTree(Id const &initialBlock, QObject *parent = 0);
	void debugPrint();

private:
	RootNode *mRoot;  // Takes ownership
};

}
}
}
}

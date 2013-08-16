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
	explicit SemanticTree(QObject *parent = 0);

private:
	RootNode *mRoot; // Takes ownership
};

}
}
}
}

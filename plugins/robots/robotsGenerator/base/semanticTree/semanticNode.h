#pragma once

#include <QtCore/QObject>
#include <QtCore/QLinkedList>

#include <ids.h>
#include "../generatorCustomizer.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

class SemanticNode : public QObject
{
public:
	Id id() const;
	void bindTo(Id const &id);

	void setParentNode(SemanticNode *parent);

	virtual QString toString(GeneratorCustomizer &customizer) const = 0;

	void debugPrint(GeneratorCustomizer &customizer, int indent);

	SemanticNode *findNodeFor(Id const &id);

protected:
	SemanticNode(Id const &idBinded = Id(), QObject *parent = 0);

	virtual QLinkedList<SemanticNode *> children() const = 0;

	Id mId;
	SemanticNode *mParentNode;
};

}
}
}
}

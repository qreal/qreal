#pragma once

#include <QtCore/QObject>
#include <QtCore/QLinkedList>

#include <ids.h>

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

class SemanticNode : public QObject
{
public:
	void bindTo(Id const &id);
	void setParentNode(SemanticNode *parent);

	virtual QString toString() const = 0;

	void debugPrint(int indent);

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

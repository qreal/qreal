#include "semanticNode.h"

using namespace qReal::robots::generators::semantics;

SemanticNode::SemanticNode(Id const &idBinded, QObject *parent)
	: QObject(parent)
	, mId(idBinded)
	, mParentNode(NULL)
{
}

qReal::Id SemanticNode::id() const
{
	return mId;
}

void SemanticNode::bindTo(qReal::Id const &id)
{
	mId = id;
}

void SemanticNode::setParentNode(SemanticNode * const parent)
{
	mParentNode = parent;
}

SemanticNode *SemanticNode::findNodeFor(qReal::Id const &id)
{
	if (id == mId) {
		return this;
	}

	QLinkedList<SemanticNode *> const children = this->children();
	foreach (SemanticNode * const child, children) {
		SemanticNode * const searchResult = child->findNodeFor(id);
		if (searchResult) {
			return searchResult;
		}
	}

	return NULL;
}

#include "generatorBase/semanticTree/zoneNode.h"

#include <qrutils/stringUtils.h>

using namespace generatorBase::semantics;
using namespace qReal;

ZoneNode::ZoneNode(QObject *parent)
	: SemanticNode(Id(), parent)
{
}

bool ZoneNode::isEmpty() const
{
	return mChildren.isEmpty();
}

void ZoneNode::appendChild(SemanticNode *node)
{
	mChildren << node;
	node->setParentNode(this);
}

void ZoneNode::appendChildren(QLinkedList<SemanticNode *> const &nodes)
{
	foreach (SemanticNode * const node, nodes) {
		appendChild(node);
	}
}

void ZoneNode::insertAfrer(SemanticNode *after, SemanticNode *node)
{
	mChildren.insert(qFind(mChildren.begin(), mChildren.end(), after) + 1, node);
	node->setParentNode(this);
}

void ZoneNode::removeChild(SemanticNode *node)
{
	mChildren.removeOne(node);
	node->setParentNode(nullptr);
}

SemanticNode *ZoneNode::removeLast()
{
	SemanticNode * const result = mChildren.last();
	mChildren.removeLast();
	result->setParentNode(nullptr);
	return result;
}

QLinkedList<SemanticNode *> ZoneNode::removeStartingFrom(SemanticNode *node)
{
	QLinkedList<SemanticNode *> result;
	bool foundNode = node == nullptr;

	foreach (SemanticNode * const current, mChildren) {
		if (!foundNode) {
			foundNode = current == node;
		}

		if (foundNode) {
			current->setParentNode(nullptr);
			mChildren.removeOne(current);
			result << current;
		}
	}

	return result;
}

QString ZoneNode::toStringImpl(GeneratorCustomizer &customizer, int indent, const QString &indentString) const
{
	QString result;
	for (const SemanticNode * const child : mChildren) {
		result += utils::StringUtils::addIndent(child->toString(customizer, 0, indentString).trimmed()
				, indent, indentString) + "\n";
	}

	result.chop(1);
	return result;
}

SemanticNode *ZoneNode::parentNode()
{
	return mParentNode;
}

SemanticNode *ZoneNode::nextChild(SemanticNode *child)
{
	QLinkedListIterator<SemanticNode *> iterator(mChildren);
	return iterator.findNext(child) && iterator.hasNext() ? iterator.next() : nullptr;
}

QLinkedList<SemanticNode *> ZoneNode::children() const
{
	return mChildren;
}

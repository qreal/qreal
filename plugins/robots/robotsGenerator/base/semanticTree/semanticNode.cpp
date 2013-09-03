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

QString SemanticNode::addIndent(QString const &code, int indent) const
{
	if (!indent) {
		return code;
	}

	QStringList const lines = code.split("\n", QString::SkipEmptyParts);
	QString const indentString(indent, '\t');
	QStringList result;
	foreach (QString const &line, lines) {
		result << indentString + line;
	}

	return result.join('\n');
}

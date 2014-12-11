#include "generatorBase/semanticTree/ifNode.h"

#include <qrutils/stringUtils.h>

using namespace generatorBase::semantics;
using namespace qReal;

IfNode::IfNode(Id const &idBinded, QObject *parent)
	: ConditionalNode(idBinded, parent)
	, mThenZone(new ZoneNode(this))
	, mElseZone(new ZoneNode(this))
	, mIsSimple(false)
{
	mThenZone->setParentNode(this);
	mElseZone->setParentNode(this);
}

ZoneNode *IfNode::thenZone()
{
	return mThenZone;
}

ZoneNode *IfNode::elseZone()
{
	return mElseZone;
}

void IfNode::transformToSimple()
{
	mIsSimple = true;
}

QString IfNode::toStringImpl(GeneratorCustomizer &customizer, int indent, QString const &indentString) const
{
	if (mIsSimple) {
		return mThenZone->toString(customizer, indent, indentString);
	}

	if (mThenZone->isEmpty() && mElseZone->isEmpty()) {
		return QString();
	}

	bool const elseIsEmpty = mElseZone->isEmpty();
	QString result = utils::StringUtils::addIndent(customizer.factory()->
			ifGenerator(mId, customizer, elseIsEmpty, mAddNotToCondition)->generate(), indent, indentString);

	QString const thenBlock = mThenZone->toString(customizer, indent + 1, indentString);
	QString const elseBlock = mElseZone->toString(customizer, indent + 1, indentString);

	result.replace("@@THEN_BODY@@", thenBlock);
	result.replace("@@ELSE_BODY@@", elseBlock);
	return result;
}

QLinkedList<SemanticNode *> IfNode::children() const
{
	return QLinkedList<SemanticNode *>() << mThenZone << mElseZone;
}

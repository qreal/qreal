#include "intermediateNode.h"

#include <QQueue>

using namespace generatorBase;
using namespace myUtils;

SimpleNode::SimpleNode(const qReal::Id &id, QObject *parent)
	: IntermediateNode(parent)
	, mId(id)
{
	mIdsInvolved = { id };
}

IntermediateNode::Type SimpleNode::type() const
{
	return Type::simple;
}

qReal::Id SimpleNode::id() const
{
	return mId;
}

IfNode::IfNode(SimpleNode *condition
							, IntermediateNode *thenBranch
							, IntermediateNode *elseBranch
							, QObject *parent)
	: IntermediateNode(parent)
	, mCondition(condition)
	, mThenBranch(thenBranch)
	, mElseBranch(elseBranch)
	, mIsIfThenForm(elseBranch == nullptr)
{
	mIdsInvolved = mCondition->ids() + mThenBranch->ids();

	if (mElseBranch) {
		mIdsInvolved += mElseBranch->ids();
	}
}

SimpleNode *IfNode::condition() const
{
	return mCondition;
}

IntermediateNode *IfNode::thenBranch() const
{
	return mThenBranch;
}

IntermediateNode *IfNode::elseBranch() const
{
	return mElseBranch;
}

IntermediateNode::Type IfNode::type() const
{
	return Type::ifThenElseCondition;
}

IntermediateNode::IntermediateNode(QObject *parent)
	: QObject(parent)
	, mIdsInvolved(QSet<qReal::Id>())
{
}

IntermediateNode::IntermediateNode(QSet<qReal::Id> &ids, QObject *parent)
	: QObject(parent)
	, mIdsInvolved(ids)
{
}

QSet<qReal::Id> IntermediateNode::ids() const
{
	return mIdsInvolved;
}

SwitchNode::SwitchNode(SimpleNode *condition, const QList<IntermediateNode *> &branches, QObject *parent)
	: IntermediateNode(parent)
	, mCondition(condition)
	, mBranches(QList<IntermediateNode *>(branches))
{
	mIdsInvolved = {};
	for (const IntermediateNode * node : branches) {
		mIdsInvolved.unite(node->ids());
	}
}

SimpleNode *SwitchNode::condition() const
{
	return mCondition;
}

QList<IntermediateNode *> SwitchNode::branches() const
{
	return mBranches;
}

IntermediateNode::Type SwitchNode::type() const
{
	return Type::switchCondition;
}

BlockNode::BlockNode(IntermediateNode *firstNode, IntermediateNode *secondNode, QObject *parent)
	: IntermediateNode(parent)
	, mFirstNode(firstNode)
	, mSecondNode(secondNode)
{
	mIdsInvolved = firstNode->ids() + secondNode->ids();
}

IntermediateNode *BlockNode::firstNode() const
{
	return mFirstNode;
}

IntermediateNode *BlockNode::secondNode() const
{
	return mSecondNode;
}

IntermediateNode::Type BlockNode::type() const
{
	return Type::block;
}

WhileNode::WhileNode(IntermediateNode *headNode, IntermediateNode *bodyNode, QObject *parent)
	: IntermediateNode(parent)
	, mHeadNode(headNode)
	, mBodyNode(bodyNode)
{
	mIdsInvolved = mHeadNode->ids() + mBodyNode->ids();
}

IntermediateNode::Type WhileNode::type() const
{
	return Type::whileloop;
}

SelfLoopNode::SelfLoopNode(IntermediateNode *bodyNode, QObject *parent)
	: IntermediateNode(parent)
	, mBodyNode(bodyNode)
{
	mIdsInvolved = bodyNode->ids();
}

IntermediateNode::Type SelfLoopNode::type() const
{
	return Type::infiniteloop;
}

IfWithBreakNode::IfWithBreakNode(SimpleNode *condition, IntermediateNode *actionsBeforeBreak, QObject *parent)
	: IntermediateNode(parent)
	, mCondition(condition)
	, mActionsBeforeBreak(actionsBeforeBreak)
{
	mIdsInvolved = mCondition->ids();
	if (mActionsBeforeBreak) {
		mIdsInvolved += mActionsBeforeBreak->ids();
	}
}

SimpleNode *IfWithBreakNode::condition() const
{
	return mCondition;
}

IntermediateNode *IfWithBreakNode::actionsBeforeBreak() const
{
	return mActionsBeforeBreak;
}

IntermediateNode::Type IfWithBreakNode::type() const
{
	return Type::ifWithBreakCondition;
}

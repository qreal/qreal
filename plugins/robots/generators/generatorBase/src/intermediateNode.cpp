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

qReal::Id SimpleNode::firstId() const
{
	return mId;
}

bool SimpleNode::analyzeBreak()
{
	mHasBreakInside = false;
	return mHasBreakInside;
}

qReal::Id SimpleNode::id() const
{
	return mId;
}

IfNode::IfNode(IntermediateNode *condition
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

IntermediateNode *IfNode::condition() const
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

bool IfNode::analyzeBreak()
{
	mHasBreakInside = mThenBranch->analyzeBreak();

	if (mElseBranch) {
		mHasBreakInside |= mElseBranch->analyzeBreak();
	}

	return mHasBreakInside;
}

IntermediateNode::Type IfNode::type() const
{
	return Type::ifThenElseCondition;
}

qReal::Id IfNode::firstId() const
{
	return mCondition->firstId();
}

IntermediateNode::IntermediateNode(QObject *parent)
	: QObject(parent)
	, mIdsInvolved(QSet<qReal::Id>())
	, mIsInsideCycle(false)
	, mIsInsideSwitch(false)
	, mHasBreakInside(false)
	, mBreakWasAnalyzed(false)
{
}

IntermediateNode::IntermediateNode(QSet<qReal::Id> &ids, QObject *parent)
	: QObject(parent)
	, mIdsInvolved(ids)
	, mIsInsideCycle(false)
	, mIsInsideSwitch(false)
	, mHasBreakInside(false)
{
}

//bool IntermediateNode::analyzeBreak()
//{
//	if (mBreakWasAnalyzed) {
//		return mHasBreakInside;
//	}

//	mBreakWasAnalyzed = true;
//	return analyzeBreak();
//}

bool IntermediateNode::hasBreakInside() const
{
	return mHasBreakInside;
}

QSet<qReal::Id> IntermediateNode::ids() const
{
	return mIdsInvolved;
}

SwitchNode::SwitchNode(IntermediateNode *condition, const QList<IntermediateNode *> &branches, QObject *parent)
	: IntermediateNode(parent)
	, mCondition(condition)
	, mBranches(QList<IntermediateNode *>(branches))
{
	mIdsInvolved = {};
	for (const IntermediateNode * node : branches) {
		mIdsInvolved.unite(node->ids());
	}
}

IntermediateNode *SwitchNode::condition() const
{
	return mCondition;
}

QList<IntermediateNode *> SwitchNode::branches() const
{
	return mBranches;
}

bool SwitchNode::analyzeBreak()
{
	if (mBreakWasAnalyzed) {
		return mHasBreakInside;
	}

	mHasBreakInside = false;
	for (IntermediateNode *node : mBranches) {
		mHasBreakInside |= node->analyzeBreak();
	}
	mBreakWasAnalyzed = true;

	return mHasBreakInside;
}

IntermediateNode::Type SwitchNode::type() const
{
	return Type::switchCondition;
}

qReal::Id SwitchNode::firstId() const
{
	return mCondition->firstId();
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

bool BlockNode::analyzeBreak()
{
	if (mBreakWasAnalyzed) {
		return mHasBreakInside;
	}

	mHasBreakInside = mFirstNode->analyzeBreak() || mSecondNode->analyzeBreak();
	mBreakWasAnalyzed = true;
	return mHasBreakInside;
}

IntermediateNode::Type BlockNode::type() const
{
	return Type::block;
}

qReal::Id BlockNode::firstId() const
{
	return firstNode()->firstId();
}

WhileNode::WhileNode(IntermediateNode *headNode, IntermediateNode *bodyNode, IntermediateNode *exitNode, QObject *parent)
	: IntermediateNode(parent)
	, mHeadNode(headNode)
	, mBodyNode(bodyNode)
	, mExitNode(exitNode)
{
	mIdsInvolved = mHeadNode->ids() + mBodyNode->ids();
}

IntermediateNode *WhileNode::headNode() const
{
	return mHeadNode;
}

IntermediateNode *WhileNode::bodyNode() const
{
	return mBodyNode;
}

IntermediateNode *WhileNode::exitNode() const
{
	return mExitNode;
}

bool WhileNode::analyzeBreak()
{
	if (mBreakWasAnalyzed) {
		return mHasBreakInside;
	}

	mHasBreakInside = mHeadNode->analyzeBreak() || mBodyNode->analyzeBreak();
	mBreakWasAnalyzed = true;
	return mHasBreakInside;
}

IntermediateNode::Type WhileNode::type() const
{
	return Type::whileloop;
}

qReal::Id WhileNode::firstId() const
{
	return mHeadNode->firstId();
}

SelfLoopNode::SelfLoopNode(IntermediateNode *bodyNode, QObject *parent)
	: IntermediateNode(parent)
	, mBodyNode(bodyNode)
{
	mIdsInvolved = bodyNode->ids();
}

IntermediateNode *SelfLoopNode::bodyNode() const
{
	return mBodyNode;
}

bool SelfLoopNode::analyzeBreak()
{
	if (mBreakWasAnalyzed) {
		return mHasBreakInside;
	}

	mHasBreakInside = mBodyNode->analyzeBreak();
	mBreakWasAnalyzed = true;
	return mHasBreakInside;
}

IntermediateNode::Type SelfLoopNode::type() const
{
	return Type::infiniteloop;
}

qReal::Id SelfLoopNode::firstId() const
{
	return mBodyNode->firstId();
}

BreakNode::BreakNode(const qReal::Id &id, QObject *parent)
	: IntermediateNode(parent)
	, mId(id)
{
}

IntermediateNode::Type BreakNode::type() const
{
	return breakNode;
}

qReal::Id BreakNode::firstId() const
{
	return mId;
}

bool BreakNode::analyzeBreak()
{
	mHasBreakInside = true;
	mBreakWasAnalyzed = true;
	return mHasBreakInside;
}

FakeCycleHeadNode::FakeCycleHeadNode(const qReal::Id &id, QObject *parent)
	: IntermediateNode(parent)
	, mId(id)
{
}

IntermediateNode::Type FakeCycleHeadNode::type() const
{
	return fakeCycleHead;
}

qReal::Id FakeCycleHeadNode::firstId() const
{
	return mId;
}

bool FakeCycleHeadNode::analyzeBreak()
{
	mHasBreakInside = false;
	mBreakWasAnalyzed = true;
	return mHasBreakInside;
}

NodeWithBreaks::NodeWithBreaks(IntermediateNode *condition, QList<IntermediateNode *> &exitBranches, QObject *parent)
	: IntermediateNode(parent)
	, mCondition(condition)
	, mExitBranches(exitBranches)
{
}

IntermediateNode *NodeWithBreaks::condition() const
{
	return mCondition;
}

QList<IntermediateNode *> NodeWithBreaks::exitBranches() const
{
	return mExitBranches;
}

QList<IntermediateNode *> NodeWithBreaks::restBranches() const
{
	return mRestBranches;
}

void NodeWithBreaks::setRestBranches(const QList<IntermediateNode *> &restBranches)
{
	mRestBranches = restBranches;
}

bool NodeWithBreaks::analyzeBreak()
{
	mHasBreakInside = true;
	mBreakWasAnalyzed = true;
	return mHasBreakInside;
}

IntermediateNode::Type NodeWithBreaks::type() const
{
	return nodeWithBreaks;
}

qReal::Id NodeWithBreaks::firstId() const
{
	return mCondition->firstId();
}
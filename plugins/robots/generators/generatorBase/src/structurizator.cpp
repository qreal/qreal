#include "structurizator.h"

using namespace generatorBase;

generatorBase::utils::SimpleNode::SimpleNode(const qReal::Id &id)
	: mId(id)
{
}

qReal::Id generatorBase::utils::SimpleNode::id() const
{
	return mId;
}

generatorBase::utils::IfNode::IfNode(generatorBase::utils::Node *condition, generatorBase::utils::Node *thenBranch, generatorBase::utils::Node *elseBranch)
	: mCondition(condition)
	, mThenBranch(thenBranch)
	, mElseBranch(elseBranch)
	, mIsIfThenForm(elseBranch == nullptr)
{
}

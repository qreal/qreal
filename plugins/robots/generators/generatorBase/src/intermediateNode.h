#pragma once

#include <qrrepo/repoApi.h>
#include <qrkernel/ids.h>

#include <generatorBase/semanticTree/semanticNode.h>
#include <generatorBase/semanticTree/semanticTree.h>

namespace generatorBase {
namespace myUtils {

class IntermediateNode : public QObject {

	Q_OBJECT

public:

	enum Type {
		simple
		, block
		, ifThenCondition
		, ifThenElseCondition
		, ifWithBreakCondition
		, switchCondition
		, infiniteloop
		, whileloop
	};

	IntermediateNode(QObject *parent = nullptr);
	IntermediateNode(QSet<qReal::Id> &ids, QObject *parent = nullptr);


	virtual Type type() const = 0;
	virtual qReal::Id firstId() const = 0;
	bool hasBreakInside() const;
	//virtual semantics::SemanticNode *semanticNode(semantics::SemanticTree *tree, ) = 0;


	QSet<qReal::Id> ids() const;
protected:
	QSet<qReal::Id> mIdsInvolved;
	bool mIsInsideCycle;
	bool mIsInsideSwitch;
	bool mHasBreakInside;
};

class SimpleNode : public IntermediateNode {

	Q_OBJECT

public:
	SimpleNode(const qReal::Id &id, QObject *parent = nullptr);

	Type type() const;
	qReal::Id firstId() const;
	//semantics::SemanticNode *semanticNode(semantics::SemanticTree *tree);

	qReal::Id id() const;
private:
	const qReal::Id &mId;
};

class IfNode : public IntermediateNode {

	Q_OBJECT

public:
	IfNode(SimpleNode *condition, IntermediateNode *thenBranch, IntermediateNode *elseBranch, QObject *parent = nullptr);

	SimpleNode *condition() const;
	IntermediateNode *thenBranch() const;
	IntermediateNode *elseBranch() const;

	Type type() const;
	qReal::Id firstId() const;
	//semantics::SemanticNode *semanticNode(semantics::SemanticTree *tree);
private:
	SimpleNode *mCondition;
	IntermediateNode *mThenBranch;
	IntermediateNode *mElseBranch;
	bool mIsIfThenForm;
};


class SwitchNode : public IntermediateNode {

	Q_OBJECT

public:
	SwitchNode(SimpleNode *condition, const QList<IntermediateNode *> &branches, QObject *parent = nullptr);

	SimpleNode *condition() const;
	QList<IntermediateNode *> branches() const;

	Type type() const;
	qReal::Id firstId() const;
private:
	SimpleNode *mCondition;
	const QList<IntermediateNode *> mBranches;
};


class BlockNode : public IntermediateNode {

	Q_OBJECT

public:
	BlockNode(IntermediateNode *firstNode, IntermediateNode *secondNode, QObject *parent = nullptr);

	IntermediateNode *firstNode() const;
	IntermediateNode *secondNode() const;

	Type type() const;
	qReal::Id firstId() const;
private:
	IntermediateNode *mFirstNode;
	IntermediateNode *mSecondNode;
};

class WhileNode : public IntermediateNode {

	Q_OBJECT

public:
	WhileNode(IntermediateNode *headNode, IntermediateNode *bodyNode, QObject *parent = nullptr);

	IntermediateNode *headNode() const;
	IntermediateNode *bodyNode() const;

	Type type() const;
	qReal::Id firstId() const;
private:
	IntermediateNode *mHeadNode;
	IntermediateNode *mBodyNode;
};


class SelfLoopNode : public IntermediateNode {

	Q_OBJECT

public:
	SelfLoopNode(IntermediateNode *bodyNode, QObject *parent = nullptr);

	IntermediateNode *bodyNode() const;

	Type type() const;
	qReal::Id firstId() const;
private:
	IntermediateNode *mBodyNode;
};


class IfWithBreakNode : public IntermediateNode {

	Q_OBJECT

public:
	IfWithBreakNode(SimpleNode *condition, IntermediateNode *actionsBeforeBreak,
							IntermediateNode *nodeThatIsConnectedWithCondition, QObject *parent = nullptr);

	SimpleNode *condition() const;
	IntermediateNode *actionsBeforeBreak() const;
	IntermediateNode *nodeThatIsConnectedWithCondition() const;

	Type type() const;
	qReal::Id firstId() const;
private:
	SimpleNode *mCondition;
	IntermediateNode *mActionsBeforeBreak;
	IntermediateNode *mNodeThatIsConnectedWithCondition;
};

}
}

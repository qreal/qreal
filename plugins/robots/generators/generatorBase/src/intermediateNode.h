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
		, switchCondition
		, infiniteloop
		, whileloop
		, breakNode
		, fakeCycleHead
		, nodeWithBreaks
	};

	IntermediateNode(QObject *parent = nullptr);
	IntermediateNode(QSet<qReal::Id> &ids, QObject *parent = nullptr);


	virtual Type type() const = 0;
	virtual qReal::Id firstId() const = 0;
	virtual bool analyzeBreak() = 0;
	bool hasBreakInside() const;


	QSet<qReal::Id> ids() const;
protected:
	QSet<qReal::Id> mIdsInvolved;
	bool mIsInsideCycle;
	bool mIsInsideSwitch;
	bool mHasBreakInside;
	bool mBreakWasAnalyzed;
};

class SimpleNode : public IntermediateNode {

	Q_OBJECT

public:
	SimpleNode(const qReal::Id &id, QObject *parent = nullptr);

	Type type() const;
	qReal::Id firstId() const;
	bool analyzeBreak();

	qReal::Id id() const;
private:
	const qReal::Id mId;
};


class FakeCycleHeadNode : public IntermediateNode {

	Q_OBJECT

public:
	FakeCycleHeadNode(const qReal::Id &id, QObject *parent = nullptr);

	Type type() const;
	qReal::Id firstId() const;
	bool analyzeBreak();

private:
	const qReal::Id mId;
};


class BreakNode : public IntermediateNode {

	Q_OBJECT

public:
	BreakNode(const qReal::Id &id, QObject *parent = nullptr);

	Type type() const;
	qReal::Id firstId() const;
	bool analyzeBreak();

private:
	const qReal::Id mId;
};

class IfNode : public IntermediateNode {

	Q_OBJECT

public:
	IfNode(IntermediateNode *condition, IntermediateNode *thenBranch, IntermediateNode *elseBranch, QObject *parent = nullptr);

	IntermediateNode *condition() const;
	IntermediateNode *thenBranch() const;
	IntermediateNode *elseBranch() const;

	bool analyzeBreak();
	Type type() const;
	qReal::Id firstId() const;
private:
	IntermediateNode *mCondition;
	IntermediateNode *mThenBranch;
	IntermediateNode *mElseBranch;
	bool mIsIfThenForm;
};


class NodeWithBreaks : public IntermediateNode {

	Q_OBJECT

public:
	NodeWithBreaks(IntermediateNode *condition, QList<IntermediateNode *> &exitBranches, QObject *parent = nullptr);

	IntermediateNode *condition() const;
	QList<IntermediateNode *> exitBranches() const;
	QList<IntermediateNode *> restBranches() const;

	void setRestBranches(const QList<IntermediateNode *> &restBranches);
	bool analyzeBreak();
	Type type() const;
	qReal::Id firstId() const;
private:
	IntermediateNode *mCondition;
	QList<IntermediateNode *> mExitBranches;
	QList<IntermediateNode *> mRestBranches;
};

class SwitchNode : public IntermediateNode {

	Q_OBJECT

public:
	SwitchNode(IntermediateNode *condition, const QList<IntermediateNode *> &branches, QObject *parent = nullptr);

	IntermediateNode *condition() const;
	QList<IntermediateNode *> branches() const;

	bool analyzeBreak();
	Type type() const;
	qReal::Id firstId() const;
private:
	IntermediateNode *mCondition;
	const QList<IntermediateNode *> mBranches;
};


class BlockNode : public IntermediateNode {

	Q_OBJECT

public:
	BlockNode(IntermediateNode *firstNode, IntermediateNode *secondNode, QObject *parent = nullptr);

	IntermediateNode *firstNode() const;
	IntermediateNode *secondNode() const;

	bool analyzeBreak();
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

	bool analyzeBreak();
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

	bool analyzeBreak();
	Type type() const;
	qReal::Id firstId() const;
private:
	IntermediateNode *mBodyNode;
};

}
}

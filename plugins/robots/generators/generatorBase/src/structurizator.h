#pragma once

#include <qrrepo/repoApi.h>
#include <qrkernel/ids.h>

#include <QObject>
#include <QSet>
#include <QMap>

namespace generatorBase {

namespace myUtils {

class Node : public QObject {

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

	Node(QObject *parent = nullptr);
	Node(QSet<qReal::Id> &ids, QObject *parent = nullptr);

	virtual Type type() const = 0;

	QSet<qReal::Id> ids() const;
protected:
	QSet<qReal::Id> mIdsInvolved;
};

class SimpleNode : public Node {

	Q_OBJECT

public:
	SimpleNode(const qReal::Id &id, QObject *parent = nullptr);

	Type type() const;

	qReal::Id id() const;
private:
	const qReal::Id &mId;
};

class IfNode : public Node {

	Q_OBJECT

public:
	IfNode(Node *condition, Node *thenBranch, Node *elseBranch, QObject *parent = nullptr);

	Node *condition() const;
	Node *thenBranch() const;
	Node *elseBranch() const;

	Type type() const;
private:
	Node *mCondition;
	Node *mThenBranch;
	Node *mElseBranch;
	bool mIsIfThenForm;
};


class SwitchNode : public Node {

	Q_OBJECT

public:
	SwitchNode(Node *condition, const QList<Node *> &branches, QObject *parent = nullptr);

	Node *condition() const;
	QList<Node *> branches() const;

	Type type() const;
private:
	Node *mCondition;
	const QList<Node *> mBranches;
};


class BlockNode : public Node {

	Q_OBJECT

public:
	BlockNode(Node *firstNode, Node *secondNode, QObject *parent = nullptr);

	Node *firstNode() const;
	Node *secondNode() const;

	Type type() const;
private:
	Node *mFirstNode;
	Node *mSecondNode;
};

class WhileNode : public Node {

	Q_OBJECT

public:
	WhileNode(Node *headNode, Node *bodyNode, QObject *parent = nullptr);

	Node *headNode() const;
	Node *bodyNode() const;

	Type type() const;
private:
	Node *mHeadNode;
	Node *mBodyNode;
};


class SelfLoopNode : public Node {

	Q_OBJECT

public:
	SelfLoopNode(Node *bodyNode, QObject *parent = nullptr);

	Node *bodyNode() const;

	Type type() const;
private:
	Node *mBodyNode;
};


class IfWithBreakNode : public Node {

	Q_OBJECT

public:
	IfWithBreakNode(Node *condition, Node *actionsBeforeBreak, QObject *parent = nullptr);

	Node *condition() const;
	Node *actionsBeforeBreak() const;

private:
	Node *mCondition;
	Node *mActionsBeforeBreak;
};

}

class Structurizator : public QObject
{
	Q_OBJECT

public:
	Structurizator(const qrRepo::RepoApi &repo
						, QSet<qReal::Id> &vertecesIds
						, QObject *parent = 0);

	myUtils::Node *performStructurization();


private:

	/// methods to identify patterns for structural analysis
	bool isBlock(int v, QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles);
	bool isIfThenElse(int v, QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles);
	bool isIfThen(int v, QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles);
	bool isSwitch(int v, QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles);
	bool isInfiniteLoop(int v, QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles);
	bool isWhileLoop(int v, QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles);

	bool checkIfThenHelper(int thenNumber, int elseNumber);
	bool checkWhileLoopHelper(int head, int body);

	bool isCycleWithBreaks(QSet<int> &reachUnder, QMap<int, int> &nodesWithExits, int &commonExit);
	bool isHeadOfCycle(int v, QSet<int> &reachUnder);


	bool findCommonExit(QSet<int> &reachUnder, QMap<int, int> &nodesWithExits, int &commonExit);
	bool checkCommonExit(int commonExit, const QMap<int, int> &nodesWithExits);

	void reduceBlock(QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles);
	void reduceIfThenElse(QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles);
	void reduceIfThen(QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles);
	void reduceSwitch(QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles);
	void reduceInfiniteLoop(QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles);
	void reduceWhileLoop(QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles);
	void reduceConditionsWithBreaks(int v, QMap<int, int> &nodesWithExits, int commonExit);
	void reduceSimpleIfWithBreak(int conditionVertex, int thenVertex, int exitVertex);
	void addAdditionalConditionWithBreak(int conditionVertex, int thenVertex, int exitVertex);

	/// Replacing some verteces with a new one and proper maintenance of edges
	void replace(int newNodeNumber, QSet<int> &edgesToRemove, QSet<int> &verteces);
	void updateEdges(int newNodeNumber, QSet<int> &edgesToRemove, QSet<int> &verteces);
	void updatePostOrder(int newNodeNumber, QSet<int> &verteces);
	void updateDominators(int newNodeNumber, QSet<int> &verteces);
	void updateVerteces(int newNodeNumber, QSet<int> &verteces);

	/// methods used before structurization process
	void createGraph();
	void calculateDominators();
	void findStartVertex();
	void calculatePostOrder();
	void createInitialNodesForIds();
	void dfs(int v, int &currentTime, QMap<int, bool> &used);

	void appendVertex(myUtils::Node *node, QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles);
	void appendVertex(myUtils::Node *node, QSet<int> &edgesToRemove, QSet<int> &verteces);

	int outgoingEdgesNumber(int v) const;
	int incomingEdgesNumber(int v) const;

	QMap<qReal::Id, int> mMapIdToInt;

	QMap<QPair<int, int>, int> mMapEdgeNumberToVerteces;
	QSet<int> mEdges;

	QSet<int> mVerteces;
	QMap<int, QVector<int> > mFollowers;
	QMap<int, QVector<int> > mPredecessors;
	QMap<int, QSet<int> > mDominators;
	QMap<int, int> mPostOrder;

	QMap<int, myUtils::Node *> mTrees;

	const qrRepo::RepoApi &mRepo;
	QSet<qReal::Id> initialIds;
	int mVertecesNumber;
	int mEdgesNumber;
	int mStartVertex;
	int mMaxPostOrderTime;
};

}

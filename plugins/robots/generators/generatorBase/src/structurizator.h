#pragma once

#include <qrrepo/repoApi.h>
#include <qrkernel/ids.h>

#include <QObject>
#include <QSet>
#include <QMap>

namespace generatorBase {

namespace utils {

class Node : public QObject {

	Q_OBJECT

public:
	Node(QObject *parent = nullptr);
	Node(QSet<qReal::Id> &ids, QObject *parent = nullptr);

	QSet<qReal::Id> ids() const;
protected:
	QSet<qReal::Id> mIdsInvolved;
};

class SimpleNode : public Node {

	Q_OBJECT

public:
	SimpleNode(const qReal::Id &id, QObject *parent = nullptr);

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

private:
	Node *mHeadNode;
	Node *mBodyNode;
};


class SelfLoopNode : public Node {

	Q_OBJECT

public:
	SelfLoopNode(Node *bodyNode, QObject *parent = nullptr);

	Node *bodyNode() const;

private:
	Node *mBodyNode;
};

}

class Structurizator : public QObject
{
	Q_OBJECT

public:
	Structurizator(const qrRepo::RepoApi &repo
						, const qReal::IdList &vertecesIds
						, QObject *parent = 0);

	utils::Node *performStructurization();


private:


	/// methods to identify patterns for structural analysis
	bool isBlock(int v, QSet<int> &edgesToRemove, QSet<int> &verteces);
	bool isIfThenElse(int v, QSet<int> &edgesToRemove, QSet<int> &verteces);
	bool isIfThen(int v, QSet<int> &edgesToRemove, QSet<int> &verteces);
	bool isSwitch(int v, QSet<int> &edgesToRemove, QSet<int> &verteces);
	bool isDummySwitch(int v, QSet<int> &edgesToRemove, QSet<int> &verteces);
	bool isInfiniteLoop(int v, QSet<int> &edgesToRemove, QSet<int> &verteces);
	bool isWhileLoop(int v, QSet<int> &edgesToRemove, QSet<int> &verteces);

	void reduceBlock(int v, QSet<int> &edgesToRemove, QSet<int> &verteces);
	void reduceIfThenElse(int v, QSet<int> &edgesToRemove, QSet<int> &verteces);
	void reduceIfThen(int v, QSet<int> &edgesToRemove, QSet<int> &verteces);
	void reduceSwitch(int v, QSet<int> &edgesToRemove, QSet<int> &verteces);
	void reduceDummySwitch(int v, QSet<int> &edgesToRemove, QSet<int> &verteces);
	void reduceInfiniteLoop(int v, QSet<int> &edgesToRemove, QSet<int> &verteces);
	void reduceWhileLoop(int v, QSet<int> &edgesToRemove, QSet<int> &verteces);


	void createGraph();
	void calculateDominators();
	void findStartVertex();
	void calculatePostOrder();
	void createInitialNodesForIds();
	void dfs(int v, int currentTime, QMap<int, bool> &used);


	QMap<qReal::Id, int> mMapIdToInt;

	QMap<int, QPair<int, int> > mMapEdgeNumberToVerteces;
	QSet<int> mEdges;

	QSet<int> mVerteces;
	QMap<int, QSet<int> > mFollowers;
	QMap<int, QSet<int> > mPredecessors;
	QMap<int, QSet<int> > mDominators;
	QMap<int, int> mPostOrder;

	QMap<int, utils::Node *> mTrees;

	const qrRepo::RepoApi &mRepo;
	qReal::IdList initialIds;
	int mVertecesNumber;
	int mEdgesNumber;
	int mStartVertex;
	int mMaxPostOrderTime;
};

}

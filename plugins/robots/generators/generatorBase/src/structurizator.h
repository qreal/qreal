#pragma once

#include <qrrepo/repoApi.h>
#include <qrkernel/ids.h>

#include "intermediateNode.h"

#include <QObject>
#include <QSet>
#include <QMap>

namespace generatorBase {

class Structurizator : public QObject
{
	Q_OBJECT

public:
	Structurizator(const qrRepo::RepoApi &repo
						, QSet<qReal::Id> &vertecesIds
						, QObject *parent = 0);

	myUtils::IntermediateNode *performStructurization();


private:

	typedef int Time;
	typedef int VertexNumber;


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

	void appendVertex(myUtils::IntermediateNode *node, QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles);
	void appendVertex(myUtils::IntermediateNode *node, QSet<int> &edgesToRemove, QSet<int> &verteces);

	int outgoingEdgesNumber(int v) const;
	int incomingEdgesNumber(int v) const;

	QMap<qReal::Id, int> mMapIdToInt;

	QMap<QPair<int, int>, int> mMapEdgeNumberToVerteces;
	QSet<int> mEdges;


	bool checkAllStructures();
	bool checkFollowers();
	bool checkDominators();
	bool checkPostOrder();

	QSet<VertexNumber> mVerteces;
	QMap<VertexNumber, QVector<VertexNumber> > mFollowers;
	QMap<VertexNumber, QVector<VertexNumber> > mPredecessors;
	QMap<VertexNumber, QSet<VertexNumber> > mDominators;
	QMap<VertexNumber, Time> mPostOrder;

	QMap<int, myUtils::IntermediateNode *> mTrees;

	const qrRepo::RepoApi &mRepo;
	QSet<qReal::Id> initialIds;
	int mVertecesNumber;
	int mEdgesNumber;
	int mStartVertex;
	int mMaxPostOrderTime;
};

}

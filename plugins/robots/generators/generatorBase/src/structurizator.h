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
	bool isBlock(int v, QSet<QPair<int, int> > &edgesToRemove, QMap<QString, int> &verticesRoles);
	bool isIfThenElse(int v, QSet<QPair<int, int> > &edgesToRemove, QMap<QString, int> &verticesRoles);
	bool isIfThen(int v, QSet<QPair<int, int> > &edgesToRemove, QMap<QString, int> &verticesRoles);
	bool isSwitch(int v, QSet<QPair<int, int> > &edgesToRemove, QMap<QString, int> &verticesRoles);
	bool isInfiniteLoop(int v, QSet<QPair<int, int> > &edgesToRemove, QMap<QString, int> &verticesRoles);
	bool isWhileLoop(int v, QSet<QPair<int, int> > &edgesToRemove, QMap<QString, int> &verticesRoles);

	bool checkIfThenHelper(int thenNumber, int elseNumber);
	bool checkWhileLoopHelper(int head, int body);

	bool isCycleWithBreaks(QSet<int> &reachUnder, QMap<int, int> &nodesWithExits, int &commonExit);
	bool isHeadOfCycle(int v, QSet<int> &reachUnder);


	bool findCommonExit(QSet<int> &reachUnder, QMap<int, int> &nodesWithExits, int &commonExit);
	bool checkCommonExit(int commonExit, const QMap<int, int> &nodesWithExits);

	void reduceBlock(QSet<QPair<int, int> > &edgesToRemove, QMap<QString, int> &verticesRoles);
	void reduceIfThenElse(QSet<QPair<int, int> > &edgesToRemove, QMap<QString, int> &verticesRoles);
	void reduceIfThen(QSet<QPair<int, int> > &edgesToRemove, QMap<QString, int> &verticesRoles);
	void reduceSwitch(QSet<QPair<int, int> > &edgesToRemove, QMap<QString, int> &verticesRoles);
	void reduceInfiniteLoop(QSet<QPair<int, int> > &edgesToRemove, QMap<QString, int> &verticesRoles);
	void reduceWhileLoop(QSet<QPair<int, int> > &edgesToRemove, QMap<QString, int> &verticesRoles);
	void reduceConditionsWithBreaks(int v, QMap<int, int> &nodesWithExits, int commonExit);
	void reduceSimpleIfWithBreak(int conditionVertex, int thenVertex, int exitVertex);
	void addAdditionalConditionWithBreak(int conditionVertex, int thenVertex, int exitVertex);

	/// Replacing some verteces with a new one and proper maintenance of edges
	void replace(int newNodeNumber, QSet<QPair<int, int> > &edgesToRemove, QSet<int> &vertices);
	void replace(int newNodeNumber, QSet<QPair<int, int> > &edgesToRemove, QMap<QString, int> &verticesRoles);
	void updateEdges(int newNodeNumber, QSet<QPair<int, int> > &edgesToRemove, QSet<int> &vertices);
	void updatePostOrder(int newNodeNumber, QSet<int> &vertices);
	void updateDominators(int newNodeNumber, QSet<int> &vertices);
	void updateVertices(int newNodeNumber, QSet<int> &vertices);

	/// methods used before structurization process
	void createGraph();
	void calculateDominators();
	void findStartVertex();
	void calculatePostOrder();
	void createInitialNodesForIds();
	void dfs(int v, int &currentTime, QMap<int, bool> &used);

	int appendVertex(myUtils::IntermediateNode *node);

	int outgoingEdgesNumber(int v) const;
	int incomingEdgesNumber(int v) const;

	QMap<qReal::Id, int> mMapIdToInt;

	QPair<int, int> makePair(int a, int b);

	bool checkAllStructures();
	bool checkFollowers();
	bool checkDominators();
	bool checkPostOrder();

	QSet<VertexNumber> mVertices;
	QMap<VertexNumber, QVector<VertexNumber> > mFollowers;
	QMap<VertexNumber, QVector<VertexNumber> > mPredecessors;
	QMap<VertexNumber, QSet<VertexNumber> > mDominators;
	QMap<VertexNumber, Time> mPostOrder;

	QMap<int, myUtils::IntermediateNode *> mTrees;

	const qrRepo::RepoApi &mRepo;
	QSet<qReal::Id> initialIds;
	int mVertecesNumber;
	int mStartVertex;
	int mMaxPostOrderTime;
};

}

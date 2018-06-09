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
	explicit Structurizator(QObject *parent = 0);

	myUtils::IntermediateNode *performStructurization(const QSet<qReal::Id> &verticesIds, int startVertex
														, const QMap<int, QSet<int>> &followers, const QMap<qReal::Id, int> &vertexNumber
														, int verticesNumber);

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

	bool isCycleWithBreaks(QSet<int> &reachUnder, QMap<int, QSet<int> > &nodesWithExits, int &commonExit);
	bool isHeadOfCycle(int v, QSet<int> &reachUnder);


	bool findCommonExit(QSet<int> &reachUnder, QMap<int, QSet<int> > &nodesWithExits, int &commonExit);
	bool checkCommonExitUniqueness(int commonExit, const QMap<int, QSet<int> > &nodesWithExits);
	bool checkNodes(const QSet<int> &verticesWithExits);

	void reduceBlock(QSet<QPair<int, int> > &edgesToRemove, QMap<QString, int> &verticesRoles);
	void reduceIfThenElse(QSet<QPair<int, int> > &edgesToRemove, QMap<QString, int> &verticesRoles);
	void reduceIfThen(QSet<QPair<int, int> > &edgesToRemove, QMap<QString, int> &verticesRoles);
	void reduceSwitch(QSet<QPair<int, int> > &edgesToRemove, QMap<QString, int> &verticesRoles);
	void reduceInfiniteLoop(QSet<QPair<int, int> > &edgesToRemove, QMap<QString, int> &verticesRoles);
	void reduceWhileLoop(QSet<QPair<int, int> > &edgesToRemove, QMap<QString, int> &verticesRoles);

	void reduceConditionsWithBreaks(int &v, QMap<int, QSet<int> > &nodesWithExits, int commonExit);
	void reduceSimpleIfWithBreak(int conditionVertex, int thenVertex, int exitVertex);
	void addAdditionalConditionWithBreak(int conditionVertex, int thenVertex, int exitVertex);

	/// Replacing some verteces with a new one and proper maintenance of edges
	void replace(int newNodeNumber, QSet<QPair<int, int> > &edgesToRemove, QSet<int> &vertices);
	void replace(int newNodeNumber, QSet<QPair<int, int> > &edgesToRemove, QMap<QString, int> &verticesRoles);
	void updateEdges(int newNodeNumber, QSet<QPair<int, int> > &edgesToRemove, QSet<int> &vertices);
	void updatePostOrder(int newNodeNumber, QSet<int> &vertices);
	void updateDominators(int newNodeNumber, QSet<int> &vertices);
	void updateVertices(int newNodeNumber, QSet<int> &vertices);
	void removeNodesPreviouslyDetectedAsNodeWithExit(QSet<int> &vertices);

	/// methods used before structurization process
	void calculateDominators();
	void findStartVertex();
	void calculatePostOrder();
	void createInitialNodesForIds();
	void dfs(int v, int &currentTime, QMap<int, bool> &used);

	void appendNodesDetectedAsNodeWithExit(QSet<int> &vertices, int cycleHead);
	int appendVertex(myUtils::IntermediateNode *node);

	int outgoingEdgesNumber(int v) const;
	int incomingEdgesNumber(int v) const;

	QMap<qReal::Id, int> mMapIdToInt;

	QPair<int, int> makePair(int a, int b);

	QSet<VertexNumber> mVertices;
	QMap<VertexNumber, QVector<VertexNumber> > mFollowers;
	QMap<VertexNumber, QVector<VertexNumber> > mPredecessors;
	QMap<VertexNumber, QSet<VertexNumber> > mDominators;
	QMap<VertexNumber, Time> mPostOrder;
	QMap<VertexNumber, VertexNumber> mWasPreviouslyDetectedAsNodeWithExit;

	QMap<int, myUtils::IntermediateNode *> mTrees;

	QSet<qReal::Id> mInitialIds;
	int mVerticesNumber;
	int mStartVertex;
	int mMaxPostOrderTime;
};

}

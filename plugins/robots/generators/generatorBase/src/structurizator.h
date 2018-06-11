/* Copyright 2018 Konstantin Batoev
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <qrkernel/ids.h>

#include <QObject>
#include <QSet>
#include <QMap>

namespace generatorBase {

class IntermediateStructurizatorNode;
class SimpleStructurizatorNode;
class BreakStructurizatorNode;
class IfStructurizatorNode;
class SwitchStructurizatorNode;
class BlockStructurizatorNode;
class WhileStructurizatorNode;
class SelfLoopStructurizatorNode;
class StructurizatorNodeWithBreaks;

/// @class Structurizator is aimed to produce control flow AST from diagram performing Structural Analysis.
/// AST Nodes are concrete classes (BlockNode, WhileNode, etc.) of abstract @class IntermediateNode
/// and represent some type of control structure of high-order programming language.
///
/// We should fill needed structures before performing main algorithm:
/// 1) calculate dominators used for detecting vertices in cycle body;
/// 2) calculate postorder vertices numbers to perform structurizaion from down to up.
///
/// Structurization is a process of graph transformations made iteratively.
/// Each transformation has four steps:
/// 1) pattern matching, remembering vertices and their roles
/// 2) creating new vertex and IntermediateNode of corresponding subclass
/// 3) reducing graph substituting "old" edges incident to old vertices with "new" ones;
/// 4) updating dominance relationship and postorder
///
/// These four steps are performed until there would be one vertex with no edges.
///
/// @return a pointer to an obtained tree and nullptr if structurization was unsuccessfull.
class Structurizator : public QObject
{
	Q_OBJECT

public:
	explicit Structurizator(QObject *parent = 0);

	/// main function that performs structurization
	IntermediateStructurizatorNode *performStructurization(const QSet<qReal::Id> &verticesIds, int startVertex
			, const QMap<int, QSet<int>> &followers, const QMap<qReal::Id, int> &vertexNumber, int verticesNumber);

private:
	typedef int Time;
	typedef int VertexNumber;

	/// methods to identify patterns for structural analysis
	/// @arg edgesToRemove -- passed by reference.
	/// @arg verticesRoles -- passed by reference.
	bool isBlock(int v, QSet<QPair<int, int>> &edgesToRemove, QMap<QString, int> &verticesRoles);
	bool isIfThenElse(int v, QSet<QPair<int, int>> &edgesToRemove, QMap<QString, int> &verticesRoles);
	bool isIfThen(int v, QSet<QPair<int, int>> &edgesToRemove, QMap<QString, int> &verticesRoles);
	bool isSwitch(int v, QSet<QPair<int, int>> &edgesToRemove, QMap<QString, int> &verticesRoles);
	bool isInfiniteLoop(int v, QSet<QPair<int, int>> &edgesToRemove, QMap<QString, int> &verticesRoles);
	bool isWhileLoop(int v, QSet<QPair<int, int>> &edgesToRemove, QMap<QString, int> &verticesRoles);

	/// helper functions for clarifying vertices roles
	bool checkIfThenHelper(int thenNumber, int elseNumber);
	bool checkWhileLoopHelper(int head, int body);

	/// functions for identifying loops that have "obstructive" vertices with edges going outside loop to EXIT.
	/// EXIT --- is a vertex to which control is transfered after loop execution.
	/// the main idea is to remove such vertices and edges substituting them with new vertices
	/// of class NodeWithBreaks which remember actions to perform before Break.
	bool isCycleWithBreaks(QSet<int> &reachUnder, QMap<int, QSet<int>> &nodesWithExits, int &commonExit);
	bool isHeadOfCycle(int v, QSet<int> &reachUnder);
	bool findCommonExit(QSet<int> &reachUnder, QMap<int, QSet<int>> &nodesWithExits, int &commonExit);

	/// EXIT must be unique.
	bool checkCommonExitUniqueness(int commonExit, const QMap<int, QSet<int>> &nodesWithExits);

	/// Vertices with exits must belong only to one loop.
	/// This limitation was introduced in order to forbid situation of nested loops when break
	/// nodes belong to inner one, because in general such a situation is resolved with
	/// introducing new flag-variables.
	bool checkNodes(const QSet<int> &verticesWithExits);

	/// methods for reducing recognised pattern
	void reduceBlock(QSet<QPair<int, int>> &edgesToRemove, QMap<QString, int> &verticesRoles);
	void reduceIfThenElse(QSet<QPair<int, int>> &edgesToRemove, QMap<QString, int> &verticesRoles);
	void reduceIfThen(QSet<QPair<int, int>> &edgesToRemove, QMap<QString, int> &verticesRoles);
	void reduceSwitch(QSet<QPair<int, int>> &edgesToRemove, QMap<QString, int> &verticesRoles);
	void reduceInfiniteLoop(QSet<QPair<int, int>> &edgesToRemove, QMap<QString, int> &verticesRoles);
	void reduceWhileLoop(QSet<QPair<int, int>> &edgesToRemove, QMap<QString, int> &verticesRoles);
	void reduceConditionsWithBreaks(int &v, QMap<int, QSet<int>> &nodesWithExits, int commonExit);

	/// Replacing some verteces with a new one and proper maintenance of edges
	void replace(int newNodeNumber, QSet<QPair<int, int>> &edgesToRemove, QSet<int> &vertices);
	void replace(int newNodeNumber, QSet<QPair<int, int>> &edgesToRemove, QMap<QString, int> &verticesRoles);
	void updateEdges(int newNodeNumber, QSet<QPair<int, int>> &edgesToRemove, QSet<int> &vertices);
	void updatePostOrder(int newNodeNumber, QSet<int> &vertices);
	void updateDominators(int newNodeNumber, QSet<int> &vertices);
	void updateVertices(int newNodeNumber, QSet<int> &vertices);

	/// methods used before structurization process
	void calculateDominators();
	void findStartVertex();
	void calculatePostOrder();
	void createInitialNodesForIds();
	void dfs(int v, int &currentTime, QMap<int, bool> &used);

	void appendNodesDetectedAsNodeWithExit(QSet<int> &vertices, int cycleHead);
	void removeNodesPreviouslyDetectedAsNodeWithExit(QSet<int> &vertices);
	int appendVertex(IntermediateStructurizatorNode *node);

	int outgoingEdgesNumber(int v) const;
	int incomingEdgesNumber(int v) const;

	QMap<qReal::Id, int> mMapIdToInt;
	QSet<VertexNumber> mVertices;
	QMap<VertexNumber, QVector<VertexNumber>> mFollowers;
	QMap<VertexNumber, QVector<VertexNumber>> mPredecessors;
	QMap<VertexNumber, QSet<VertexNumber>> mDominators;
	QMap<VertexNumber, Time> mPostOrder;
	QMap<VertexNumber, VertexNumber> mWasPreviouslyDetectedAsNodeWithExit;

	QMap<int, IntermediateStructurizatorNode *> mTrees;

	QSet<qReal::Id> mInitialIds;
	int mVerticesNumber;
	int mStartVertex;
	int mMaxPostOrderTime;
};

}

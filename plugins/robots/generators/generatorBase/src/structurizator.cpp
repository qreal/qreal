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

#include "structurizator.h"

#include "structurizatorNodes/intermediateStructurizatorNode.h"
#include "structurizatorNodes/blockStructurizatorNode.h"
#include "structurizatorNodes/breakStructurizatorNode.h"
#include "structurizatorNodes/ifStructurizatorNode.h"
#include "structurizatorNodes/structurizatorNodeWithBreaks.h"
#include "structurizatorNodes/selfLoopStructurizatorNode.h"
#include "structurizatorNodes/simpleStructurizatorNode.h"
#include "structurizatorNodes/switchStructurizatorNode.h"
#include "structurizatorNodes/whileStructurizatorNode.h"
#include <QQueue>

using namespace generatorBase;

Structurizator::Structurizator(QObject *parent)
	: QObject(parent)
	, mVerticesNumber(1)
	, mStartVertex(-1)
	, mMaxPostOrderTime(-1)
{
}

IntermediateStructurizatorNode *Structurizator::performStructurization(const QSet<qReal::Id> &verticesIds
		, int startVertex
		, const QMap<int, QSet<int>> &followers
		, const QMap<qReal::Id, int> &vertexNumber
		, int verticesNumber)
{
	for (const qReal::Id &id : verticesIds) {
		mInitialIds.insert(id);
		mMapIdToInt[id] = vertexNumber[id];
		mVertices.insert(vertexNumber[id]);
	}

	mVerticesNumber = verticesNumber;

	mStartVertex = startVertex;

	for (const int v : followers.keys()) {
		for (const int u : followers[v]) {
			mFollowers[v].push_back(u);
			mPredecessors[u].push_back(v);
		}
	}

	calculatePostOrder();

	calculateDominators();

	createInitialNodesForIds();

	bool somethingChanged = true;
	while (somethingChanged) {
		somethingChanged = false;

		int t = 0;
		while (t <= mMaxPostOrderTime && (mVertices.size() > 1 || !mFollowers[mStartVertex].isEmpty())) {
			int v = mPostOrder.key(t);

			QSet<int> reachUnder;
			QSet<QPair<int, int>> edgesToRemove = {};
			QMap<QString, int> verticesRoles;
			if (isBlock(v, edgesToRemove, verticesRoles)) {
				reduceBlock(edgesToRemove, verticesRoles);
			} else if (isSwitch(v, edgesToRemove, verticesRoles)) {
				reduceSwitch(edgesToRemove, verticesRoles);
			} else if (isIfThenElse(v, edgesToRemove, verticesRoles)) {
				reduceIfThenElse(edgesToRemove, verticesRoles);
			} else if (isIfThen(v, edgesToRemove, verticesRoles)) {
				reduceIfThen(edgesToRemove, verticesRoles);
			} else if (isInfiniteLoop(v, edgesToRemove, verticesRoles)) {
				reduceInfiniteLoop(edgesToRemove, verticesRoles);
			} else if (isWhileLoop(v, edgesToRemove, verticesRoles)) {
				reduceWhileLoop(edgesToRemove, verticesRoles);
			} else if (isHeadOfCycle(v, reachUnder)) {
				int minTime = -1;
				for (const int vertex : reachUnder) {
					if (minTime == -1 || minTime > mPostOrder[vertex]) {
						minTime = mPostOrder[vertex];
					}
				}

				QMap<int, QSet<int>> nodesWithExits;
				int commonExit = -1;
				bool isCycle = isCycleWithBreaks(reachUnder, nodesWithExits, commonExit);
				QSet<int> verticesWithExits = nodesWithExits.keys().toSet();

				if (!isCycle) {
					t++;
					appendNodesDetectedAsNodeWithExit(verticesWithExits, v);
					continue;
				}

				if (!nodesWithExits.isEmpty() && checkNodes(verticesWithExits)) {

					for (const int vertexInsideLoop : nodesWithExits.keys()) {
						for (const int vertexOutsideLoop : nodesWithExits[vertexInsideLoop]) {
							if (minTime > mPostOrder[vertexOutsideLoop]) {
								minTime = mPostOrder[vertexOutsideLoop];
							}
						}
					}

					reduceConditionsWithBreaks(v, nodesWithExits, commonExit);
					t = minTime;
					somethingChanged = true;
					appendNodesDetectedAsNodeWithExit(verticesWithExits, v);
					continue;
				}
			}

			if (verticesRoles.size()) {
				t -= (verticesRoles.size() - 1);
				if (t < 0) {
					t = 0;
				}

				somethingChanged = true;
			} else {
				t++;
			}
		}
	}

	if (mVertices.size() == 1) {
		return mTrees[mStartVertex];
	}

	return nullptr;
}

bool Structurizator::isBlock(int v, QSet<QPair<int, int>> &edgesToRemove, QMap<QString, int> &verticesRoles)
{
	if (outgoingEdgesNumber(v) != 1) {
		return false;
	}

	int u = mFollowers[v].first();
	if (outgoingEdgesNumber(u) <= 1 && incomingEdgesNumber(u) == 1 && u != v && mDominators[u].contains(v)) {
		verticesRoles["block1"] = v;
		verticesRoles["block2"] = u;

		edgesToRemove.insert(qMakePair(v, u));
		return true;
	}

	return false;
}

bool Structurizator::isIfThenElse(int v, QSet<QPair<int, int>> &edgesToRemove, QMap<QString, int> &verticesRoles)
{
	if (outgoingEdgesNumber(v) != 2) {
		return false;
	}

	int u1 = mFollowers[v].first();
	int u2 = mFollowers[v].last();
	if (incomingEdgesNumber(u1) != 1 || incomingEdgesNumber(u2) != 1 || mDominators[v].contains(u1)
			|| mDominators[v].contains(u2)) {
		return false;
	}

	if ((outgoingEdgesNumber(u1) == 0 && outgoingEdgesNumber(u2) == 0) ||
				(outgoingEdgesNumber(u1) == 1 && outgoingEdgesNumber(u2) == 1 &&
				mFollowers[u1].first() == mFollowers[u2].first())) {

		verticesRoles["condition"] = v;
		verticesRoles["then"] = u1;
		verticesRoles["else"] = u2;

		if (outgoingEdgesNumber(u1) > 0) {
			verticesRoles["exit"] = mFollowers[u1].first();
		}

		edgesToRemove += { qMakePair(v, u1), qMakePair(v, u2) };
		return true;
	}

	return false;
}

bool Structurizator::isIfThen(int v, QSet<QPair<int, int>> &edgesToRemove, QMap<QString, int> &verticesRoles)
{
	if (outgoingEdgesNumber(v) != 2) {
		return false;
	}

	int u1 = mFollowers[v].first();
	int u2 = mFollowers[v].last();

	int thenNumber = -1;
	int elseNumber = -1;
	if (checkIfThenHelper(u1, u2)) {
		thenNumber = u1;
		elseNumber = u2;
	} else if (checkIfThenHelper(u2, u1)) {
		thenNumber = u2;
		elseNumber = u1;
	}

	if (thenNumber == -1 || elseNumber == v || mDominators[v].contains(thenNumber)) {
		return false;
	}

	verticesRoles["condition"] = v;
	verticesRoles["then"] = thenNumber;

	if (outgoingEdgesNumber(thenNumber) > 0) {
		verticesRoles["exit"] = mFollowers[thenNumber].first();
	}

	edgesToRemove = { qMakePair(v, u1), qMakePair(v, u2) };

	return true;
}

bool Structurizator::isSwitch(int v, QSet<QPair<int, int>> &edgesToRemove, QMap<QString, int> &verticesRoles)
{
	if (outgoingEdgesNumber(v) < 3) {
		return false;
	}

	int exit = -1;
	QSet<int> vertices = {};
	QSet<QPair<int, int>> edges = {};
	for (const int u : mFollowers[v]) {
		if (incomingEdgesNumber(u) != 1 || outgoingEdgesNumber(u) >= 2) {
			if (exit == -1) {
				exit = u;
			} else if (exit != u) {
				return false;
			}
		} else {
			if (outgoingEdgesNumber(u) == 1) {
				int m = mFollowers[u].first();
				if (exit == -1) {
					exit = m;
				} else if (m != exit) {
					return false;
				}
			}
			vertices.insert(u);
		}

		if (u != exit && mDominators[v].contains(u)) {
			return false;
		}

		edges.insert(qMakePair(v, u));
	}

	verticesRoles["head"] = v;
	edgesToRemove = edges;

	int cnt = 1;
	for (int u : vertices) {
		verticesRoles[QString::number(cnt)] = u;
		cnt++;
	}

	verticesRoles["exit"] = exit;

	return true;
}

bool Structurizator::isInfiniteLoop(int v, QSet<QPair<int, int>> &edgesToRemove, QMap<QString, int> &verticesRoles)
{
	if (outgoingEdgesNumber(v) != 1) {
		return false;
	}

	int u = mFollowers[v].first();
	if (u != v) {
		return false;
	}

	verticesRoles["body"] = v;
	edgesToRemove.insert(qMakePair(v, v));
	return true;
}

bool Structurizator::isWhileLoop(int v, QSet<QPair<int, int>> &edgesToRemove, QMap<QString, int> &verticesRoles)
{
	if (outgoingEdgesNumber(v) != 2) {
		return false;
	}

	int u1 = mFollowers[v].first();
	int u2 = mFollowers[v].last();

	int bodyNumber = -1;
	int exitNumber = -1;
	if (checkWhileLoopHelper(v, u1)) {
		bodyNumber = u1;
		exitNumber = u2;
	} else if (checkWhileLoopHelper(v, u2)) {
		bodyNumber = u2;
		exitNumber = u1;
	}

	if (bodyNumber == -1) {
		return false;
	}

	if (mDominators[v].contains(bodyNumber)) {
		return false;
	}

	edgesToRemove = { qMakePair(v, bodyNumber), qMakePair(bodyNumber, v) };

	verticesRoles["head"] = v;
	verticesRoles["body"] = bodyNumber;
	verticesRoles["exit"] = exitNumber;

	return true;
}

bool Structurizator::checkIfThenHelper(int thenNumber, int exitNumber)
{
	if (incomingEdgesNumber(thenNumber) == 1 && outgoingEdgesNumber(thenNumber) == 1) {
		if (mFollowers[thenNumber].contains(exitNumber)) {
			return true;
		}
	}

	return false;
}

bool Structurizator::checkWhileLoopHelper(int head, int body)
{
	if (incomingEdgesNumber(body) == 1 && outgoingEdgesNumber(body) == 1) {
		int w = mFollowers[body].first();
		if (w == head) {
			return true;
		}
	}

	return false;
}

bool Structurizator::isCycleWithBreaks(QSet<int> &reachUnder, QMap<int, QSet<int>> &nodesWithExits, int &commonExit)
{
	bool result = findCommonExit(reachUnder, nodesWithExits, commonExit);
	if (!result) {
		return false;
	}

	return checkCommonExitUniqueness(commonExit, nodesWithExits);
}

bool Structurizator::isHeadOfCycle(int v, QSet<int> &reachUnder)
{
	QQueue<int> queueForReachUnder;

	for (const int u : mPredecessors[v]) {
		if (mDominators[u].contains(v)) {
			queueForReachUnder.push_back(u);
		}
	}

	while (!queueForReachUnder.empty()) {
		int u = queueForReachUnder.front();
		queueForReachUnder.pop_front();
		reachUnder.insert(u);
		for (const int w : mPredecessors[u]) {
			if (mDominators[w].contains(v) && !reachUnder.contains(w)) {
				queueForReachUnder.push_back(w);
			}
		}
	}

	return !reachUnder.isEmpty();
}

bool Structurizator::findCommonExit(QSet<int> &reachUnder, QMap<int, QSet<int>> &nodesWithExits, int &commonExit)
{
	commonExit = -1;
	QSet<int> exits;

	for (const int u : reachUnder) {
		for (const int w : mFollowers[u]) {
			if (!reachUnder.contains(w)) {
				if (exits.contains(w) || incomingEdgesNumber(w) > 1) {
					if (commonExit != -1 && commonExit != w) {
						return false;
					}
					commonExit = w;
				}

				exits.insert(w);
				nodesWithExits[u].insert(w);
			}
		}
	}

	if (commonExit != -1) {
		return true;
	}

	QList<int> regionToFindCommonChild;
	for (const int exitNumber : exits) {
		if (outgoingEdgesNumber(exitNumber) == 1) {
			regionToFindCommonChild.append(exitNumber);
		} else if (outgoingEdgesNumber(exitNumber) > 1) {
			if (commonExit == -1) {
				// we have found post-cycle execution point
				commonExit = exitNumber;
			} else if (commonExit != exitNumber) {
				// each cycle can have at most 1 point to transfer execution
				return false;
			}
		}
	}

	if (commonExit != -1) {
		return true;
	}

	// assume that one exit point is commonChild
	if (regionToFindCommonChild.size() == 1) {
		commonExit = regionToFindCommonChild.first();
		return true;
	}

	for (const int exitNumber : regionToFindCommonChild) {
		for (const int postExit : mFollowers[exitNumber]) {
			if (commonExit == -1) {
				commonExit = postExit;
			} else if (commonExit != postExit) {
				return false;
			}
		}
	}

	return true;
}

bool Structurizator::checkCommonExitUniqueness(int commonExit, const QMap<int, QSet<int>> &nodesWithExits)
{
	for (const int vertexFromCycle : nodesWithExits.keys()) {
		for (const int exit : nodesWithExits[vertexFromCycle]) {
			if (commonExit == exit) {
				continue;
			}

			if (incomingEdgesNumber(exit) != 1 || outgoingEdgesNumber(exit) >= 2) {
				return false;
			}

			if (outgoingEdgesNumber(exit) == 1 && commonExit != mFollowers[exit].first()) {
				return false;
			}
		}
	}

	return true;
}

bool Structurizator::checkNodes(const QSet<int> &verticesWithExits)
{
	QSet<int> testSet = mWasPreviouslyDetectedAsNodeWithExit.keys().toSet();
	testSet.intersect(verticesWithExits);
	return testSet.isEmpty();
}

void Structurizator::reduceBlock(QSet<QPair<int, int>> &edgesToRemove, QMap<QString, int> &verticesRoles)
{
	BlockStructurizatorNode *block = new BlockStructurizatorNode(mTrees[verticesRoles["block1"]]
			, mTrees[verticesRoles["block2"]], this);
	replace(appendVertex(block), edgesToRemove, verticesRoles);
}

void Structurizator::reduceIfThenElse(QSet<QPair<int, int>> &edgesToRemove, QMap<QString, int> &verticesRoles)
{
	IntermediateStructurizatorNode *exit = nullptr;
	if (verticesRoles.contains("exit")) {
		exit = mTrees[verticesRoles["exit"]];
		verticesRoles.remove("exit");
	}

	IfStructurizatorNode *ifNode = new IfStructurizatorNode(mTrees[verticesRoles["condition"]]
			, mTrees[verticesRoles["then"]]
			, mTrees[verticesRoles["else"]]
			, exit
			, this);
	replace(appendVertex(ifNode), edgesToRemove, verticesRoles);
}

void Structurizator::reduceIfThen(QSet<QPair<int, int>> &edgesToRemove, QMap<QString, int> &verticesRoles)
{
	IntermediateStructurizatorNode *exit = nullptr;
	if (verticesRoles.contains("exit")) {
		exit = mTrees[verticesRoles["exit"]];
		verticesRoles.remove("exit");
	}

	IfStructurizatorNode *ifNode = new IfStructurizatorNode(mTrees[verticesRoles["condition"]],
			mTrees[verticesRoles["then"]]
			, nullptr
			, exit
			, this);
	replace(appendVertex(ifNode), edgesToRemove, verticesRoles);
}

void Structurizator::reduceSwitch(QSet<QPair<int, int>> &edgesToRemove, QMap<QString, int> &verticesRoles)
{
	int v = verticesRoles["head"];
	int exitNodeNumber = verticesRoles["exit"];
	verticesRoles.remove("exit");

	QSet<int> otherVerteces = verticesRoles.values().toSet();
	otherVerteces.remove(v);

	QList<IntermediateStructurizatorNode *> branches;
	for (const int u : mFollowers[v]) {
		if (otherVerteces.contains(u)) {
			branches.append(mTrees[u]);
		} else {
			branches.append(new SimpleStructurizatorNode(qReal::Id(), this));
		}
	}

	IntermediateStructurizatorNode *exitNode = exitNodeNumber == -1 ? nullptr : mTrees[exitNodeNumber];
	SwitchStructurizatorNode *switchNode = new SwitchStructurizatorNode(mTrees[v], branches, exitNode, this);

	replace(appendVertex(switchNode), edgesToRemove, verticesRoles);
}

void Structurizator::reduceInfiniteLoop(QSet<QPair<int, int>> &edgesToRemove, QMap<QString, int> &verticesRoles)
{
	SelfLoopStructurizatorNode *loopNode = new SelfLoopStructurizatorNode(mTrees[verticesRoles["body"]], this);

	replace(appendVertex(loopNode), edgesToRemove, verticesRoles);
}

void Structurizator::reduceWhileLoop(QSet<QPair<int, int>> &edgesToRemove, QMap<QString, int> &verticesRoles)
{
	WhileStructurizatorNode *whileNode = new WhileStructurizatorNode(mTrees[verticesRoles["head"]]
			, mTrees[verticesRoles["body"]]
			, mTrees[verticesRoles["exit"]]
			, this);

	verticesRoles.remove("exit");
	replace(appendVertex(whileNode), edgesToRemove, verticesRoles);
}

void Structurizator::reduceConditionsWithBreaks(int &v, QMap<int, QSet<int>> &nodesWithExits, int commonExit)
{
	for (const int u : nodesWithExits.keys()) {
		QList<IntermediateStructurizatorNode *> exitBranches;
		QSet<QPair<int, int>> edgesToRemove;
		QSet<int> vertices = {u};

		for (const int exit : nodesWithExits[u]) {
			qReal::Id id = mTrees[exit]->firstId();
			IntermediateStructurizatorNode *node;
			if (exit == commonExit) {
				node = new BreakStructurizatorNode(id, this);
			} else {
				node = new BlockStructurizatorNode(mTrees[exit], new BreakStructurizatorNode(id, this), this);

				vertices.insert(exit);

				if (mFollowers[exit].contains(commonExit)) {
					edgesToRemove.insert(qMakePair(exit, commonExit));
				}
			}
			edgesToRemove.insert(qMakePair(u, exit));
			exitBranches.append(node);
		}

		StructurizatorNodeWithBreaks *nodeWithBreaks = new StructurizatorNodeWithBreaks(mTrees[u]
				, exitBranches, this);
		replace(appendVertex(nodeWithBreaks), edgesToRemove, vertices);

		if (u == v) {
			v = mTrees.key(nodeWithBreaks);
		}
	}

	// adding edge from head to common exit
	if (commonExit != -1 && !mFollowers[v].contains(commonExit)) {
		mFollowers[v].push_back(commonExit);
		mPredecessors[commonExit].push_back(v);
	}
}

void Structurizator::replace(int newNodeNumber, QSet<QPair<int, int>> &edgesToRemove, QSet<int> &vertices)
{
	updateEdges(newNodeNumber, edgesToRemove, vertices);
	updatePostOrder(newNodeNumber, vertices);
	updateDominators(newNodeNumber, vertices);
	updateVertices(newNodeNumber, vertices);
	removeNodesPreviouslyDetectedAsNodeWithExit(vertices);
}

void Structurizator::replace(int newNodeNumber, QSet<QPair<int, int>> &edgesToRemove,
		QMap<QString, int> &verticesRoles)
{
	QSet<int> vertices = verticesRoles.values().toSet();
	replace(newNodeNumber, edgesToRemove, vertices);
}

void Structurizator::updateEdges(int newNodeNumber, QSet<QPair<int, int>> &edgesToRemove, QSet<int> &vertices)
{
	for (const QPair<int, int> p : edgesToRemove) {
		mFollowers[p.first].removeAll(p.second);
		mPredecessors[p.second].removeAll(p.first);
	}

	const QMap<int, QVector<int>> followers = mFollowers;
	for (const int v : mVertices) {
		for (const int u : followers[v]) {

			int newV = vertices.contains(v) ? newNodeNumber : v;
			int newU = vertices.contains(u) ? newNodeNumber : u;

			if (newU == newNodeNumber || newV == newNodeNumber) {
				// removing old information
				mFollowers[v].removeAll(u);
				mPredecessors[u].removeAll(v);

				// inserting new information
				if (!mFollowers[newV].contains(newU)) {
					mFollowers[newV].push_back(newU);
					mPredecessors[newU].push_back(newV);
				}
			}
		}
	}

	for (const int v : vertices) {
		mFollowers.remove(v);
		mPredecessors.remove(v);
	}
}

void Structurizator::updatePostOrder(int newNodeNumber, QSet<int> &verteces)
{
	int maximum = -1;
	for (int v : verteces) {
		if (maximum == -1 || maximum < mPostOrder[v]) {
			maximum = mPostOrder[v];
		}
	}

	mPostOrder[newNodeNumber] = maximum;

	for (int v : verteces) {
		mPostOrder.remove(v);
	}

	mMaxPostOrderTime = mMaxPostOrderTime - verteces.size() + 1;

	QVector<int> times = mPostOrder.values().toVector();
	std::sort(times.begin(), times.end());

	for (int i = 0; i <= mMaxPostOrderTime; i++) {
		int v = mPostOrder.key(times[i]);
		mPostOrder[v] = i;
	}
}

void Structurizator::updateDominators(int newNodeNumber, QSet<int> &vertices)
{
	// others
	for (int v : mPostOrder.keys()) {
		QSet<int> tempSet = mDominators[v];
		tempSet.intersect(vertices);
		if (!tempSet.isEmpty()) {
			mDominators[v].subtract(vertices);
			mDominators[v].insert(newNodeNumber);
		}
	}

	// new
	QSet<int> doms = mVertices;
	for (int v : vertices) {
		doms.intersect(mDominators[v]);
	}

	doms.subtract(vertices);
	doms.insert(newNodeNumber);

	mDominators[newNodeNumber] = doms;

	// old
	for (int v : vertices) {
		mDominators.remove(v);
	}
}

void Structurizator::updateVertices(int newNodeNumber, QSet<int> &vertices)
{
	mStartVertex = vertices.contains(mStartVertex) ? newNodeNumber : mStartVertex;
	mVertices.subtract(vertices);
	mVertices.insert(newNodeNumber);
}

void Structurizator::removeNodesPreviouslyDetectedAsNodeWithExit(QSet<int> &vertices)
{
	for (const int v : vertices) {
		mWasPreviouslyDetectedAsNodeWithExit.remove(v);
	}
}

void Structurizator::calculateDominators()
{
	for (const int u : mVertices) {
		mDominators[u] = mVertices;
	}

	mDominators[mStartVertex] = { mStartVertex };

	bool somethingChanged = true;
	while (somethingChanged) {
		somethingChanged = false;

		for (const int v : mVertices) {
			if (v == mStartVertex) {
				continue;
			}

			QSet<int> doms = mVertices;
			for (const int u : mPredecessors[v]) {
				doms = doms.intersect(mDominators[u]);
			}

			doms.insert(v);

			if (doms != mDominators[v]) {
				mDominators[v] = doms;
				somethingChanged = true;
			}
		}
	}


}

void Structurizator::findStartVertex()
{
	for (const int u : mVertices) {
		if (mPredecessors[u].isEmpty()) {
			mStartVertex = u;
			return;
		}
	}
}

void Structurizator::calculatePostOrder()
{
	mPostOrder.clear();

	QMap<int, bool> used;
	for (const int v : mVertices) {
		used[v] = false;
	}

	int currentTime = 0;
	dfs(mStartVertex, currentTime, used);

	mMaxPostOrderTime = currentTime - 1;
}

void Structurizator::createInitialNodesForIds()
{
	for (const int v : mVertices) {
		mTrees[v] = new SimpleStructurizatorNode(mMapIdToInt.key(v), this);
	}
}

void Structurizator::dfs(int v, int &currentTime, QMap<int, bool> &used)
{
	used[v] = true;
	for (const int u : mFollowers[v]) {
		if (!used[u]) {
			dfs(u, currentTime, used);
		}
	}

	mPostOrder[v] = currentTime;
	currentTime++;
}

void Structurizator::appendNodesDetectedAsNodeWithExit(QSet<int> &vertices, int cycleHead)
{
	for (const int v : vertices) {
		mWasPreviouslyDetectedAsNodeWithExit[v] = cycleHead;
	}
}

int Structurizator::appendVertex(IntermediateStructurizatorNode *node)
{
	mVerticesNumber++;
	mTrees[mVerticesNumber] = node;
	mVertices.insert(mVerticesNumber);

	return mVerticesNumber;
}

int Structurizator::outgoingEdgesNumber(int v) const
{
	return mFollowers[v].size();
}

int Structurizator::incomingEdgesNumber(int v) const
{
	return mPredecessors[v].size();
}

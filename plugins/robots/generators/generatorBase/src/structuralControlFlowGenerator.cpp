/* Copyright 2017 QReal Research Group
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
#include "structuralControlFlowGenerator.h"

#include <QtCore/QQueue>

#include <QtCore/QDebug>

using namespace qReal;
using namespace generatorBase;
using namespace semantics;


StructuralControlFlowGenerator::StructuralControlFlowGenerator(const qrRepo::RepoApi &repo
		, ErrorReporterInterface &errorReporter
		, GeneratorCustomizer &customizer
		, PrimaryControlFlowValidator &validator
		, const Id &diagramId
		, QObject *parent
		, bool isThisDiagramMain)
	: ControlFlowGeneratorBase(repo, errorReporter, customizer, validator, diagramId, parent, isThisDiagramMain)
	, mVertecesNumber(1)
	, mEdgesNumber(1)
	, isPerformingGeneration(false)
{
}

ControlFlowGeneratorBase *StructuralControlFlowGenerator::cloneFor(const Id &diagramId, bool cloneForNewDiagram)
{
	StructuralControlFlowGenerator * const copy = new StructuralControlFlowGenerator(mRepo
			, mErrorReporter, mCustomizer, (cloneForNewDiagram ? *mValidator.clone() : mValidator)
			, diagramId, parent(), false);

	return copy;
}

void StructuralControlFlowGenerator::beforeSearch()
{
}

void StructuralControlFlowGenerator::visit(const Id &id, QList<LinkInfo> &links)
{
	initVerteces(id, links);
}

void StructuralControlFlowGenerator::visitRegular(const Id &id, const QList<LinkInfo> &links)
{
	Q_UNUSED(id)
	Q_UNUSED(links)
}

void StructuralControlFlowGenerator::visitConditional(const Id &id, const QList<LinkInfo> &links)
{
	Q_UNUSED(id)
	Q_UNUSED(links)
}

void StructuralControlFlowGenerator::visitLoop(const Id &id, const QList<LinkInfo> &links)
{
	Q_UNUSED(id)
	Q_UNUSED(links)
}

void StructuralControlFlowGenerator::visitSwitch(const Id &id, const QList<LinkInfo> &links)
{
	Q_UNUSED(id)
	Q_UNUSED(links)
}

void StructuralControlFlowGenerator::visitUnknown(const Id &id, const QList<LinkInfo> &links)
{
	Q_UNUSED(id)
	Q_UNUSED(links)
}

void StructuralControlFlowGenerator::afterSearch()
{
}

bool StructuralControlFlowGenerator::cantBeGeneratedIntoStructuredCode() const
{
	return mCantBeGeneratedIntoStructuredCode;
}

SemanticTree *StructuralControlFlowGenerator::generate(const Id &initialNode, const QString &threadId)
{
	ControlFlowGeneratorBase::generate(initialNode, threadId);
	return mSemanticTree;
}

void StructuralControlFlowGenerator::performGeneration()
{
	//isPerformingGeneration = false;
	mCantBeGeneratedIntoStructuredCode = false;
	ControlFlowGeneratorBase::performGeneration();
	performStructurization();
}

void StructuralControlFlowGenerator::performStructurization()
{
	buildGraph();
	findStartVertex();
	findDominators();
	createInitialSemanticNodes();
	int currentTime = 0;

	mUsed.clear();
	for (const int v : mVerteces) {
		mUsed[v] = false;
	}

	dfs(mStartVertex, currentTime);
	mMaxPostOrderTime = currentTime - 1;

	mSomethingChanged = true;
	while (mSomethingChanged) {
		mSomethingChanged = false;

		int t = 0;
		while (t <= mMaxPostOrderTime) {
			int v = mPostOrder.key(t);
			QSet<int> edgesToRemove = {};
			QMap<QString, int> vertecesRoles;
			if (isBlock(v, edgesToRemove, vertecesRoles)) {
				reduceBlock(edgesToRemove, vertecesRoles);
			} else if (isDummySwitch(v, edgesToRemove, vertecesRoles)) {
				reduceDummySwitch(edgesToRemove, vertecesRoles);
			} else if (isSwitch(v, edgesToRemove, vertecesRoles)) {
				reduceSwitch(edgesToRemove, vertecesRoles);
			} else if (isIfThenElse(v, edgesToRemove, vertecesRoles)) {
				reduceIfThenElse(edgesToRemove, vertecesRoles);
			} else if (isIfThen(v, edgesToRemove, vertecesRoles)) {
				reduceIfThen(edgesToRemove, vertecesRoles);
			} else if (isInfiniteLoop(v, edgesToRemove, vertecesRoles)) {
				reduceInfiniteLoop(edgesToRemove, vertecesRoles);
			} else if (isWhileLoop(v, edgesToRemove, vertecesRoles)) {
				reduceWhileLoop(edgesToRemove, vertecesRoles);
			} else {
				t++;
				continue;
			}
			t -= (vertecesRoles.size() - 1);
			mSomethingChanged = true;
		}
	}

	//isPerformingGeneration = true;
	//ControlFlowGeneratorBase::performGeneration();

	//isPerformingGeneration = false;

	if (mVerteces.size() == 1) {
		qDebug() << "Success!";
		mSemanticTree->setRoot(new RootNode(mTrees[mStartVertex], mSemanticTree));
	} else {
		mCantBeGeneratedIntoStructuredCode = true;
		mSemanticTree = nullptr;
	}
}

bool StructuralControlFlowGenerator::isBlock(int v, QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles)
{
	if (numberOfOutgoingEdges(v) != 1)
		return false;

	QMap<int, QVector<int>> outgoingEdges = mFollowers2[v];
	int u = outgoingEdges.keys().first();
	if (numberOfOutgoingEdges(u) <= 1 && numberOfUniqueIncomingEdges(u) == 1) {
		vertecesRoles["block1"] = v;
		vertecesRoles["block2"] = u;
		edgesToRemove += { mFollowers2[v][u].first() };
		return true;
	}

	return false;
}

bool StructuralControlFlowGenerator::isIfThenElse(int v, QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles)
{
	if (numberOfOutgoingEdges(v) != 2) {
		return false;
	}

	if (!mFollowers2.contains(v)) {
		return false;
	}

	QList<int> followers = mFollowers2[v].keys();
	int u1 = followers.first();
	int u2 = followers.last();
	if (numberOfUniqueIncomingEdges(u1) != 1 || numberOfUniqueIncomingEdges(u2) != 1) {
		return false;
	}

	if (numberOfOutgoingEdges(u1) == 0 && numberOfOutgoingEdges(u2) == 0) {
		vertecesRoles["condition"] = v;
		vertecesRoles["a"] = u1;
		vertecesRoles["b"] = u2;
		edgesToRemove += {mFollowers2[v][u2].first(), mFollowers2[v][u1].first()};
		return true;
	}

	if (numberOfOutgoingEdges(u1) == 1 && numberOfOutgoingEdges(u2) == 1) {
		int w1 = mFollowers2[u1].keys().first();
		int w2 = mFollowers2[u2].keys().first();
		if (w1 != w2) {
			return false;
		}

		if (numberOfOutgoingEdges(w1) >= 2) {
			return false;
		}

		vertecesRoles["condition"] = v;
		vertecesRoles["a"] = u1;
		vertecesRoles["b"] = u2;
		edgesToRemove += {mFollowers2[v][u2].first(), mFollowers2[v][u1].first(), mFollowers2[u1][w1].first()};
		return true;
	}

	return false;
}

bool StructuralControlFlowGenerator::isIfThen(int v, QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles)
{
	if (numberOfOutgoingEdges(v) != 2) {
		return false;
	}

	QList<int> followers = mFollowers2[v].keys();
	int u1 = followers.first();
	int u2 = followers.last();

	int thenNumber = -1;
	int elseNumber = -1;
	if (checkIfThenHelper(u1, u2)) {
		thenNumber = u1;
		elseNumber = u2;
	} else if (checkIfThenHelper(u2, u1)) {
		thenNumber = u2;
		elseNumber = u1;
	}

	if (thenNumber == -1) {
		return false;
	}

	vertecesRoles["condition"] = v;
	vertecesRoles["then"] = thenNumber;
	vertecesRoles["else"] = elseNumber;

	edgesToRemove.insert(mFollowers2[v][u1].first());
	edgesToRemove.insert(mFollowers2[v][u2].first());

	return true;
}

bool StructuralControlFlowGenerator::isSwitch(int v, QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles)
{
	const qReal::Id id = mMapVertexLabel.key(v);
	if (semanticsOf(id) != enums::semantics::switchBlock) {
		return false;
	}

	int exit = -1;
	QSet<int> verteces = {};
	for (const int u : mFollowers2[v].keys()) {
		if (numberOfUniqueIncomingEdges(u) != 1 || numberOfOutgoingEdges(u) >= 2) {
			return false;
		}

		if (numberOfOutgoingEdges(u) == 1) {
			int m = mFollowers2[u].keys().first();
			if (exit == -1 && numberOfOutgoingEdges(m) <= 1) {
				exit = m;
			} else if (m != exit) {
				return false;
			}
		}

		verteces.insert(u);
	}

	vertecesRoles["head"] = v;

	for (int u : verteces) {
		for (const int edgeNumber : mFollowers2[v][u]) {
			edgesToRemove.insert(edgeNumber);
			const QString label = mRepo.property(mEdges[edgeNumber], "Guard").toString();
			vertecesRoles[label] = u;
		}
	}

	bool wasOneEdgeSaved = false;
	if (exit != -1) {
		for (int u : verteces) {
			if (numberOfOutgoingEdges(u) == 1) {
				if (!wasOneEdgeSaved) {
					wasOneEdgeSaved = true;
				} else {
					int w = mFollowers2[u].keys().first();
					edgesToRemove.insert(mFollowers2[u][w].first());
				}
			}
		}
	}

	return true;
}

bool StructuralControlFlowGenerator::isDummySwitch(int v, QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles)
{
	const qReal::Id id = mMapVertexLabel.key(v);
	if (semanticsOf(id) != enums::semantics::switchBlock) {
		return false;
	}

	if (numberOfUniqueOutgoingEdges(v) != 1) {
		return false;
	}

	vertecesRoles["head"] = v;
	bool wasOneEdgeSaved = false;
	for (int u : mFollowers2[v].keys()) {
		for (int edge : mFollowers2[v][u]) {
			if (wasOneEdgeSaved) {
				edgesToRemove.insert(edge);
			} else {
				wasOneEdgeSaved = true;
			}
		}
	}

	return true;
}

bool StructuralControlFlowGenerator::isInfiniteLoop(int v, QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles)
{
	if (numberOfOutgoingEdges(v) != 1) {
		return false;
	}

	int u = mFollowers2[v].keys().first();
	if (u != v) {
		return false;
	}

	edgesToRemove.insert(mFollowers2[v][v].first());
	vertecesRoles["body"] = v;
	return true;
}

bool StructuralControlFlowGenerator::isWhileLoop(int v, QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles)
{
	if (numberOfOutgoingEdges(v) != 2) {
		return false;
	}

	int u1 = mFollowers2[v].keys().first();
	int u2 = mFollowers2[v].keys().last();

	int bodyNumber = -1;
	if (checkWhileLoopHelper(v, u1)) {
		bodyNumber = u1;
	} else if (checkWhileLoopHelper(v, u2)) {
		bodyNumber = u2;
	}

	if (bodyNumber == -1) {
		return false;
	}

	edgesToRemove.insert(mFollowers2[v][bodyNumber].first());
	edgesToRemove.insert(mFollowers2[bodyNumber][v].first());

	vertecesRoles["head"] = v;
	vertecesRoles["body"] = bodyNumber;

	return true;
}

bool StructuralControlFlowGenerator::checkIfThenHelper(int thenNumber, int elseNumber)
{
	if (numberOfUniqueIncomingEdges(thenNumber) == 1 && numberOfOutgoingEdges(thenNumber) == 1) {
		if (mFollowers2[thenNumber].keys().contains(elseNumber)) {
			return true;
		}
	}

	return false;
}

bool StructuralControlFlowGenerator::checkWhileLoopHelper(int head, int body)
{
	if (numberOfUniqueIncomingEdges(body) == 1 && numberOfOutgoingEdges(body) == 1) {
		int w = mFollowers2[body].keys().first();
		if (w == head) {
			return true;
		}
	}

	return false;
}

void StructuralControlFlowGenerator::initVerteces(const Id &id, const QList<LinkInfo> &links)
{
	Q_UNUSED(links)

	if (!mMapVertexLabel.contains(id)) {
		mMapVertexLabel[id] = mVertecesNumber;
		//mTrees[mVerteces] = mSemanticTree->produceNodeFor(id);
		mVerteces.insert(mVertecesNumber);
		mVertecesNumber++;
	}
}

/*
void StructuralControlFlowGenerator::identifyPatterns(const qReal::Id &id)
{
	bool ok = true;
	while (ok) {
		ok = false;
		Region region;
		if (isBlock(id, region)) {
			reduceBlock(id, region);
			ok = true;
		}
		else if (isIfThenElse(id, region)) {
			reduceIfThenElse(id, region);
			ok = true;
		}
		else if (isSelfLoop(id, region)) {
			reduceSelfLoop(id, region);
			ok = true;
		} else if (isWhileLoop(id, region)) {
			reduceWhileLoop(id, region);
			ok = true;
		}
		else if (isDoWhileLoop(id, region)) {
			reduceDoWhileLoop(id, region);
			ok = true;
		} else {
			int vertexWithBackEdge = mMapIdToVertexLabel[id];
			bool hasCycle = false;


			// the highest node with back edge to head
			int head = -1;
			for (const int u : mFollowers[vertexWithBackEdge]) {
				if (mDominators[vertexWithBackEdge].contains(u)) {
					hasCycle = true;
					if (head == -1 || mDominators[u].contains(head)) {
						head = u;
					}
				}
			}

			if (!hasCycle) {
				break;
			}

			LoopNode *loopNode = dynamic_cast<LoopNode *>(mTrees[head]);
			if (loopNode) {
				break;
			}

			QSet<int> reachUnder = {vertexWithBackEdge, head};
			QQueue<int> q;
			q.push_back(vertexWithBackEdge);
			while (!q.empty()) {
				int vertex = q.front();
				q.pop_front();
				for (const int u : mPredecessors[vertex]) {
					if (!reachUnder.contains(u) && mDominators[u].contains(head)) {
						reachUnder.insert(u);
						q.push_back(u);
					}
				}
			}

			int cnt = 0;
			ok = true;
			int breakVertex = -1;
			for (const int u : reachUnder) {
				for (const int z : mFollowers[u]) {
					if (!reachUnder.contains(z) && breakVertex == -1) {
						breakVertex = z;
						cnt++;
					} else if (!reachUnder.contains(z) && breakVertex != z) {
						ok = false;
					} else if (!reachUnder.contains(z)) {
						cnt++;
					}
				}
			}

			if (!ok || cnt == 1) {
				break;
			}

			if (breakVertex == -1) {
				break;
			}

			for (const int u : reachUnder) {
				for (const int z : mFollowers[u]) {
					if (!reachUnder.contains(z)) {
						SemanticNode *node = mTrees[u];
						SemanticNode *externalNode = mTrees[z];
						qReal::Id ifId = node->lastIfId();
						qReal::Id externalId = externalNode->firstId();
						QPair<LinkInfo, LinkInfo> p = ifBranchesFor(ifId);
						SimpleNode *breakNode = new SimpleNode(qReal::Id(), node);
						breakNode->bindToSyntheticConstruction(SimpleNode::SyntheticBlockType::breakNode);
						if (p.first.target == externalId) {
							node->makeBreakForLastIf(true, breakNode);
						} else {
							node->makeBreakForLastIf(false, breakNode);
						}
						node->setHasBreakInside(true);
					}
				}
			}

			// we can safely remove excess edges
			QMap<int, QVector<int> > oldFollowers = mFollowers;
			for (const int u : reachUnder) {
				for (const int z : oldFollowers[u]) {
					if (!reachUnder.contains(z)) {
						mFollowers[u].removeAll(z);
						mPredecessors[z].removeAll(u);
					}
				}
			}

			// adding edge from head
			mFollowers[head].push_back(breakVertex);
			mPredecessors[breakVertex].push_back(head);
			mTrees[head]->setHasBreakInside(true);
		}

	}
}
*/

void StructuralControlFlowGenerator::buildGraph()
{
	for (int v : mVerteces) {
		const qReal::Id vId = mMapVertexLabel.key(v);

		for (const qReal::Id link : mRepo.outgoingLinks(vId)) {
			const int u = mMapVertexLabel[mRepo.otherEntityFromLink(link, vId)];
			addEdge(mFollowers2, v, u, link);
		}

		for (const qReal::Id link : mRepo.incomingLinks(vId)) {
			const qReal::Id uId = mRepo.otherEntityFromLink(link, vId);
			const int u = mMapVertexLabel[uId];
			addEdge(mPredecessors2, v, u, link);
		}
	}
}

void StructuralControlFlowGenerator::findDominators()
{
	mDominators[mStartVertex] = { mStartVertex };
	bool somethingChanged = true;

	while (somethingChanged) {
		somethingChanged = false;

		for (const int v : mVerteces) {
			if (v == mStartVertex)
				continue;

			QSet<int> doms = mVerteces;
			QList<int> predecessors = mPredecessors2[v].keys();

			for (const int u : predecessors) {
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

void StructuralControlFlowGenerator::createInitialSemanticNodes()
{
	for (const int u : mVerteces) {
		if (numberOfOutgoingEdges(u) <= 1) {
			mTrees[u] = mSemanticTree->produceNodeFor(mMapVertexLabel.key(u));
		}
	}
}

void StructuralControlFlowGenerator::findStartVertex()
{
	for (int v : mFollowers2.keys()) {
		if (!mPredecessors2[v].size()) {
			mStartVertex = v;
			break;
		}
	}
}

void StructuralControlFlowGenerator::dfs(int v, int &currentTime)
{
	mUsed[v] = true;
	for (const int u : mFollowers2[v].keys().toSet()) {
		if (!mUsed[u]) {
			dfs(u, currentTime);
		}
	}
	mPostOrder[v] = currentTime++;
}

void StructuralControlFlowGenerator::obtainReachUnder(int v, QSet<int> &reachUnder)
{
	Q_UNUSED(v)
	Q_UNUSED(reachUnder)
}

int StructuralControlFlowGenerator::numberOfOutgoingEdges(int v)
{
	int ans = 0;
	for (int u : mFollowers2[v].keys()) {
		ans += mFollowers2[v][u].size();
	}

	return ans;
}

int StructuralControlFlowGenerator::numberOfUniqueIncomingEdges(int v)
{
	return mPredecessors2[v].keys().toSet().size();
}

int StructuralControlFlowGenerator::numberOfUniqueOutgoingEdges(int v)
{
	return mFollowers2[v].keys().toSet().size();
}

void StructuralControlFlowGenerator::replace(int newNodeNumber, QSet<int> &edgesToRemove, QSet<int> &verteces)
{
	updateEdges(newNodeNumber, edgesToRemove, verteces);
	updatePostOrder(newNodeNumber, verteces);
	updateDominators(newNodeNumber, verteces);
	updateIds(newNodeNumber, verteces);
	updateVerteces(newNodeNumber, verteces);
}

void StructuralControlFlowGenerator::updateEdges(int newNodeNumber, QSet<int> &edgesToRemove, QSet<int> &verteces)
{
	QMap<int, QMap<int, QVector<int>>> followers = mFollowers2;

	for (int v : mVerteces) {
		for (int u : followers[v].keys()) {
			for (int edge : followers[v][u]) {
				if (edgesToRemove.contains(edge)) {
					//mFollowers2[v][u].remove(edge);
					//mPredecessors2[u][v].remove(edge);
					continue;
				}
				int newV = v;
				int newU = u;

				if (verteces.contains(v)) {
					newV = newNodeNumber;
				}

				if (verteces.contains(u)) {
					newU = newNodeNumber;
				}

				if (newU == newNodeNumber || newV == newNodeNumber) {
					mFollowers2[newV][newU].push_back(edge);
					mPredecessors2[newU][newV].push_back(edge);
				}

			}
		}
	}

	// removing old information
	for (int v : verteces) {
		mFollowers2.remove(v);
		mPredecessors2.remove(v);
	}

	// removing information from actual verteces to old ones
	for (int v : mVerteces) {
		if (verteces.contains(v))
			continue;

		for (int u : verteces) {
			mFollowers2[v].remove(u);
			mPredecessors2[v].remove(u);
		}
	}
}

void StructuralControlFlowGenerator::updatePostOrder(int newNodeNumber, QSet<int> &verteces)
{
	int minimum = -1;
	for (int v : verteces) {
		if (minimum == -1 || minimum > mPostOrder[v]) {
			minimum = mPostOrder[v];
		}
	}

	mPostOrder[newNodeNumber] = minimum;

	for (int v : verteces) {
		mPostOrder.remove(v);
	}

	int offset = verteces.size() - 1;
	for (int v : mPostOrder.keys()) {
		if (mPostOrder[v] > minimum) {
			mPostOrder[v] -= offset;
		}
	}

	mMaxPostOrderTime -= offset;
}

void StructuralControlFlowGenerator::updateDominators(int newNodeNumber, QSet<int> &verteces)
{
	// others
	for (int v : mPostOrder.keys()) {
		if (mDominators[v].intersects(verteces)) {
			mDominators[v].subtract(verteces);
			mDominators[v].insert(newNodeNumber);
		}
	}

	// new
	QSet<int> doms = mVerteces;
	for (int v : verteces) {
		doms.intersect(mDominators[v]);
	}

	mDominators[newNodeNumber] = doms;

	// old
	for (int v : verteces) {
		mDominators.remove(v);
	}
}

void StructuralControlFlowGenerator::updateVerteces(int newNodeNumber, QSet<int> &verteces)
{
	if (mVerteces.contains(mStartVertex)) {
		mStartVertex = newNodeNumber;
	}

	for (int u : verteces) {
		mVerteces.remove(u);
	}

	mVerteces.insert(newNodeNumber);
}

void StructuralControlFlowGenerator::updateIds(int newNodeNumber, QSet<int> &verteces)
{
	for (int v : verteces) {
		qReal::Id id = mMapVertexLabel.key(v);
		mMapVertexLabel[id] = newNodeNumber;
	}
}

void StructuralControlFlowGenerator::appendVertex(SemanticNode *node, QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles)
{
	mTrees[mVertecesNumber] = node;
	mVerteces.insert(mVertecesNumber);

	QSet<int> verteces = vertecesRoles.values().toSet();
	replace(mVertecesNumber, edgesToRemove, verteces);
	mVertecesNumber++;
}

void StructuralControlFlowGenerator::addEdge(QMap<int, QMap<int, QVector<int> > > &graph, int v, int u, const Id &edge)
{
	graph[v][u].push_back(mEdgesNumber);
	mEdges[mEdgesNumber] = edge;
	mEdgesNumber++;
}

void StructuralControlFlowGenerator::reduceBlock(QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles)
{
	// it is supposed that v, u are Actions
	int v = vertecesRoles["block1"];
	int u = vertecesRoles["block2"];
	SemanticNode *block1 = mTrees[v];
	SemanticNode *block2 = mTrees[u];

	ZoneNode *block = new ZoneNode(mSemanticTree);
	block->appendChild(block1);
	block->appendChild(block2);

	appendVertex(block, edgesToRemove, vertecesRoles);
}

void StructuralControlFlowGenerator::reduceIfThenElse(QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles)
{
	// ASSERT that there's no SemanticNode with key v in mTrees ?
	int v = vertecesRoles["condition"];
	int u1 = vertecesRoles["a"];
	int u2 = vertecesRoles["b"];

	qReal::Id vId = mMapVertexLabel.key(v);
	IfNode *ifNode = new IfNode(vId, mSemanticTree);
	QPair<LinkInfo, LinkInfo> branches = ifBranchesFor(vId);
	if (mMapVertexLabel[branches.first.target] == u2) {
		int temp = u1;
		u1 = u2;
		u2 = temp;
	}

	SemanticNode *thenNode = mTrees[u1];
	SemanticNode *elseNode = mTrees[u2];
	ifNode->thenZone()->appendChild(thenNode);
	ifNode->elseZone()->appendChild(elseNode);


	appendVertex(ifNode, edgesToRemove, vertecesRoles);
}

void StructuralControlFlowGenerator::reduceIfThen(QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles)
{
	int v = vertecesRoles["condition"];
	int thenNumber = vertecesRoles["then"];
	int elseNumber = vertecesRoles["else"];

	qReal::Id vId = mMapVertexLabel.key(v);
	IfNode *ifNode = new IfNode(vId, mSemanticTree);
	QPair<LinkInfo, LinkInfo> branches = ifBranchesFor(vId);

	if (mMapVertexLabel[branches.first.target] == elseNumber) {
		ifNode->invertCondition();
	}

	SemanticNode *thenNode = mTrees[thenNumber];
	ifNode->thenZone()->appendChild(thenNode);

	vertecesRoles.remove("else");
	appendVertex(ifNode, edgesToRemove, vertecesRoles);
}

void StructuralControlFlowGenerator::reduceSwitch(QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles)
{
	int v = vertecesRoles["head"];
	qReal::Id switchId = mMapVertexLabel.key(v);
	SwitchNode *switchNode = new SwitchNode(switchId, mSemanticTree);

	QSet<int> verteces = vertecesRoles.values().toSet();
	QMap<int, bool> used;
	for (int u : verteces) {
		used[u] = false;
	}

	for (const QString &label : vertecesRoles.keys()) {
		if (label == "head")
			continue;

		int u = vertecesRoles[label];
		if (!used[u]) {
			switchNode->addBranch(label, mTrees[u]);
			used[u] = true;
		} else {
			switchNode->addBranch(label, mTrees[u]);
		}
	}

	appendVertex(switchNode, edgesToRemove, vertecesRoles);
}

void StructuralControlFlowGenerator::reduceDummySwitch(QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles)
{
	int v = vertecesRoles["head"];
	qReal::Id vId = mMapVertexLabel.key(v);
	SwitchNode *switchNode = new SwitchNode(vId, mSemanticTree);

	appendVertex(switchNode, edgesToRemove, vertecesRoles);
}

void StructuralControlFlowGenerator::reduceInfiniteLoop(QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles)
{
	int v = vertecesRoles["body"];

	LoopNode *loopNode = new LoopNode(qReal::Id(), mSemanticTree);
	SemanticNode *body = mTrees[v];
	loopNode->bodyZone()->appendChild(body);

	appendVertex(loopNode, edgesToRemove, vertecesRoles);
}

void StructuralControlFlowGenerator::reduceWhileLoop(QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles)
{
	const int v = vertecesRoles["head"];
	const int bodyNumber = vertecesRoles["body"];
	LoopNode *loopNode;
	if (mTrees.contains(v)) {
		loopNode = dynamic_cast<LoopNode *>(mTrees[v]);
		if (loopNode) {
			loopNode->bodyZone()->appendChild(mTrees[bodyNumber]);
		} else {
			loopNode = new LoopNode(qReal::Id(), mSemanticTree);
			loopNode->bodyZone()->appendChild(mTrees[v]);
			loopNode->bodyZone()->appendChild(mTrees[bodyNumber]);
		}
	} else {
		qReal::Id vId = mMapVertexLabel.key(v);
		if (semanticsOf(vId) == enums::semantics::conditionalBlock
				|| semanticsOf(vId) == enums::semantics::loopBlock) {
			loopNode = new LoopNode(vId, mSemanticTree);
			loopNode->bodyZone()->appendChild(mTrees[bodyNumber]);
		} else if (semanticsOf(vId) == enums::semantics::switchBlock) {

		}
	}

	appendVertex(loopNode, edgesToRemove, vertecesRoles);
}

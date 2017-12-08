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
	, mNumberOfVerteces(0)
	, mEntry(nullptr)
{
}

ControlFlowGeneratorBase *StructuralControlFlowGenerator::cloneFor(const Id &diagramId, bool cloneForNewDiagram)
{
	// to do
	return nullptr;
}

void StructuralControlFlowGenerator::beforeSearch()
{
	// to do
}

void StructuralControlFlowGenerator::visitFinal(const Id &id, const QList<LinkInfo> &links)
{
	buildGraph(id, links);
}

void StructuralControlFlowGenerator::visitFork(const Id &id, QList<LinkInfo> &links)
{
	buildGraph(id, links);
}

void StructuralControlFlowGenerator::visitJoin(const Id &id, QList<LinkInfo> &links)
{
	buildGraph(id, links);
}

void StructuralControlFlowGenerator::visitRegular(const Id &id, const QList<LinkInfo> &links)
{
	buildGraph(id, links);
}

void StructuralControlFlowGenerator::visitConditional(const Id &id, const QList<LinkInfo> &links)
{
	buildGraph(id, links);
}

void StructuralControlFlowGenerator::visitLoop(const Id &id, const QList<LinkInfo> &links)
{
	buildGraph(id, links);
}

void StructuralControlFlowGenerator::visitSwitch(const Id &id, const QList<LinkInfo> &links)
{
	buildGraph(id, links);
}

void StructuralControlFlowGenerator::visitUnknown(const Id &id, const QList<LinkInfo> &links)
{
	buildGraph(id, links);
}

void StructuralControlFlowGenerator::afterSearch()
{
	// to do
}

void StructuralControlFlowGenerator::performGeneration()
{
	// to bypass diagram and build graph
	ControlFlowGeneratorBase::performGeneration();
	// to-do check whether diagram was right, maybe ControlFlowGeneratorBase is checking it

	findDominators();

	performAnalysis();
}

void StructuralControlFlowGenerator::buildGraph(const Id &id, const QList<LinkInfo> &links)
{
	Node *currentVertex = nullptr;
	if (mInitialVerteces.contains(id)) {
		currentVertex = mInitialVerteces[id];
	} else {
		currentVertex = new Node(RegionType::simpleNode);
		mNumberOfVerteces++;
		mInitialVerteces[id] = currentVertex;
		mVerteces.push_back(currentVertex);
	}

	if (!mEntry) {
		mEntry = currentVertex;
	}

	for (const LinkInfo link : links) {
		Node * targetVertex = nullptr;
		if (mInitialVerteces.contains(link.target)) {
			targetVertex = mInitialVerteces[link.target];
		} else {
			targetVertex = new Node(RegionType::simpleNode);
			mNumberOfVerteces++;
			mInitialVerteces[link.target] = targetVertex;
			mVerteces.push_back(targetVertex);
		}
		mFollowers[currentVertex].push_back(targetVertex);
		mPredecessors[targetVertex].push_back(currentVertex);
	}
}

// to do
// to save invariant that mGraph only has valid nodes and edges,
// so it would be possible not to pass setOfNodes
void StructuralControlFlowGenerator::performAnalysis()
{
	for (auto it = mInitialVerteces.values().begin(); it != mInitialVerteces.values().end(); ++it) {
		mUsed[*it] = false;
		mPostOrder[*it] = -1;
	}
	int time = 0;
	Node * entry = mEntry;
	dfs(entry, time);

	mCurrentTime = 0;
	mMaxTime = time - 1;

	// it is supposed that mFollowers.size() == mPredecessors.size() == actual number of
	// verteces in current flowgraph

	while (mFollowers.keys().size() > 1 && mCurrentTime <= mMaxTime) {
		Node * currentNode = nullptr;

		// dummy cycle for finding node number that
		// has postOrder equal to currentTime

		for (auto it = mPostOrder.keys().begin(); it != mPostOrder.keys().end(); ++it) {
			Node * vertex = *(it);
			if (mPostOrder[vertex] == mCurrentTime) {
				currentNode = vertex;
				break;
			}
		}

		QVector<graphUtils::Node *> nodesThatComposeRegion;
		nodesThatComposeRegion.clear();
		RegionType type = determineAcyclicRegionType(currentNode, nodesThatComposeRegion);

		if (type != RegionType::nil) {

		}

	}
}

void StructuralControlFlowGenerator::findDominators()
{

	for (auto it = mVerteces.begin(); it != mVerteces.end(); it++) {
		mDominators[*it] = mVerteces.toSet();
	}

	Node * entry = mEntry;
	mDominators[entry] = { entry };


	bool somethingChanged = true;
	while (somethingChanged) {
		somethingChanged = false;

		// excluding root
		for (auto it = mVerteces.begin(); it != mVerteces.end(); it++) {
			Node * vertex = *it;
			if (vertex != mEntry) {
				if (vertex) {
					QSet<Node *> newDominators = mVerteces.toSet();
					for (auto it = mPredecessors[vertex].begin(); it != mPredecessors[vertex].end(); it++) {
						Node * predecessor = *it;
						newDominators.intersect(mDominators[predecessor]);
					}
					// adding the current number, because reflexivity of dominance relation
					newDominators.insert(vertex);
					if (newDominators != mDominators[vertex]) {
						somethingChanged = true;
						mDominators[vertex] = newDominators;
					}
				}
			}
		}
	}
}

void StructuralControlFlowGenerator::dfs(Node * u, int &postOrderLabel)
{
	mUsed[u] = true;
	for (auto it = mFollowers[u].begin(); it != mFollowers[u].end(); it++) {
		Node * v = *it;
		if (!mUsed[v]) {
			dfs(v, postOrderLabel);
		}
	}
	mPostOrder[u] = postOrderLabel;
	postOrderLabel++;
}

RegionType StructuralControlFlowGenerator::determineAcyclicRegionType(Node* &node, QVector<graphUtils::Node *> &nodesThatComposeRegion)
{
	nodesThatComposeRegion.clear();

	Node * currentNode = node;
	bool hasOnlyOneIncomingEdge = true;
	bool hasOnlyOneOutcomingEdge = mFollowers[currentNode].size() == 1;

	while (hasOnlyOneIncomingEdge && hasOnlyOneOutcomingEdge) {
		nodesThatComposeRegion.push_back(currentNode);
		currentNode = mFollowers[currentNode].at(0);
		hasOnlyOneIncomingEdge = mPredecessors[currentNode].size() == 1;
		hasOnlyOneOutcomingEdge = mFollowers[currentNode].size() == 1;
	}

	if (hasOnlyOneIncomingEdge) {
		nodesThatComposeRegion.push_back(currentNode);
	}

	currentNode = node;
	hasOnlyOneIncomingEdge = mPredecessors[currentNode].size() == 1;
	hasOnlyOneOutcomingEdge = true;

	while (hasOnlyOneIncomingEdge && hasOnlyOneOutcomingEdge) {
		if (!nodesThatComposeRegion.contains(currentNode)) {
			nodesThatComposeRegion.push_front(currentNode);
		}
		currentNode = mPredecessors[currentNode].at(0);
		hasOnlyOneIncomingEdge = mPredecessors[currentNode].size() == 1;
		hasOnlyOneOutcomingEdge = mFollowers[currentNode].size() == 1;
	}

	if (hasOnlyOneOutcomingEdge && !nodesThatComposeRegion.contains(currentNode)) {
		nodesThatComposeRegion.push_front(currentNode);
	}

	node = currentNode;

	if (nodesThatComposeRegion.size() >= 2) {
		return RegionType::Block;
	}

	nodesThatComposeRegion.clear();

	// checking for IfThenElse
	if (mFollowers[node].size() == 2) {
		Node * m = mFollowers[node].at(0);
		Node * n = mFollowers[node].at(1);

		if (mFollowers[m].size() == 1 && mFollowers[n].size() == 1
				&& mFollowers[m].at(0) == mFollowers[n].at(0)
				&& mPredecessors[m].size() == 1 && mPredecessors[n].size() == 1) {

			nodesThatComposeRegion.push_back(node);
			nodesThatComposeRegion.push_back(m);
			nodesThatComposeRegion.push_back(n);
			return RegionType::IfThenElse;
		}

	}

	// checking IfThen
	if (mFollowers[node].size() == 2) {
		Node * m = mFollowers[node].at(0);
		Node * n = mFollowers[node].at(1);

		Node * thenNode = nullptr;
		if (mFollowers[m].size() == 1 && mFollowers[m].at(0) == n && mPredecessors[m].size() == 1) {
			thenNode = m;
		} else if (mFollowers[n].size() == 1 && mFollowers[n].at(0) == m && mPredecessors[n].size() == 1) {
			thenNode = n;
		}

		if (thenNode) {
			nodesThatComposeRegion.push_back(node);
			nodesThatComposeRegion.push_back(thenNode);
			return RegionType::IfThen;
		}
	}

	return RegionType::nil;
}

RegionType StructuralControlFlowGenerator::determineCyclicRegionType(graphUtils::Node *&node, QVector<graphUtils::Node *> &nodesThatComposeRegion)
{

}

graphUtils::Node *StructuralControlFlowGenerator::reduce(graphUtils::RegionType type, QVector<graphUtils::Node *> &nodesThatComposeRegion)
{
	bool hasBackEdgeForBlock = false;
	if (type == RegionType::Block) {
		Node *lastNode = nodesThatComposeRegion.last();
		Node *firstNode = nodesThatComposeRegion.first();
		hasBackEdgeForBlock = mFollowers[lastNode].contains(firstNode);
	}

	Node *abstractNode = new Node(type);
	replace(abstractNode, nodesThatComposeRegion);

	allNodes.append(abstractNode);
	for (auto it = nodesThatComposeRegion.begin(); it != nodesThatComposeRegion.end(); ++it) {
		Node *currentNode = *it;
		currentNode->setParent(abstractNode);
	}

	abstractNode->appendChildren(nodesThatComposeRegion);
}

void StructuralControlFlowGenerator::replace(graphUtils::Node *node, QVector<graphUtils::Node *> &nodesThatComposeRegion)
{
	compact(node, nodesThatComposeRegion);

	// set of edges is determined by mFollowers or mPredecessors
	for (auto vertexIt = mFollowers.keys().begin(); vertexIt != mFollowers.keys().end(); ++vertexIt) {
		Node *currentVertex = *vertexIt;

		for (auto nextVertexIt = mFollowers[currentVertex].begin(); nextVertexIt != mFollowers[currentVertex].end(); nextVertexIt++) {
			Node *nextVertex = *nextVertexIt;

			if (nodesThatComposeRegion.contains(nextVertex) || nodesThatComposeRegion.contains(currentVertex)) {
				mFollowers[currentVertex].removeOne(nextVertex);
				mPredecessors[nextVertex].removeOne(currentVertex);
				if (mVerteces.contains(currentVertex) && currentVertex != node) {
					mFollowers[currentVertex].push_back(node);
					mPredecessors[node].push_back(currentVertex);
				} else if (mVerteces.contains(nextVertex)) {
					mFollowers[node].push_back(nextVertex);
					mPredecessors[nextVertex].push_back(node);
				}

			}

		}
	}

	// deal with Tree
	//node->appendChildren(nodesThatComposeRegion);
}

void StructuralControlFlowGenerator::compact(graphUtils::Node *node, QVector<graphUtils::Node *> &nodesThatComposeRegion)
{
	mVerteces.append(node);
	int maxPostOrderNumber = -1;

	for (auto it = nodesThatComposeRegion.begin(); it != nodesThatComposeRegion.end(); ++it) {
		if (mPostOrder[*it] > maxPostOrderNumber) {
			maxPostOrderNumber = mPostOrder[*it];
		}
	}

	mPostOrder[node] = maxPostOrderNumber;

	for (auto it = nodesThatComposeRegion.begin(); it != nodesThatComposeRegion.end(); ++it) {
		mVerteces.removeOne(*it);
		mPostOrder.remove(*it);
	}

	int appropriateTime = 0;
	for (int i = 0; i < mMaxTime; i++) {
		Node *nodeWithTimeI = mPostOrder.key(i);
		if (nodeWithTimeI) {
			mPostOrder[nodeWithTimeI] = appropriateTime;
			appropriateTime++;
		}
	}

	mCurrentTime = mPostOrder[node];
	mMaxTime = appropriateTime - 1;
}


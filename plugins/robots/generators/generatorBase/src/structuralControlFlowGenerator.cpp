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
	ControlFlowGeneratorBase::performGeneration();
	// to check whether diagram was right

	findDominators();

	performAnalysis();
}

void StructuralControlFlowGenerator::buildGraph(const Id &id, const QList<LinkInfo> &links)
{
	int numberOfCurrentVertex = -1;
	if (initialVerteces.contains(id)) {
		numberOfCurrentVertex = initialVerteces[id];
	} else {
		numberOfCurrentVertex = mNumberOfVerteces;
		mNumberOfVerteces++;
		initialVerteces[id] = numberOfCurrentVertex;
		mVerteces.push_back(numberOfCurrentVertex);
	}

	for (const LinkInfo link : links) {
		int targetNumber = -1;
		if (initialVerteces.contains(link.target)) {
			targetNumber = initialVerteces[link.target];
		} else {
			targetNumber = mNumberOfVerteces;
			mNumberOfVerteces++;
			initialVerteces[link.target] = targetNumber;
			mVerteces.push_back(targetNumber);
		}
		mFollowers[numberOfCurrentVertex].push_back(targetNumber);
		mPredecessors[targetNumber].push_back(numberOfCurrentVertex);
	}
}

// to do
// to save invariant that mGraph only has valid nodes and edges,
// so it would be possible not to pass setOfNodes
void StructuralControlFlowGenerator::performAnalysis()
{
	for (VertexIterator it = initialVerteces.values().begin(); it != initialVerteces.values().end(); ++it) {
		mUsed[*it] = false;
		mPostOrder[*it] = -1;
	}
	int time = 0;
	const GraphLabel root = 0;
	dfs(root, time);

	int currentTime = 0;
	int maxTime = time;

	// it is supposed that mFollowers.size() == mPredecessors.size() == actual number of
	// verteces in current flowgraph

	while (mFollowers.keys().size() > 1 && currentTime < maxTime) {
		int currentNode = -1;

		// dummy cycle for finding node number that
		// has postOrder equal to currentTime

		for (VertexIterator it = mPostOrder.keys().begin(); it != mPostOrder.keys().end(); ++it) {
			GraphLabel number = *(it);
			if (mPostOrder[number] == currentTime) {
				currentNode = number;
				break;
			}
		}
	}
}

void StructuralControlFlowGenerator::findDominators()
{

	for (int i = 1; i < mVerteces.size(); i++) {
		mDominators[i] = mVerteces.toSet();
	}

	const GraphLabel root = 0;
	mDominators[root] = { root };


	bool somethingChanged = true;
	while (somethingChanged) {
		somethingChanged = false;

		// excluding root
		for (VertexIterator it = mVerteces.begin(); it != mVerteces.end(); it++) {
			GraphLabel vertex = *it;
			if (vertex) {
				QSet<GraphLabel> newDominators = mVerteces.toSet();
				for (int t = 0; t < mPredecessors[vertex].size(); t++) {
					GraphLabel predecessor = mPredecessors[vertex].at(t);
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

void StructuralControlFlowGenerator::dfs(GraphLabel u, int &postOrderLabel)
{
	mUsed[u] = true;
	for (int t = 0; t < mFollowers[u].size(); t++) {
		int v = mFollowers[u].at(t);
		if (!mUsed[v]) {
			dfs(v, postOrderLabel);
		}
	}
	mPostOrder[u] = postOrderLabel;
	postOrderLabel++;
}

StructuralControlFlowGenerator::RegionType StructuralControlFlowGenerator::acyclicRegionType(QSet<int> restNodes, int &nodeNumber, QSet<int> &nodesThatComposeRegion)
{
	nodesThatComposeRegion.clear();

	int currentNodeNumber = nodeNumber;
	bool hasOnlyOneIncomingEdge = true;
	bool hasOnlyOneOutcomingEdge = mFollowers[currentNodeNumber].size() == 1;

	while (hasOnlyOneIncomingEdge && hasOnlyOneOutcomingEdge) {
		nodesThatComposeRegion.insert(currentNodeNumber);
		currentNodeNumber = mFollowers[currentNodeNumber].at(0);
		hasOnlyOneIncomingEdge = mPredecessors[currentNodeNumber].size() == 1;
		hasOnlyOneOutcomingEdge = mFollowers[currentNodeNumber].size() == 1;
	}

	if (hasOnlyOneIncomingEdge) {
		nodesThatComposeRegion.insert(currentNodeNumber);
	}

	currentNodeNumber = nodeNumber;
	hasOnlyOneIncomingEdge = mPredecessors[currentNodeNumber].size() == 1;
	hasOnlyOneOutcomingEdge = true;

	while (hasOnlyOneIncomingEdge && hasOnlyOneOutcomingEdge) {
		nodesThatComposeRegion.insert(currentNodeNumber);
		currentNodeNumber = mPredecessors[currentNodeNumber].at(0);
		hasOnlyOneIncomingEdge = mPredecessors[currentNodeNumber].size() == 1;
		hasOnlyOneOutcomingEdge = mFollowers[currentNodeNumber].size() == 1;
	}

	if (hasOnlyOneOutcomingEdge) {
		nodesThatComposeRegion.insert(currentNodeNumber);
	}

	nodeNumber = currentNodeNumber;

	if (nodesThatComposeRegion.size() == 2) {
		return Block;
	}

	// checking for IfThenElse
	if (mFollowers[nodeNumber].size() == 2) {
		int m = mFollowers[nodeNumber].at(0);
		int n = mFollowers[nodeNumber].at(1);

		if (mFollowers[m].size() == 1 && mFollowers[n].size() == 1
				&& mFollowers[m].at(0) == mFollowers[n].at(0)
				&& mPredecessors[m].size() == 1 && mPredecessors[n].size() == 1) {
			nodesThatComposeRegion.clear();
			nodesThatComposeRegion.insert(nodeNumber);
			nodesThatComposeRegion.insert(m);
			nodesThatComposeRegion.insert(n);
			return IfThenElse;
		}

	}

}


StructuralControlFlowGenerator::Node::Node(StructuralControlFlowGenerator::RegionType type, int number)
	: mRegionType(type)
	, mNumber(number)
	, mParent(nullptr)
{
}

void StructuralControlFlowGenerator::Node::appendChild(StructuralControlFlowGenerator::Node *child)
{
	mChildren.append(child);
}

void StructuralControlFlowGenerator::Node::appendChildren(const QVector<StructuralControlFlowGenerator::Node *> &children)
{
	for (StructuralControlFlowGenerator::Node *child : children) {
		mChildren.append(child);
	}
}

StructuralControlFlowGenerator::Node *StructuralControlFlowGenerator::Node::structOf() const
{
	return mParent;
}

void StructuralControlFlowGenerator::Node::setParent(StructuralControlFlowGenerator::Node *parent)
{
	mParent = parent;
}

StructuralControlFlowGenerator::RegionType StructuralControlFlowGenerator::Node::structType() const
{
	return mRegionType;
}

QVector<StructuralControlFlowGenerator::Node *> StructuralControlFlowGenerator::Node::structNodes() const
{
	return mChildren;
}


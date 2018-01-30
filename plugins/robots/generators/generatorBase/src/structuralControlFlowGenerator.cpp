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
	, mNumberOfVerteces(0)
	, mEntry(0)
	, mCounter(1)
	, mVerteces(QList<VertexLabel>())
{
}

ControlFlowGeneratorBase *StructuralControlFlowGenerator::cloneFor(const Id &diagramId, bool cloneForNewDiagram)
{
	Q_UNUSED(diagramId)
	Q_UNUSED(cloneForNewDiagram)
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

	performAnalysis();
}

void StructuralControlFlowGenerator::buildGraph(const Id &id, const QList<LinkInfo> &links)
{
	Node *currentVertex = nullptr;
	if (mInitialVerteces.contains(id)) {
		currentVertex = mInitialVerteces[id];
	} else {
		currentVertex = new Node(mCounter++, RegionType::simpleNode);
		allNodes.append(currentVertex);
		mNumberOfVerteces++;
		mInitialVerteces[id] = currentVertex;
		mVerteces.push_back(currentVertex->id());
	}

	if (!mEntry) {
		mEntry = currentVertex->id();
	}

	for (const LinkInfo link : links) {
		Node * targetVertex = nullptr;
		if (mInitialVerteces.contains(link.target)) {
			targetVertex = mInitialVerteces[link.target];
		} else {
			targetVertex = new Node(mCounter++, RegionType::simpleNode);
			allNodes.append(currentVertex);
			mNumberOfVerteces++;
			mInitialVerteces[link.target] = targetVertex;
			mVerteces.push_back(targetVertex->id());
		}
		mFollowers[currentVertex->id()].push_back(targetVertex->id());
		mPredecessors[targetVertex->id()].push_back(currentVertex->id());
	}
}

// to do
// to save invariant that mGraph only has valid nodes and edges,
// so it would be possible not to pass setOfNodes
void StructuralControlFlowGenerator::performAnalysis()
{
	findDominators();

	for (auto it = mInitialVerteces.values().begin(); it != mInitialVerteces.values().end(); ++it) {
		mUsed[(*it)->id()] = false;
		mPostOrder[(*it)->id()] = -1;
	}
	int time = 0;
	VertexLabel entry = mEntry;
	dfs(entry, time);

	mCurrentTime = 0;
	mMaxTime = time - 1;

	// it is supposed that mFollowers.size() == mPredecessors.size() == actual number of
	// verteces in current flowgraph

	while (mFollowers.keys().size() > 1 && mCurrentTime <= mMaxTime) {
		VertexLabel currentNode = -1;

		// dummy cycle for finding node number that
		// has postOrder equal to currentTime

		for (auto it = mPostOrder.keys().begin(); it != mPostOrder.keys().end(); ++it) {
			if (mPostOrder[*it] == mCurrentTime) {
				currentNode = *it;
				break;
			}
		}

		QVector<VertexLabel> nodesThatComposeRegion;
		nodesThatComposeRegion.clear();
		RegionType type = determineAcyclicRegionType(currentNode, nodesThatComposeRegion);

		if (type != RegionType::nil) {
			Node *newNode = reduce(type, nodesThatComposeRegion);
			if (nodesThatComposeRegion.contains(mEntry)) {
				mEntry = newNode->id();
			}
		} else {
			QVector<VertexLabel> reachUnder = countReachUnder(currentNode);

			type = determineCyclicRegionType(currentNode, reachUnder);
			if (type != RegionType::nil) {
				Node *newNode = reduce(type, reachUnder);
				if (reachUnder.contains(mEntry)) {
					mEntry = newNode->id();
				}
			} else {
				mCurrentTime++;
			}
		}

	}

	qDebug() << "Success\n";
}

void StructuralControlFlowGenerator::findDominators()
{

	for (auto it = mVerteces.begin(); it != mVerteces.end(); it++) {
		mDominators[*it] = mVerteces.toSet();
	}

	VertexLabel entry = mEntry;
	mDominators[entry] = { entry };


	bool somethingChanged = true;
	while (somethingChanged) {
		somethingChanged = false;

		// excluding root
		for (auto it = mVerteces.begin(); it != mVerteces.end(); it++) {
			VertexLabel vertex = *it;
			if (vertex != mEntry) {
				QSet<VertexLabel> newDominators = mVerteces.toSet();
				for (auto it = mPredecessors[vertex].begin(); it != mPredecessors[vertex].end(); it++) {
					VertexLabel predecessor = *it;
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

void StructuralControlFlowGenerator::dfs(graphUtils::VertexLabel u, int &postOrderLabel)
{
	mUsed[u] = true;
	for (auto it = mFollowers[u].begin(); it != mFollowers[u].end(); it++) {
		VertexLabel v = *it;
		if (!mUsed[v]) {
			dfs(v, postOrderLabel);
		}
	}
	mPostOrder[u] = postOrderLabel;
	postOrderLabel++;
}

RegionType StructuralControlFlowGenerator::determineAcyclicRegionType(graphUtils::VertexLabel node, QVector<graphUtils::VertexLabel> &nodesThatComposeRegion)
{
	nodesThatComposeRegion.clear();

	VertexLabel currentNode = node;
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
		VertexLabel m = mFollowers[node].at(0);
		VertexLabel n = mFollowers[node].at(1);

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
		VertexLabel m = mFollowers[node].at(0);
		VertexLabel n = mFollowers[node].at(1);

		VertexLabel thenNode = -1;
		if (mFollowers[m].size() == 1 && mFollowers[m].at(0) == n && mPredecessors[m].size() == 1) {
			thenNode = m;
		} else if (mFollowers[n].size() == 1 && mFollowers[n].at(0) == m && mPredecessors[n].size() == 1) {
			thenNode = n;
		}

		if (thenNode >= 0) {
			nodesThatComposeRegion.push_back(node);
			nodesThatComposeRegion.push_back(thenNode);
			return RegionType::IfThen;
		}
	}

	return RegionType::nil;
}

RegionType StructuralControlFlowGenerator::determineCyclicRegionType(graphUtils::VertexLabel node, QVector<graphUtils::VertexLabel> &reachUnder)
{
	if (reachUnder.size() == 1) {
		VertexLabel node = reachUnder.first();
		if (mFollowers[node].contains(node)) {
			return RegionType::SelfLoop;
		} else {
			return RegionType::nil;
		}
	}

	// check for improper

	VertexLabel lastNode = reachUnder.last();
	if (lastNode == node) {
		lastNode = reachUnder.first();
	}

	if (mFollowers[node].size() == 2 && mFollowers[lastNode].size() == 1
				&& mPredecessors[node].size() == 2 && mPredecessors[lastNode].size() == 1) {
		return RegionType::WhileLoop;
	} else {
		return RegionType::NaturalLoop;
	}
}

graphUtils::Node *StructuralControlFlowGenerator::reduce(graphUtils::RegionType type, QVector<VertexLabel> &nodesThatComposeRegion)
{
	bool hasBackEdgeForBlock = false;
	if (type == RegionType::Block) {
		VertexLabel lastNode = nodesThatComposeRegion.last();
		VertexLabel firstNode = nodesThatComposeRegion.first();
		hasBackEdgeForBlock = mFollowers[lastNode].contains(firstNode);
	}

	Node *abstractNode = new Node(mCounter++, type);
	replace(abstractNode->id(), nodesThatComposeRegion);

	allNodes.append(abstractNode);
	for (auto it = nodesThatComposeRegion.begin(); it != nodesThatComposeRegion.end(); ++it) {
		VertexLabel currentLabel = *it;

		// place for optimization
		for (auto it1 = allNodes.begin(); it1 != allNodes.end(); ++it1) {
			Node *current = *it1;
			if (current->id() == currentLabel) {
				current->setParent(abstractNode->id());
			}
		}

		// saving graph invariant
		mFollowers.remove(currentLabel);
		mPredecessors.remove(currentLabel);
	}

	VertexLabel newLabel = abstractNode->id();
	if (hasBackEdgeForBlock) {
		mFollowers[newLabel].push_back(newLabel);
		mPredecessors[newLabel].push_back(newLabel);
	}

	// updating dominators
	QSet<VertexLabel> setForNewNode = {newLabel};
	for (auto it = nodesThatComposeRegion.begin(); it != nodesThatComposeRegion.end(); ++it) {
		setForNewNode = setForNewNode.unite(mDominators[*it]);
	}
	setForNewNode = setForNewNode.intersect(mVerteces.toSet());
	mDominators.insert(abstractNode->id(), setForNewNode);

	for (auto it = nodesThatComposeRegion.begin(); it != nodesThatComposeRegion.end(); ++it) {
		mDominators.remove(*it);
	}

	for (auto it = mDominators.begin(); it != mDominators.end(); ++it) {
		VertexLabel label = it.key();
		QSet<VertexLabel> set = it.value();
		bool isNeededNewLabel = false;

		auto vertexIt = set.begin();
		while (vertexIt != set.end()) {
			if (nodesThatComposeRegion.contains(*vertexIt)) {
				isNeededNewLabel = true;
				vertexIt = set.erase(vertexIt);
			} else {
				vertexIt++;
			}
		}

		if (isNeededNewLabel) {
			set.insert(newLabel);
		}

		mDominators.insert(label, set);
	}

	abstractNode->appendChildren(nodesThatComposeRegion);

	return abstractNode;
}

void StructuralControlFlowGenerator::replace(VertexLabel node, QVector<graphUtils::VertexLabel> &nodesThatComposeRegion)
{
	compact(node, nodesThatComposeRegion);

	QMap<VertexLabel, QVector<VertexLabel> > followers;
	followers.clear();

	// set of edges is determined by mFollowers or mPredecessors
	for (auto it = mFollowers.begin(); it != mFollowers.end(); ++it) {
		VertexLabel currentLabel = it.key();
		QVector<VertexLabel> labels = it.value();

		for (auto nextVertexIt = labels.begin(); nextVertexIt != labels.end(); nextVertexIt++) {
			VertexLabel nextLabel = *nextVertexIt;

			if (nodesThatComposeRegion.contains(nextLabel) || nodesThatComposeRegion.contains(currentLabel)) {
				removeFrom(mPredecessors, nextLabel, currentLabel);

				if (mVerteces.contains(currentLabel) && currentLabel != node) {
					if (followers.contains(currentLabel)) {
						if (!followers[currentLabel].contains(node)) {
							addTo(followers, currentLabel, node);
							addTo(mPredecessors, node, currentLabel);
						}
					} else {
						addTo(followers, currentLabel, node);
						addTo(mPredecessors, node, currentLabel);
					}
				} else if (mVerteces.contains(nextLabel) && currentLabel != node) {
					if (followers.contains(node)) {
						if (!followers[node].contains(nextLabel)) {
							addTo(followers, node, nextLabel);
							addTo(mPredecessors, nextLabel, node);
						}
					} else {
						addTo(followers, node, nextLabel);
						addTo(mPredecessors, nextLabel, node);
					}
				}

			} else {
				addTo(followers, currentLabel, nextLabel);
			}

		}
	}

	mFollowers = followers;

	// deal with Tree
	//node->appendChildren(nodesThatComposeRegion);
}

void StructuralControlFlowGenerator::compact(VertexLabel node, QVector<VertexLabel> &nodesThatComposeRegion)
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
	for (int i = 0; i <= mMaxTime; i++) {

		if (mPostOrder.values().contains(i)) {
			VertexLabel labelWithTimeI = mPostOrder.key(i);
			mPostOrder[labelWithTimeI] = appropriateTime;
			appropriateTime++;
		}
	}

	mCurrentTime = mPostOrder[node];
	mMaxTime = appropriateTime - 1;
}

void StructuralControlFlowGenerator::removeFrom(QMap<graphUtils::VertexLabel, QVector<graphUtils::VertexLabel> > &map, graphUtils::VertexLabel element, graphUtils::VertexLabel elementToRemove)
{
	auto labels = map.value(element);
	if (!labels.isEmpty()) {
		labels.removeOne(elementToRemove);
		map.insert(element, labels);
	}
}

void StructuralControlFlowGenerator::addTo(QMap<graphUtils::VertexLabel, QVector<graphUtils::VertexLabel> > &map, graphUtils::VertexLabel element, graphUtils::VertexLabel elementToAdd)
{
	auto labels = map.value(element);
	if (!labels.contains(elementToAdd)) {
		labels.push_back(elementToAdd);
		map.insert(element, labels);
	}
}

QVector<graphUtils::VertexLabel> StructuralControlFlowGenerator::countReachUnder(VertexLabel currentNode)
{
	QVector<VertexLabel> reachUnder = {currentNode};

	QMap<VertexLabel, bool> used;

	QQueue<VertexLabel> nodesThatHavePathAndBackEdgeToCurrentNode;
	// only nodes that are dominated by currentNode can be presented in reachUnder
	for (auto it = mVerteces.begin(); it != mVerteces.end(); ++it) {
		VertexLabel node = *it;
		used[node] = false;
		if (mDominators[node].contains(currentNode) && mFollowers[node].contains(currentNode)) {
			nodesThatHavePathAndBackEdgeToCurrentNode.enqueue(node);
		}
	}

	used[currentNode] = true;

	while (!nodesThatHavePathAndBackEdgeToCurrentNode.isEmpty()) {
		VertexLabel node = nodesThatHavePathAndBackEdgeToCurrentNode.dequeue();
		reachUnder.push_back(node);
		used[node] = true;
		for (auto it = mPredecessors[node].begin(); it != mPredecessors[node].end(); ++it) {
			if (!used[*it]) {
				nodesThatHavePathAndBackEdgeToCurrentNode.enqueue(*it);
			}
		}
	}

	return reachUnder;
}


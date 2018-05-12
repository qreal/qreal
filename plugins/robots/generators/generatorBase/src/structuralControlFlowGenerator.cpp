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

#include <algorithm>

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
		while (t <= mMaxPostOrderTime && mVerteces.size() > 1) {

			if (!checkAllStructures()) {
				bool flag = checkAllStructures();
			}
			Q_ASSERT(checkAllStructures());
			int v = mPostOrder.key(t);
			if (!v) {
				qDebug() << "Problem! time = " << t;
			}

			QSet<int> edgesToRemove = {};
			QMap<QString, int> vertecesRoles;
			if (isBlock(v, edgesToRemove, vertecesRoles)) {
				reduceBlock(edgesToRemove, vertecesRoles);
				qDebug() << "Block";
			} else if (isDummySwitch(v, edgesToRemove, vertecesRoles)) {
				reduceDummySwitch(edgesToRemove, vertecesRoles);
				qDebug() << "Dummy switch";
			} else if (isSwitch(v, edgesToRemove, vertecesRoles)) {
				reduceSwitch(edgesToRemove, vertecesRoles);
				qDebug() << "Switch";
			} else if (isIfThenElse(v, edgesToRemove, vertecesRoles)) {
				reduceIfThenElse(edgesToRemove, vertecesRoles);
				qDebug() << "If then else";
			} else if (isIfThen(v, edgesToRemove, vertecesRoles)) {
				reduceIfThen(edgesToRemove, vertecesRoles);
				qDebug() << "If then";
			} else if (isInfiniteLoop(v, edgesToRemove, vertecesRoles)) {
				reduceInfiniteLoop(edgesToRemove, vertecesRoles);
				qDebug() << "Infinite loop";
			} else if (isWhileLoop(v, edgesToRemove, vertecesRoles)) {
				reduceWhileLoop(edgesToRemove, vertecesRoles);
				qDebug() << "While loop";

			} else {
				QSet<int> reachUnder;
				obtainReachUnder(v, reachUnder);
				bool ok = dealWithReachUnder(v, reachUnder);
				if (ok) {
					t = 0;
					mSomethingChanged = true;
				} else {
					t++;
				}

				continue;
			}

			if (vertecesRoles.size()) {
				t -= (vertecesRoles.size() - 1);
			}
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
		for (int v : mFollowers2.keys()) {
			if (mFollowers2[v].size()) {
				qDebug() << "vertex " << v << " : ";
				for (const int u : mFollowers2[v].keys()) {
					//qDebug() << u << " ";
					int numberOfEdges = mFollowers2[v][u].size();
					qDebug() << "u = " << u << ", number of edges = " << numberOfEdges;
				}
			}
		}

		for (int v : mPredecessors2.keys()) {
			if (mPredecessors2[v].size()) {
				qDebug() << "vertex " << v << " : ";
				for (const int u : mPredecessors2[v].keys()) {
					qDebug() << u << " ";
				}
			}
		}
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
	if (mTrees.contains(v)) {
		return false;
	}

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
	if (mTrees.contains(v)) {
		return false;
	}

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
	if (numberOfUniqueOutgoingEdges(v) != 2) {
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

	for (int edge : mFollowers2[v][bodyNumber]) {
		edgesToRemove.insert(edge);
	}
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
		mVerteces.insert(mVertecesNumber);
		mVertecesNumber++;
	}
}

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

	for (const int u : mVerteces) {
		if (u != mStartVertex) {
			mDominators[u] = mVerteces;
		}
	}

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
	QQueue<int> queueForReachUnder;

	for (const int u : mPredecessors2[v].keys()) {
		if (mDominators[u].contains(v)) {
			queueForReachUnder.push_back(u);
		}
	}

	while (!queueForReachUnder.empty()) {
		int u = queueForReachUnder.front();
		queueForReachUnder.pop_front();
		reachUnder.insert(u);
		for (const int w : mPredecessors2[u].keys()) {
			if (mDominators[w].contains(v) && !reachUnder.contains(w)) {
				queueForReachUnder.push_back(w);
			}
		}
	}
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

void StructuralControlFlowGenerator::reduceIfThenElse(QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles)
{
	// ASSERT that there's no SemanticNode with key v in mTrees ?
	int v = vertecesRoles["condition"];
	int u1 = vertecesRoles["a"];
	int u2 = vertecesRoles["b"];

	qReal::Id vId = mMapVertexLabel.key(v);
	IfNode *ifNode = new IfNode(vId, mSemanticTree);
	if (thenBranchNumber(vId) == u2) {
		int temp = u1;
		u1 = u2;
		u2 = temp;
	}

	SemanticNode *thenNode = mTrees[u1];
	SemanticNode *elseNode = mTrees[u2];
	ifNode->thenZone()->appendChild(thenNode);
	ifNode->elseZone()->appendChild(elseNode);
	if (u1 == u2) {
		ifNode->transformToSimple();
	}

	appendVertex(ifNode, edgesToRemove, vertecesRoles);
}

void StructuralControlFlowGenerator::reduceIfThen(QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles)
{
	int v = vertecesRoles["condition"];
	int thenNumber = vertecesRoles["then"];
	int elseNumber = vertecesRoles["else"];

	qReal::Id vId = mMapVertexLabel.key(v);
	IfNode *ifNode = new IfNode(vId, mSemanticTree);

	if (thenBranchNumber(vId) == elseNumber) {
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
	switchNode->transformToSimple();

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
		loopNode = new LoopNode(qReal::Id(), mSemanticTree);
		loopNode->bodyZone()->appendChild(mTrees[v]);
		loopNode->bodyZone()->appendChild(mTrees[bodyNumber]);
	} else {
		qReal::Id vId = mMapVertexLabel.key(v);
		if (semanticsOf(vId) == enums::semantics::loopBlock) {
			loopNode = new LoopNode(vId, mSemanticTree);
			loopNode->bodyZone()->appendChild(mTrees[bodyNumber]);
		} else if (semanticsOf(vId) == enums::semantics::conditionalBlock) {
			loopNode = new LoopNode(vId, mSemanticTree);
			loopNode->bodyZone()->appendChild(mTrees[bodyNumber]);
			if (thenBranchNumber(vId) != bodyNumber) {
				loopNode->invertCondition();
			}
		} else if (semanticsOf(vId) == enums::semantics::switchBlock) {

			IfNode *ifNode = createIfFromSwitch(v, bodyNumber);
			ifNode->thenZone()->appendChild(SimpleNode::createBreakNode(mSemanticTree));

			loopNode = new LoopNode(qReal::Id(), mSemanticTree);
			loopNode->bodyZone()->appendChild(ifNode);
			loopNode->bodyZone()->appendChild(mTrees[bodyNumber]);
		}
	}

	appendVertex(loopNode, edgesToRemove, vertecesRoles);
}

void StructuralControlFlowGenerator::reduceConditionAndAddBreak(QSet<int> &edgesToRemove, QMap<QString, int> &vertecesRoles)
{
	int v = vertecesRoles["condition"];
	int u = vertecesRoles["then"];
	int w = vertecesRoles["exit"];

	qReal::Id vId = mMapVertexLabel.key(v);
	IfNode *ifNode = nullptr;
	if (semanticsOf(vId) == enums::semantics::conditionalBlock) {
		ifNode = new IfNode(vId, mSemanticTree);
		if (thenBranchNumber(vId) != u) {
			ifNode->invertCondition();
		}
	} else if (semanticsOf(vId) == enums::semantics::switchBlock) {
		ifNode = createIfFromSwitch(v, u);
	} else {
		qDebug() << "Problem";
	}

	if (u != w) {
		ifNode->thenZone()->appendChild(mTrees[u]);
	}

	ifNode->thenZone()->appendChild(SimpleNode::createBreakNode(mSemanticTree));

	if (u == w) {
		vertecesRoles.remove("then");
	}
	vertecesRoles.remove("exit");

	appendVertex(ifNode, edgesToRemove, vertecesRoles);
}

bool StructuralControlFlowGenerator::dealWithReachUnder(int v, QSet<int> &reachUnder)
{
	if (reachUnder.isEmpty()) {
		return false;
	}

	int commonChild = -1;
	QSet<int> exits;
	QMap<int, int> nodesWithExits;

	for (const int u : reachUnder) {
		for (const int w : mFollowers2[u].keys()) {
			if (!reachUnder.contains(w)) {
				if (exits.contains(w)) {
					if (commonChild != -1 && commonChild != w) {
						return false;
					}
					commonChild = w;
				}
				exits.insert(w);
				nodesWithExits[u] = w;
			}
		}
	}

	if (commonChild == -1) {
		QSet<int> regionToFindCommonChild;
		for (const int exitNumber : exits) {
			if (numberOfOutgoingEdges(exitNumber) == 1) {
				regionToFindCommonChild.insert(exitNumber);
			} else if (numberOfOutgoingEdges(exitNumber) > 1) {
				if (commonChild == -1) {
					// we have found post-cycle execution point
					commonChild = exitNumber;
				} else if (commonChild != exitNumber) {
					// each cycle can have at most 1 point to transfer execution
					return false;
				}
			}
		}


		// assume that one exit point is commonChild
		if (regionToFindCommonChild.size() == 1) {
			commonChild = regionToFindCommonChild.toList().first();
		}

		if (regionToFindCommonChild.size() > 1) {
			// maybe there's a common child between exits

			for (const int exit : regionToFindCommonChild) {
				if (numberOfUniqueIncomingEdges(exit) > 1) {
					if (commonChild == -1) {
						return false;
					}
				}
			}

			if (commonChild == -1) {
				for (const int exitNumber : regionToFindCommonChild) {
					for (const int u2 : mFollowers2[exitNumber].keys()) {
						if (commonChild == -1) {
							commonChild = u2;
						} else if (commonChild != u2) {
							return false;
						}
					}
				}
			}
		}
	}

	for (const int w : nodesWithExits.values()) {
		if (w == commonChild) {
			continue;
		}

		if (numberOfOutgoingEdges(w) >= 2) {
			return false;
		}
	}

	int oneSavedEdge = -1;
	for (const int u : nodesWithExits.keys()) {
		QSet<int> edgesToRemove;

		int u1 = nodesWithExits[u];
		for (int edge : mFollowers2[u][u1]) {
			if (oneSavedEdge == -1) {
				oneSavedEdge = edge;
			}
			edgesToRemove.insert(edge);
		}


		if (u1 != commonChild) {
			for (int edge : mFollowers2[u1][commonChild]) {
				edgesToRemove.insert(edge);
			}
		}

		bool needToUpdateHead = u == v;
		QMap<QString, int> vertecesRoles;
		vertecesRoles["condition"] = u;
		vertecesRoles["then"] = nodesWithExits[u];
		vertecesRoles["exit"] = commonChild;
		reduceConditionAndAddBreak(edgesToRemove, vertecesRoles);
		if (needToUpdateHead) {
			v = mVertecesNumber - 1;
		}
	}

	if (commonChild != -1) {
		mFollowers2[v][commonChild].push_back(oneSavedEdge);
		mPredecessors2[commonChild][v].push_back(oneSavedEdge);
	}

	return true;
}

int StructuralControlFlowGenerator::thenBranchNumber(const Id &id) const
{
	QPair<LinkInfo, LinkInfo> branches = ifBranchesFor(id);
	return mMapVertexLabel[branches.first.target];
}

bool StructuralControlFlowGenerator::checkAllStructures()
{
	return checkDominators() && checkFollowers() && checkPostorder();
}

bool StructuralControlFlowGenerator::checkDominators()
{
	for (const int u : mDominators.keys()) {
		QSet<int> testSet = mDominators[u];
		testSet.subtract(mVerteces);
		if (!testSet.isEmpty()) {
			return false;
		}
	}
	return true;
}

bool StructuralControlFlowGenerator::checkPostorder()
{
	QSet<int> testSet = mPostOrder.keys().toSet();
	testSet.subtract(mVerteces);
	return testSet.isEmpty();
}

bool StructuralControlFlowGenerator::checkFollowers()
{
	for (const int u : mFollowers2.keys()) {
		QSet<int> testSet = mFollowers2[u].keys().toSet();
		if (!mVerteces.contains(u) && !testSet.isEmpty()) {
			return false;
		}

		if (!testSet.subtract(mVerteces).isEmpty()) {
			return false;
		}
	}

	return true;
}

IfNode *StructuralControlFlowGenerator::createIfFromSwitch(int v, int bodyNumber)
{
	qReal::Id vId = mMapVertexLabel.key(v);
	bool needInverting = true;
	QList<qReal::Id> links;
	bool hasDefaultBranch = false;
	for (const int edge : mFollowers2[v][bodyNumber]) {
		links.append(mEdges[edge]);
		if (mRepo.property(mEdges[edge], "Guard").toString().isEmpty()) {
			hasDefaultBranch = true;
		}
	}

	if (hasDefaultBranch) {
		needInverting = false;
		links.clear();
		for (const int u : mFollowers2[v].keys()) {
			if (u == bodyNumber) {
				continue;
			}

			for (const int edge : mFollowers2[v][u]) {
				links.append(mEdges[edge]);
			}
		}
	}

	IfNode *ifNode = IfNode::fromSwitchCase(vId, links);

	if (needInverting) {
		ifNode->invertCondition();
	}

	ifNode->setCondition(constructConditionFromSwitch(vId, links));

	return ifNode;
}

QString StructuralControlFlowGenerator::constructConditionFromSwitch(const Id &id, const QList<Id> &links) const
{
	const QString expression = mRepo.property(id, "Expression").toString();
	QString condition = "";
	for (const qReal::Id &link : links) {
		condition += "(" + expression + " == " + mRepo.property(link, "Guard").toString() + ") || ";
	}
	condition.chop(4);
	return condition;
}

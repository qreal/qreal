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
	, mVerteces(1)
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
	if (isPerformingGeneration) {
		ControlFlowGeneratorBase::visit(id, links);
	} else {
		updateVerteces(id, links);
	}
}

void StructuralControlFlowGenerator::visitRegular(const Id &id, const QList<LinkInfo> &links)
{
	Q_UNUSED(links)

	identifyPatterns(id);
}

void StructuralControlFlowGenerator::visitConditional(const Id &id, const QList<LinkInfo> &links)
{
	Q_UNUSED(links)
	Region region;
	if (isIfThenElse(id, region)) {
		reduceIfThenElse(id, region);
	}

	identifyPatterns(id);
}

void StructuralControlFlowGenerator::visitLoop(const Id &id, const QList<LinkInfo> &links)
{
	Q_UNUSED(links)

	Region region;
	if (isSelfLoop(id, region)) {
		reduceSelfLoop(id, region);
	} else if (isWhileLoop(id, region)) {
		reduceWhileLoop(id, region);
	}

	identifyPatterns(id);
}

void StructuralControlFlowGenerator::visitSwitch(const Id &id, const QList<LinkInfo> &links)
{
	Region region;
	if (isSwitch(id, region)) {
		Region guards;

		for (const LinkInfo &branch : links) {
			const QString value = mRepo.property(branch.linkId, "Guard").toString();
			const int targetNumber = mMapIdToVertexLabel[branch.target];
			guards[value] = targetNumber;
		}

		reduceSwitch(id, region, guards);
	}

	identifyPatterns(id);
}

void StructuralControlFlowGenerator::visitUnknown(const Id &id, const QList<LinkInfo> &links)
{
	Q_UNUSED(links)

	identifyPatterns(id);
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
	isPerformingGeneration = false;
	mCantBeGeneratedIntoStructuredCode = false;
	ControlFlowGeneratorBase::performGeneration();
	buildGraph();

	isPerformingGeneration = true;
	ControlFlowGeneratorBase::performGeneration();

	isPerformingGeneration = false;

	if (mTrees.keys().size() == 1) {
		qDebug() << "Success!";
		mSemanticTree->setRoot(new RootNode(mTrees[mTrees.keys().first()]));
	} else {
		mCantBeGeneratedIntoStructuredCode = true;
		mSemanticTree = nullptr;
	}

}

bool StructuralControlFlowGenerator::isBlock(const Id &id, Region &region)
{
	int node = mMapIdToVertexLabel[id];
	QVector<int> outgoingList = mFollowers[node];
	if (outgoingList.size() == 1) {
		int next = outgoingList.first();
		if (mPredecessors[next].size() == 1) {
			region["head"] = node;
			region["tail"] = next;
			return true;
		}
	}

	return false;
}

bool StructuralControlFlowGenerator::isIfThenElse(const Id &id, Region &region)
{
	// it is assumed that semantics of id is If
	QPair<LinkInfo, LinkInfo> branches(ifBranchesFor(id));
	qReal::Id thenNode = branches.first.target;
	qReal::Id elseNode = branches.second.target;

	int thenNodeNumber = mMapIdToVertexLabel[thenNode];
	int elseNodeNumber = mMapIdToVertexLabel[elseNode];

	if (mFollowers[thenNodeNumber].size() == 1 && mPredecessors[thenNodeNumber].size() == 1 &&
				mFollowers[elseNodeNumber].size() == 1 && mPredecessors[elseNodeNumber].size() == 1) {

		int v = mFollowers[thenNodeNumber].first();
		int u = mFollowers[elseNodeNumber].first();
		if (v == u) {
			region["guard"] = mMapIdToVertexLabel[id];
			region["then"] = thenNodeNumber;
			region["else"] = elseNodeNumber;
			return true;
		}
	}

	if (mFollowers[thenNodeNumber].size() == 1 && mPredecessors[thenNodeNumber].size() == 1) {
		int v = mFollowers[thenNodeNumber].first();
		if (v == elseNodeNumber) {
			region["guard"] = mMapIdToVertexLabel[id];
			region["then"] = thenNodeNumber;
			return true;
		}
	}

	if (mFollowers[elseNodeNumber].size() == 1 && mPredecessors[elseNodeNumber].size() == 1) {
		int v = mFollowers[elseNodeNumber].first();
		if (v == thenNodeNumber) {
			region["guard"] = mMapIdToVertexLabel[id];
			region["else"] = elseNodeNumber;
			return true;
		}
	}

	return false;
}

bool StructuralControlFlowGenerator::isSwitch(const qReal::Id &id, Region &region)
{
	bool wasTargetSet = false;
	int targetNode = -1;
	int node = mMapIdToVertexLabel[id];
	region["switch"] = node;
	bool isSwitch = true;
	for (const int u : mFollowers[node]) {
		region["case" + QString::number(u)] = u;
		if (mFollowers[u].size() > 1 || mPredecessors[u].size() != 1) {
			isSwitch = false;
		} else if (mFollowers[node].size() == 1) {
			if (!wasTargetSet) {
				wasTargetSet = true;
				targetNode = mFollowers[node].first();
			} else if (mFollowers[node].first() != targetNode) {
				isSwitch = false;
			}
		}
	}

	if (!isSwitch) {
		region.clear();
	}

	return isSwitch;
}

bool StructuralControlFlowGenerator::isSelfLoop(const Id &id, Region &region)
{
	int node = mMapIdToVertexLabel[id];
	if (mFollowers[node].size() == 1) {
		int next = mFollowers[node].first();
		if (next == node) {
			region["selfloop"] = node;
			return true;
		}
	}

	return false;
}

bool StructuralControlFlowGenerator::isWhileLoop(const Id &id, Region &region)
{
	int node = mMapIdToVertexLabel[id];
	if (mFollowers[node].size() == 2) {
		int v = mFollowers[node].at(0);
		int u = mFollowers[node].at(1);

		if (mFollowers[v].size() == 1 && mPredecessors[v].size() == 1
					&& mFollowers[v].first() == node) {
			region["condition"] = node;
			region["body"] = v;
			return true;
		} else if (mFollowers[u].size() == 1 && mPredecessors[u].size() == 1
						&& mFollowers[u].first() == node) {
			region["condition"] = node;
			region["body"] = u;
			return true;
		}
	}

	return false;
}

bool StructuralControlFlowGenerator::isDoWhileLoop(const Id &id, StructuralControlFlowGenerator::Region &region)
{
	int node = mMapIdToVertexLabel[id];
	if (mFollowers[node].size() == 2) {
		int v = mFollowers[node].at(0);
		int u = mFollowers[node].at(1);

		if (mFollowers[v].size() == 1 && mPredecessors[v].size() == 2
					&& mFollowers[v].contains(node)) {
			region["condition"] = node;
			region["body"] = v;
			return true;
		} else if (mFollowers[u].size() == 1 && mPredecessors[u].size() == 2
						&& mFollowers[u].contains(node)) {
			region["condition"] = node;
			region["body"] = u;
			return true;
		}
	}

	return false;
}

void StructuralControlFlowGenerator::updateVerteces(const Id &id, const QList<LinkInfo> &links)
{
	Q_UNUSED(links)

	if (!mMapIdToVertexLabel.contains(id)) {
		mMapIdToVertexLabel[id] = mVerteces;
		mMapVertexLabelToId[mVerteces] = id;
		mTrees[mVerteces] = mSemanticTree->produceNodeFor(id);
		mVerteces++;
	}
}

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
		}
		else if (isWhileLoop(id, region)) {
			reduceWhileLoop(id, region);
			ok = true;
		} else if (isDoWhileLoop(id, region)) {
			reduceDoWhileLoop(id, region);
			ok = true;
		}
	}
}

void StructuralControlFlowGenerator::buildGraph()
{
	for (int vertex : mMapVertexLabelToId.keys()) {
		const qReal::Id id = mMapVertexLabelToId[vertex];

		for (const qReal::Id link : mRepo.outgoingLinks(id)) {
			const qReal::Id u = mRepo.otherEntityFromLink(link, id);
			mFollowers[vertex].push_back(mMapIdToVertexLabel[u]);
		}

		for (const qReal::Id link : mRepo.incomingLinks(id)) {
			const qReal::Id u = mRepo.otherEntityFromLink(link, id);
			mPredecessors[vertex].push_back(mMapIdToVertexLabel[u]);
		}
	}
}

void StructuralControlFlowGenerator::replace(int newNodeNumber, Region &region, bool isBlock)
{
	bool addEdgeToYourself = false;
	if (isBlock) {
		int firstNode = region["head"];
		int secondNode = region["tail"];
		if (mFollowers[secondNode].contains(firstNode)) {
			addEdgeToYourself = true;
		}
	}

	//QMap<int, QVector<int> > oldPredecessors = mPredecessors;
	QMap<int, QVector<int> > oldFollowers = mFollowers;

	for (const int u : oldFollowers.keys()) {
		for (const int v : oldFollowers[u]) {
			if (region.values().contains(u) && !region.values().contains(v)) {
				mPredecessors[v].removeOne(u);

				if (!mPredecessors[v].contains(newNodeNumber)) {
					mPredecessors[v].append(newNodeNumber);
				}

				if (!mFollowers[newNodeNumber].contains(v)) {
					mFollowers[newNodeNumber].append(v);
				}
			} else if (!region.values().contains(u) && region.values().contains(v)) {
				mFollowers[u].removeOne(v);

				if (!mFollowers[u].contains(newNodeNumber)) {
					mFollowers[u].append(newNodeNumber);
				}

				if (!mPredecessors[newNodeNumber].contains(u)) {
					mPredecessors[newNodeNumber].append(u);
				}
			}
		}
	}

	for (const int u : region) {
		mFollowers.remove(u);
		mPredecessors.remove(u);
	}

	if (addEdgeToYourself) {
		mFollowers[newNodeNumber].push_back(newNodeNumber);
		mPredecessors[newNodeNumber].push_back(newNodeNumber);
	}
}

void StructuralControlFlowGenerator::reduceBlock(const Id &id, Region &region)
{
	int idNumber = mMapIdToVertexLabel[id];
	ZoneNode *newNode = new ZoneNode(mTrees[idNumber]->parent());
	newNode->appendChild(mTrees[region["head"]]);
	newNode->appendChild(mTrees[region["tail"]]);

	mTrees.remove(region["head"]);
	mTrees.remove(region["tail"]);

	int newNodeNumber = mVerteces++;
	mMapVertexLabelToId[newNodeNumber] = id;
	mMapIdToVertexLabel[id] = newNodeNumber;
	mTrees[newNodeNumber] = newNode;

	mMapVertexLabelToId.remove(region["head"]);
	mMapVertexLabelToId.remove(region["tail"]);

	replace(newNodeNumber, region, true);
}

void StructuralControlFlowGenerator::reduceIfThenElse(const Id &id, Region &region)
{
	int conditionNumber = region["guard"];
	mMapVertexLabelToId.remove(conditionNumber);

	semantics::IfNode *ifNode = static_cast<semantics::IfNode *>(mTrees[conditionNumber]);
	if (region.contains("then")) {
		int thenBranchNumber = region["then"];
		ifNode->thenZone()->appendChild(mTrees[thenBranchNumber]);
		mTrees.remove(thenBranchNumber);
		mMapVertexLabelToId.remove(thenBranchNumber);
	}

	if (region.contains("else")) {
		int elseBranchNumber = region["else"];
		ifNode->elseZone()->appendChild(mTrees[elseBranchNumber]);
		mTrees.remove(elseBranchNumber);
		mMapVertexLabelToId.remove(elseBranchNumber);
	}

	mTrees.remove(conditionNumber);
	int newNodeNumber = mVerteces++;
	mMapVertexLabelToId[newNodeNumber] = id;
	mMapIdToVertexLabel[id] = newNodeNumber;
	mTrees[newNodeNumber] = ifNode;

	replace(newNodeNumber, region, false);
}

void StructuralControlFlowGenerator::reduceSelfLoop(const Id &id, Region &region)
{
	const qReal::Id emptyIdForInfiniteLoop = qReal::Id();
	int idNumber = mMapIdToVertexLabel[id];
	semantics::LoopNode *selfLoop = new semantics::LoopNode(emptyIdForInfiniteLoop, mTrees[idNumber]->parent());
	int loopBodyNumber = region["selfloop"];
	selfLoop->bodyZone()->appendChild(mTrees[loopBodyNumber]);

	int newNodeNumber = mVerteces++;
	mMapVertexLabelToId[newNodeNumber] = id;
	mMapIdToVertexLabel[id] = newNodeNumber;
	mTrees[newNodeNumber] = selfLoop;

	mTrees.remove(loopBodyNumber);
	replace(newNodeNumber, region, false);
}

void StructuralControlFlowGenerator::reduceWhileLoop(const Id &id, Region &region)
{
	int loopConditionNumber = region["condition"];
	int loopBodyNumber = region["body"];

	semantics::LoopNode *whileLoop = new semantics::LoopNode(id, mTrees[loopConditionNumber]->parent());
	whileLoop->bodyZone()->appendChild(mTrees[loopBodyNumber]);

	int newNodeNumber = mVerteces++;
	mMapVertexLabelToId[newNodeNumber] = id;
	mMapIdToVertexLabel[id] = newNodeNumber;
	mTrees[newNodeNumber] = whileLoop;

	mTrees.remove(loopConditionNumber);
	mTrees.remove(loopBodyNumber);

	replace(newNodeNumber, region, false);
}

void StructuralControlFlowGenerator::reduceDoWhileLoop(const Id &id, StructuralControlFlowGenerator::Region &region)
{
	int loopConditionNumber = region["condition"];
	int loopBodyNumber = region["body"];

	semantics::LoopNode *whileLoop = new semantics::LoopNode(id, mTrees[loopConditionNumber]->parent());
	whileLoop->bodyZone()->appendChild(mTrees[loopBodyNumber]);
	whileLoop->setForm(true);

	int newNodeNumber = mVerteces++;
	mMapVertexLabelToId[newNodeNumber] = id;
	mMapIdToVertexLabel[id] = newNodeNumber;
	mTrees[newNodeNumber] = whileLoop;

	mTrees.remove(loopConditionNumber);
	mTrees.remove(loopBodyNumber);

	replace(newNodeNumber, region, false);
}

void StructuralControlFlowGenerator::reduceSwitch(const Id &id, Region &region, Region &guards)
{
	QMap<int, bool> visitedBranches;
	for (const int v : region) {
		visitedBranches[v] = false;
	}

	int node = mMapIdToVertexLabel[id];
	SwitchNode * const switchNode = static_cast<SwitchNode *>(mTrees[node]);

	for (const QString &value : guards.keys()) {
		int v = guards[value];
		if (visitedBranches[v]) {
			NonZoneNode * const target = static_cast<NonZoneNode *>(mTrees[v]);
			switchNode->mergeBranch(value, target);
		} else {
			switchNode->addBranch(value, mTrees[v]);
			visitedBranches[v] = true;
		}
	}

	int newNodeNumber = mVerteces++;
	mMapVertexLabelToId[newNodeNumber] = id;
	mMapIdToVertexLabel[id] = newNodeNumber;
	mTrees[newNodeNumber] = switchNode;

	for (const int u : region) {
		mTrees.remove(u);
		mMapVertexLabelToId.remove(u);
	}

	replace(newNodeNumber, region, false);
}

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

	calculatePredecessors();
	findDominators();

}

void StructuralControlFlowGenerator::buildGraph(const Id &id, const QList<LinkInfo> &links)
{
	int numberOfCurrentVertex = -1;
	if (mNumbersOfElements.contains(id)) {
		numberOfCurrentVertex = mNumbersOfElements[id];
	} else {
		numberOfCurrentVertex = mNumberOfVerteces;
		mNumberOfVerteces++;
		mNumbersOfElements[id] = numberOfCurrentVertex;
	}

	for (const LinkInfo link : links) {
		int targetNumber = -1;
		if (mNumbersOfElements.contains(link.target)) {
			targetNumber = mNumbersOfElements[link.target];
		} else {
			targetNumber = mNumberOfVerteces;
			mNumberOfVerteces++;
			mNumbersOfElements[link.target] = targetNumber;
		}
		mGraph[numberOfCurrentVertex].push_back(targetNumber);
	}
}

void StructuralControlFlowGenerator::calculatePredecessors()
{
	const int root = 0;
	mPredecessors[root].clear();

	for (int i = 0; i < mNumberOfVerteces; i++) {
		for (size_t t = 0; t < mGraph[i].size(); t++) {
			int destination = mGraph[i].at(t);
			mPredecessors[destination].push_back(i);
		}
	}
}

void StructuralControlFlowGenerator::findDominators()
{
	const int root = 0;
	QSet<int> allVerteces;

	for (int i = 0; i < mNumberOfVerteces; i++) {
		allVerteces.insert(i);
	}

	mDominators[root].insert(root);

	for (int i = 1; i < mNumberOfVerteces; i++) {
		mDominators[i] = allVerteces;
	}

	bool somethingChanged = true;
	while (somethingChanged) {
		somethingChanged = false;
		QSet<int> newDominators = allVerteces;
		for (int i = 1; i < mNumberOfVerteces; i++) {
			for (size_t t = 0; t < mPredecessors[i].size(); t++) {
				int predecessor = mPredecessors[i].at(t);
				newDominators.intersect(mDominators[predecessor]);
			}
			// adding the current number, because reflexivity of dominance relation
			newDominators.insert(i);
			if (newDominators != mDominators[i]) {
				somethingChanged = true;
				mDominators[i] = newDominators;
			}
		}
	}

	// for debugging
	for (int i = 0; i < mNumberOfVerteces; i++) {
		qDebug() << i << " : " << mDominators[i] << endl;
	}
}






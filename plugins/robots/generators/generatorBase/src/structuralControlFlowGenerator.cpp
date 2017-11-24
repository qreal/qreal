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
			int targetNumber = mNumberOfVerteces;
			mNumberOfVerteces++;
			mNumbersOfElements[link.target] = targetNumber;
		}
		mGraph[numberOfCurrentVertex].push_back(targetNumber);
	}
}






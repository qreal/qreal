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

#include "pioneerStateMachineGenerator.h"

#include <generatorBase/semanticTree/semanticNode.h>
#include <generatorBase/semanticTree/simpleNode.h>

using namespace pioneer::lua;
using namespace generatorBase;
using namespace generatorBase::semantics;

PioneerStateMachineGenerator::PioneerStateMachineGenerator(
		const qrRepo::RepoApi &repo
		, qReal::ErrorReporterInterface &errorReporter
		, generatorBase::GeneratorCustomizer &customizer
		, generatorBase::PrimaryControlFlowValidator &validator
		, const qReal::Id &diagramId
		, QObject *parent
		, bool isThisDiagramMain)
	: GotoControlFlowGenerator(repo, errorReporter, customizer, validator, diagramId, parent, isThisDiagramMain)
{
}

void PioneerStateMachineGenerator::visitRegular(const qReal::Id &id, const QList<LinkInfo> &links)
{
	ControlFlowGeneratorBase::visitRegular(id, links);
	SimpleNode * const thisNode = static_cast<SimpleNode *>(mSemanticTree->findNodeFor(id));
	SemanticNode *nextNode = nullptr;
	if (mSemanticTree->findNodeFor(links[0].target)) {
		nextNode = produceGotoNode(links[0].target);
		thisNode->insertSiblingAfterThis(nextNode);
	} else {
		nextNode = mSemanticTree->produceNodeFor(links[0].target);
		nextNode->addLabel();
		SemanticNode *gotoNode = produceGotoNode(links[0].target);
		thisNode->insertSiblingAfterThis(gotoNode);
		dynamic_cast<NonZoneNode *>(gotoNode)->insertSiblingAfterThis(nextNode);
	}
}

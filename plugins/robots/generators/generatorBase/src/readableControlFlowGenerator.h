/* Copyright 2007-2015 QReal Research Group
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

#pragma once

#include "generatorBase/robotsGeneratorDeclSpec.h"
#include "generatorBase/controlFlowGeneratorBase.h"
#include "generatorBase/semanticTree/semanticTree.h"
#include "rules/semanticTransformationRule.h"

namespace generatorBase {

/// Generates semantic tree in readable style. Code is represented in readable by
/// human style: it consists of if-operators, loop-blocks, switches and so on.
/// If control flow can`t be represented with structured code generation process
/// is considered to be unsuccessfull.
class ROBOTS_GENERATOR_EXPORT ReadableControlFlowGenerator : public ControlFlowGeneratorBase
{
	Q_OBJECT

public:
	ReadableControlFlowGenerator(
			const qrRepo::RepoApi &repo
			, qReal::ErrorReporterInterface &errorReporter
			, GeneratorCustomizer &customizer
			, PrimaryControlFlowValidator &validator
			, const qReal::Id &diagramId
			, QObject *parent = 0
			, bool isThisDiagramMain = true);

	/// Implementation of clone operation for readable generator
	ControlFlowGeneratorBase *cloneFor(const qReal::Id &diagramId, bool cloneForNewDiagram) override;

	void beforeSearch() override;

	void visitRegular(const qReal::Id &id, const QList<LinkInfo> &links) override;
	void visitConditional(const qReal::Id &id, const QList<LinkInfo> &links) override;
	void visitLoop(const qReal::Id &id, const QList<LinkInfo> &links) override;
	void visitSwitch(const qReal::Id &id, const QList<LinkInfo> &links) override;

	/// This method can be used for semantic tree debug printing after all
	/// traversal stages.
	void afterSearch() override;

	/// Returns true if some generation errors occured but the generation process may be proceeded with other
	/// control flow generators (non-fatal errors occured).
	bool cantBeGeneratedIntoStructuredCode() const;

private:
	/// Implementation of generation process for readable generator.
	/// Important: the graph in the model would be traversed two or more times
	/// for the emulation of some priority for semantic rules.
	void performGeneration() override;

	bool applyFirstPossible(const qReal::Id &currentId
			, QList<semantics::SemanticTransformationRule *> const &rules
			, bool thereWillBeMoreRules);

	bool mTravelingForSecondTime;
	bool mSomethingChangedThisIteration;
	QMap<qReal::Id, bool> mAlreadyApplied;
	bool mCantBeGeneratedIntoStructuredCode;
};

}

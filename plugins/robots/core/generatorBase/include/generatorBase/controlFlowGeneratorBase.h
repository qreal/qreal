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

#include "robotsGeneratorDeclSpec.h"
#include "primaryControlFlowValidator.h"
#include "robotsDiagramVisitor.h"
#include "semanticTree/semanticTree.h"

namespace generatorBase {

/// A base class for generators that builds sementic tree from model in repo
class ROBOTS_GENERATOR_EXPORT ControlFlowGeneratorBase : public QObject, public RobotsDiagramVisitor
{
	Q_OBJECT

public:
	/// @param isThisDiagramMain 'true' if this generator generates code for main diagram
	/// (main diagram is the one which was active when user requested generation)
	ControlFlowGeneratorBase(
			const qrRepo::RepoApi &repo
			, qReal::ErrorReporterInterface &errorReporter
			, GeneratorCustomizer &customizer
			, PrimaryControlFlowValidator &validator
			, const qReal::Id &diagramId
			, QObject *parent = 0
			, bool isThisDiagramMain = true);
	virtual ~ControlFlowGeneratorBase();

	/// Validates diagram checking if given model in repo satisfies the simplest
	/// conditions (like all links are connected and correctly marked and so on)
	bool preGenerationCheck();

	/// Copies this generator and returns new instance which is owned by the same
	/// parent. Implementation must pay attention to isThisDiagramMain parameter
	/// (it should be always false in copied objects).
	/// Ownership on the cloned generators is taken by this generator`s parent.
	virtual ControlFlowGeneratorBase *cloneFor(const qReal::Id &diagramId, bool cloneForNewDiagram) = 0;

	/// Generates control flow object representation (SemanticTree) and returns
	/// a pointer to it if generation process was successfull or nullptr otherwise.
	/// Takes ownership on result.
	/// @param initialNode The starting block of the traversal. If empty then initial node
	/// of the diagram given in constructor will be used.
	/// @param threadId The name of a thread that is to be generated.
	semantics::SemanticTree *generate(const qReal::Id &initialNode = qReal::Id(), const QString &threadId = "main");

	/// Returns true if some generation errors occured and the generation process can`t be proceeded with other
	/// control flow generators (fatal errors occured).
	bool errorsOccured() const;

	void visitRegular(const qReal::Id &id, const QList<LinkInfo> &links) override;
	void visitFinal(const qReal::Id &id, const QList<LinkInfo> &links) override;
	void visitFork(const qReal::Id &id, QList<LinkInfo> &links) override;
	void visitJoin(const qReal::Id &id, QList<LinkInfo> &links) override;

protected:
	/// Can be overloaded by ancestors for custom behaviour.
	virtual void performGeneration();

	bool generateForks();

	void error(const QString &message, const qReal::Id &id = qReal::Id(), bool critical = true);

	enums::semantics::Semantics semanticsOf(const qReal::Id &id) const;
	qReal::Id initialNode() const;
	QPair<LinkInfo, LinkInfo> ifBranchesFor(const qReal::Id &id) const;
	QPair<LinkInfo, LinkInfo> loopBranchesFor(const qReal::Id &id) const;

	GeneratorCustomizer &customizer() const;

	semantics::SemanticTree *mSemanticTree;  // Takes ownership
	const qrRepo::RepoApi &mRepo;
	qReal::ErrorReporterInterface &mErrorReporter;
	GeneratorCustomizer &mCustomizer;
	QString mThreadId;
	bool mErrorsOccured;
	const bool mIsMainGenerator;

	const qReal::Id mDiagram;
	PrimaryControlFlowValidator &mValidator;
};

}

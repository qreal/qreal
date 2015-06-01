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

#include <qrkernel/ids.h>
#include <qrrepo/repoApi.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include "generatorBase/generatorCustomizer.h"
#include "robotsDiagramVisitor.h"

#include "robotsGeneratorDeclSpec.h"

namespace generatorBase {

typedef utils::DeepFirstSearcher::LinkInfo LinkInfo;

/// Base class for all validators of robot diagrams.
/// Default implementation validates given diagram checking all nessesary for each generator conditions
/// (like all links are connected correctly marked and so on). It also collects
/// simplest info about diagram (like initial node id, if/then branches and so on).
class ROBOTS_GENERATOR_EXPORT PrimaryControlFlowValidator : public QObject, public RobotsDiagramVisitor
{
public:
	PrimaryControlFlowValidator(const qrRepo::RepoApi &repo
			, qReal::ErrorReporterInterface &errorReporter
			, GeneratorCustomizer &customizer
			, QObject *parent = 0);

	/// Validates given diagram assuming that execution of the diagram starts in a thread with given id.
	virtual bool validate(const qReal::Id &diagramId, const QString &threadId);

	/// Returns a pointer to a copy of the validator. The copy will be owned by the parent of the original validator.
	virtual PrimaryControlFlowValidator *clone();

	/// Returns id of the only node with initial semantics on diagram. The result
	/// is ready only after validation process was successfully finished.
	qReal::Id initialNode() const;

	/// Returns branches of the given block with if semantics. First value in the
	/// resulting pair is the first block of 'then' branch, second - 'else'. The
	/// result is ready only after validation process was successfully finished.
	QPair<LinkInfo, LinkInfo> ifBranchesFor(const qReal::Id &id) const;

	/// Returns branches of the given block with loop semantics. First value in the
	/// resulting pair is the first block of 'body' branch, second - of the
	/// non-marked one. The result is ready only after validation process was
	/// successfully finished.
	QPair<LinkInfo, LinkInfo> loopBranchesFor(const qReal::Id &id) const;

protected:
	void findInitialNode();
	void error(const QString &message, const qReal::Id &id);
	bool checkForConnected(const LinkInfo &link);

	void visitRegular(const qReal::Id &id, const QList<LinkInfo> &links) override;
	void visitFinal(const qReal::Id &id, const QList<LinkInfo> &links) override;
	void visitConditional(const qReal::Id &id, const QList<LinkInfo> &links) override;
	void visitLoop(const qReal::Id &id, const QList<LinkInfo> &links) override;
	void visitSwitch(const qReal::Id &id, const QList<LinkInfo> &links) override;
	void visitFork(const qReal::Id &id, QList<LinkInfo> &links) override;
	void visitJoin(const qReal::Id &id, QList<LinkInfo> &links) override;
	void visitUnknown(const qReal::Id &id, const QList<LinkInfo> &links) override;

	const qrRepo::RepoApi &mRepo;
	qReal::ErrorReporterInterface &mErrorReporter;
	GeneratorCustomizer &mCustomizer;
	qReal::Id mDiagram;
	bool mErrorsOccured;

	qReal::Id mInitialNode;
	QMap<qReal::Id, QPair<LinkInfo, LinkInfo> > mIfBranches;
	QMap<qReal::Id, QPair<LinkInfo, LinkInfo> > mLoopBranches;
};

}

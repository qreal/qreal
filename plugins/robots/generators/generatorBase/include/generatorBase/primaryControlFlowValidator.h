#pragma once

#include <qrkernel/ids.h>
#include <qrrepo/repoApi.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include "generatorBase/generatorCustomizer.h"
#include "robotsDiagramVisitor.h"

namespace generatorBase {

typedef utils::DeepFirstSearcher::LinkInfo LinkInfo;

/// Validates given diagram checking all nessesary for each generator conditions
/// (like all links are connected correctly marked and so on). Also collects
/// simplest info about diagram (like initial node id, if/then branches and so on).
class PrimaryControlFlowValidator : public RobotsDiagramVisitor
{
public:
	PrimaryControlFlowValidator(qrRepo::RepoApi const &repo
			, qReal::ErrorReporterInterface &errorReporter
			, GeneratorCustomizer &customizer
			, qReal::Id const &diagramId);

	/// Validates diagram with id specified in constructor. Returns 'true' if
	/// diagram is correct, 'false' otherwise
	bool validate();

	/// Returns id of the only node with initial semantics on diagram. The result
	/// is ready only after validation process was successfully finished.
	qReal::Id initialNode() const;

	/// Returns branches of the given block with if semantics. First value in the
	/// resulting pair is the first block of 'then' branch, second - 'else'. The
	/// result is ready only after validation process was successfully finished.
	QPair<LinkInfo, LinkInfo> ifBranchesFor(qReal::Id const &id) const;

	/// Returns branches of the given block with loop semantics. First value in the
	/// resulting pair is the first block of 'iteration' branch, second - of the
	/// non-marked one. The result is ready only after validation process was
	/// successfully finished.
	QPair<LinkInfo, LinkInfo> loopBranchesFor(qReal::Id const &id) const;

private:
	void findInitialNode();
	void error(QString const &message, qReal::Id const &id);
	bool checkForConnected(LinkInfo const &link);

	void visitRegular(qReal::Id const &id, QList<LinkInfo> const &links) override;
	void visitFinal(qReal::Id const &id, QList<LinkInfo> const &links) override;
	void visitConditional(qReal::Id const &id, QList<LinkInfo> const &links) override;
	void visitLoop(qReal::Id const &id, QList<LinkInfo> const &links) override;
	void visitSwitch(qReal::Id const &id, QList<LinkInfo> const &links) override;
	void visitFork(qReal::Id const &id, QList<LinkInfo> &links) override;
	void visitUnknown(qReal::Id const &id, QList<LinkInfo> const &links) override;

	qrRepo::RepoApi const &mRepo;
	qReal::ErrorReporterInterface &mErrorReporter;
	GeneratorCustomizer &mCustomizer;
	qReal::Id const mDiagram;
	bool mErrorsOccured;

	qReal::Id mInitialNode;
	QMap<qReal::Id, QPair<LinkInfo, LinkInfo> > mIfBranches;
	QMap<qReal::Id, QPair<LinkInfo, LinkInfo> > mLoopBranches;
};

}

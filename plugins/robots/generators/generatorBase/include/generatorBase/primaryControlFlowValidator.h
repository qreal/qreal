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
class PrimaryControlFlowValidator : public QObject, public RobotsDiagramVisitor
{
public:
	PrimaryControlFlowValidator(const qrRepo::RepoApi &repo
			, qReal::ErrorReporterInterface &errorReporter
			, GeneratorCustomizer &customizer
			, const qReal::Id &diagramId
			, QObject *parent = 0);

	/// Validates diagram with id specified in constructor. Returns 'true' if
	/// diagram is correct, 'false' otherwise
	bool validate(const QString &threadId);

	/// Returns id of the only node with initial semantics on diagram. The result
	/// is ready only after validation process was successfully finished.
	qReal::Id initialNode() const;

	/// Returns branches of the given block with if semantics. First value in the
	/// resulting pair is the first block of 'then' branch, second - 'else'. The
	/// result is ready only after validation process was successfully finished.
	QPair<LinkInfo, LinkInfo> ifBranchesFor(const qReal::Id &id) const;

	/// Returns branches of the given block with loop semantics. First value in the
	/// resulting pair is the first block of 'iteration' branch, second - of the
	/// non-marked one. The result is ready only after validation process was
	/// successfully finished.
	QPair<LinkInfo, LinkInfo> loopBranchesFor(const qReal::Id &id) const;

private:
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
	const qReal::Id mDiagram;
	bool mErrorsOccured;

	qReal::Id mInitialNode;
	QMap<qReal::Id, QPair<LinkInfo, LinkInfo> > mIfBranches;
	QMap<qReal::Id, QPair<LinkInfo, LinkInfo> > mLoopBranches;
};

}

#pragma once

#include <qrkernel/ids.h>
#include <qrrepo/repoApi.h>
#include <qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include "generatorBase/generatorCustomizer.h"
#include "robotsDiagramVisitor.h"

namespace qReal {
namespace robots {
namespace generators {

typedef utils::DeepFirstSearcher::LinkInfo LinkInfo;

/// Validates given diagram checking all nessesary for each generator conditions
/// (like all links are connected correctly marked and so on). Also collects
/// simplest info about diagram (like initial node id, if/then branches and so on).
class PrimaryControlFlowValidator : public RobotsDiagramVisitor
{
public:
	PrimaryControlFlowValidator(qrRepo::RepoApi const &repo
			, ErrorReporterInterface &errorReporter
			, GeneratorCustomizer &customizer
			, Id const &diagramId);
	virtual ~PrimaryControlFlowValidator();

	/// Validates diagram with id specified in constructor. Returns 'true' if
	/// diagram is correct, 'false' otherwise
	bool validate();

	/// Returns id of the only node with initial semantics on diagram. The result
	/// is ready only after validation process was successfully finished.
	Id initialNode() const;

	/// Returns branches of the given block with if semantics. First value in the
	/// resulting pair is the first block of 'then' branch, second - 'else'. The
	/// result is ready only after validation process was successfully finished.
	QPair<LinkInfo, LinkInfo> ifBranchesFor(Id const &id) const;

	/// Returns branches of the given block with loop semantics. First value in the
	/// resulting pair is the first block of 'iteration' branch, second - of the
	/// non-marked one. The result is ready only after validation process was
	/// successfully finished.
	QPair<LinkInfo, LinkInfo> loopBranchesFor(Id const &id) const;

private:
	void findInitialNode();
	void error(QString const &message, Id const &id);
	bool checkForConnected(LinkInfo const &link);

	virtual void visitRegular(Id const &id, QList<LinkInfo> const &links);
	virtual void visitFinal(Id const &id, QList<LinkInfo> const &links);
	virtual void visitConditional(Id const &id, QList<LinkInfo> const &links);
	virtual void visitLoop(Id const &id, QList<LinkInfo> const &links);
	virtual void visitSwitch(Id const &id, QList<LinkInfo> const &links);
	virtual void visitFork(Id const &id, QList<LinkInfo> const &links);
	virtual void visitUnknown(Id const &id, QList<LinkInfo> const &links);

	qrRepo::RepoApi const &mRepo;
	ErrorReporterInterface &mErrorReporter;
	GeneratorCustomizer &mCustomizer;
	Id const mDiagram;
	bool mErrorsOccured;

	Id mInitialNode;
	QMap<Id, QPair<LinkInfo, LinkInfo> > mIfBranches;
	QMap<Id, QPair<LinkInfo, LinkInfo> > mLoopBranches;
};

}
}
}

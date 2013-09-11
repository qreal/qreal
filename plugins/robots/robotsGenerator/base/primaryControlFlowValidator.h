#pragma once

#include <qrkernel/ids.h>
#include <qrrepo/repoApi.h>
#include <qrgui/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include "generatorCustomizer.h"
#include "robotsDiagramVisitor.h"

namespace qReal {
namespace robots {
namespace generators {

typedef utils::DeepFirstSearcher::LinkInfo LinkInfo;

class PrimaryControlFlowValidator : public RobotsDiagramVisitor
{
public:
	PrimaryControlFlowValidator(qrRepo::RepoApi const &repo
			, ErrorReporterInterface &errorReporter
			, GeneratorCustomizer &customizer
			, Id const &diagramId);
	virtual ~PrimaryControlFlowValidator();

	bool validate();

	Id initialNode() const;
	QPair<LinkInfo, LinkInfo> ifBranchesFor(Id const &id) const;
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

#pragma once

#include "controlFlowGeneratorBase.h"

namespace qReal {
namespace robots {
namespace generators {

/// Generates semantic tree in goto-style: control flow consists of goto
/// instructions modeling links transitions in model. The code is not readable,
/// but it works always, so it can be used for uploading into robot.
class GotoControlFlowGenerator : public ControlFlowGeneratorBase
{
public:
	GotoControlFlowGenerator(
			qrRepo::RepoApi const &repo
			, ErrorReporterInterface &errorReporter
			, GeneratorCustomizer &customizer
			, Id const &diagramId
			, QObject *parent = 0
			, bool isThisDiagramMain = true);


	/// Implementation of clone operation for goto generator
	ControlFlowGeneratorBase *cloneFor(Id const &diagramId) override;

	void beforeSearch();

	void visitRegular(Id const &id, QList<LinkInfo> const &links) override;
	void visitFinal(Id const &id, QList<LinkInfo> const &links) override;
	void visitConditional(Id const &id, QList<LinkInfo> const &links) override;
	void visitLoop(Id const &id, QList<LinkInfo> const &links) override;
	void visitSwitch(Id const &id, QList<LinkInfo> const &links) override;
	void visitFork(Id const &id, QList<LinkInfo> const &links) override;

	/// This method can be used for semantic tree debug printing after all
	/// traversal stages.
	void afterSearch() override;

private:
	semantics::SemanticNode *produceGotoNode(Id const &id);
	void produceNextNodeIfNeeded(LinkInfo const &info, semantics::NonZoneNode * const parent);
};

}
}
}

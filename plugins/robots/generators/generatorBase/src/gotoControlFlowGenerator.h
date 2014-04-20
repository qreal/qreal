#pragma once

#include "generatorBase/controlFlowGeneratorBase.h"

namespace generatorBase {

/// Generates semantic tree in goto-style: control flow consists of goto
/// instructions modeling links transitions in model. The code is not readable,
/// but it works always, so it can be used for uploading into robot.
class GotoControlFlowGenerator : public ControlFlowGeneratorBase
{
public:
	GotoControlFlowGenerator(
			qrRepo::RepoApi const &repo
			, qReal::ErrorReporterInterface &errorReporter
			, GeneratorCustomizer &customizer
			, qReal::Id const &diagramId
			, QObject *parent = 0
			, bool isThisDiagramMain = true);


	/// Implementation of clone operation for goto generator
	ControlFlowGeneratorBase *cloneFor(qReal::Id const &diagramId) override;

	void beforeSearch();

	void visitRegular(qReal::Id const &id, QList<LinkInfo> const &links) override;
	void visitFinal(qReal::Id const &id, QList<LinkInfo> const &links) override;
	void visitConditional(qReal::Id const &id, QList<LinkInfo> const &links) override;
	void visitLoop(qReal::Id const &id, QList<LinkInfo> const &links) override;
	void visitSwitch(qReal::Id const &id, QList<LinkInfo> const &links) override;
	void visitFork(qReal::Id const &id, QList<LinkInfo> const &links) override;

	/// This method can be used for semantic tree debug printing after all
	/// traversal stages.
	void afterSearch() override;

private:
	semantics::SemanticNode *produceGotoNode(qReal::Id const &id);
	void produceNextNodeIfNeeded(LinkInfo const &info, semantics::NonZoneNode * const parent);
};

}

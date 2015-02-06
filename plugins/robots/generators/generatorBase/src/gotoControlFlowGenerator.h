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
			const qrRepo::RepoApi &repo
			, qReal::ErrorReporterInterface &errorReporter
			, GeneratorCustomizer &customizer
			, const qReal::Id &diagramId
			, QObject *parent = 0
			, bool isThisDiagramMain = true);


	/// Implementation of clone operation for goto generator
	ControlFlowGeneratorBase *cloneFor(const qReal::Id &diagramId, bool cloneForNewDiagram) override;

	void beforeSearch();

	void visitRegular(const qReal::Id &id, QList<LinkInfo> const &links) override;
	void visitConditional(const qReal::Id &id, QList<LinkInfo> const &links) override;
	void visitLoop(const qReal::Id &id, QList<LinkInfo> const &links) override;
	void visitSwitch(const qReal::Id &id, QList<LinkInfo> const &links) override;

	/// This method can be used for semantic tree debug printing after all
	/// traversal stages.
	void afterSearch() override;

private:
	semantics::SemanticNode *produceGotoNode(const qReal::Id &id);
	void produceNextNodeIfNeeded(LinkInfo const &info, semantics::NonZoneNode * const parent);
};

}

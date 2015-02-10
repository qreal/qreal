#pragma once

#include <qrrepo/repoApi.h>
#include <qrutils/graphUtils/deepFirstSearcher.h>

#include "generatorBase/generatorCustomizer.h"

namespace generatorBase {

/// A base for those entities who must travel through model and process every block
/// differentiating their semantics
class RobotsDiagramVisitor : public utils::DeepFirstSearcher::VisitorInterface
{
public:
	RobotsDiagramVisitor(const qrRepo::RepoApi &repo, GeneratorCustomizer &customizer);
	virtual ~RobotsDiagramVisitor();

protected:
	// TODO: move this enum to global robots definitions and use it
	/// A property that used for marking links in model
	enum LinkGuard
	{
		emptyGuard = 0
		, iterationGuard
		, trueGuard
		, falseGuard
	};

	/// Starts repo graph dfs-traversal
	void startSearch(const qReal::Id &startingBlock);

	/// Terminates current graph traversal
	void terminateSearch();

	/// Parses guard property of given link in repo-representation and returns
	/// corresponding value of LinkGuard enumeration
	LinkGuard guardOf(const qReal::Id &linkId) const;

	/// This method is called when traverser gets into a block with regular semantics
	virtual void visitRegular(const qReal::Id &id, QList<utils::DeepFirstSearcher::LinkInfo> const &links) = 0;

	/// This method is called when traverser gets into a block with final-block semantics
	virtual void visitFinal(const qReal::Id &id, QList<utils::DeepFirstSearcher::LinkInfo> const &links) = 0;

	/// This method is called when traverser gets into a block with if semantics
	virtual void visitConditional(const qReal::Id &id, QList<utils::DeepFirstSearcher::LinkInfo> const &links) = 0;

	/// This method is called when traverser gets into a block with loop semantics
	virtual void visitLoop(const qReal::Id &id, QList<utils::DeepFirstSearcher::LinkInfo> const &links) = 0;

	/// This method is called when traverser gets into a block with switch semantics
	virtual void visitSwitch(const qReal::Id &id, QList<utils::DeepFirstSearcher::LinkInfo> const &links) = 0;

	/// This method is called when traverser gets into a block with fork semantics
	virtual void visitFork(const qReal::Id &id, QList<utils::DeepFirstSearcher::LinkInfo> &links) = 0;

	/// This method is called when traverser gets into a block with unknown semantics
	virtual void visitUnknown(const qReal::Id &id, QList<utils::DeepFirstSearcher::LinkInfo> const &links);

private:
	virtual void visit(const qReal::Id &nodeId, QList<utils::DeepFirstSearcher::LinkInfo> &links);

	const qrRepo::RepoApi &mRepo;
	GeneratorCustomizer &mCustomizer;
	utils::DeepFirstSearcher mDfser;
};

}

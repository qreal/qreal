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
	RobotsDiagramVisitor(qrRepo::RepoApi const &repo, GeneratorCustomizer &customizer);
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
		, threadIdGuard
	};

	/// Starts repo graph dfs-traversal
	void startSearch(qReal::Id const &startingBlock);

	/// Terminates current graph traversal
	void terminateSearch();

	/// Parses guard property of given link in repo-representation and returns
	/// corresponding value of LinkGuard enumeration
	LinkGuard guardOf(qReal::Id const &linkId) const;

	/// This method is called when traverser gets into a block with regular semantics
	virtual void visitRegular(qReal::Id const &id, QList<utils::DeepFirstSearcher::LinkInfo> const &links) = 0;

	/// This method is called when traverser gets into a block with final-block semantics
	virtual void visitFinal(qReal::Id const &id, QList<utils::DeepFirstSearcher::LinkInfo> const &links) = 0;

	/// This method is called when traverser gets into a block with if semantics
	virtual void visitConditional(qReal::Id const &id, QList<utils::DeepFirstSearcher::LinkInfo> const &links) = 0;

	/// This method is called when traverser gets into a block with loop semantics
	virtual void visitLoop(qReal::Id const &id, QList<utils::DeepFirstSearcher::LinkInfo> const &links) = 0;

	/// This method is called when traverser gets into a block with switch semantics
	virtual void visitSwitch(qReal::Id const &id, QList<utils::DeepFirstSearcher::LinkInfo> const &links) = 0;

	/// This method is called when traverser gets into a block with fork semantics
	virtual void visitFork(qReal::Id const &id, QList<utils::DeepFirstSearcher::LinkInfo> &links) = 0;

	/// This method is called when traverser gets into a block with unknown semantics
	virtual void visitUnknown(qReal::Id const &id, QList<utils::DeepFirstSearcher::LinkInfo> const &links);

private:
	virtual void visit(qReal::Id const &nodeId, QList<utils::DeepFirstSearcher::LinkInfo> &links);

	qrRepo::RepoApi const &mRepo;
	GeneratorCustomizer &mCustomizer;
	utils::DeepFirstSearcher mDfser;
};

}

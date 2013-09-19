#pragma once

#include <qrrepo/repoApi.h>
#include <qrutils/graphUtils/deepFirstSearcher.h>

#include "generatorCustomizer.h"

namespace qReal {
namespace robots {
namespace generators {

/// A base for those entities who must travel through model and process every block
/// differentiating their semantics
class RobotsDiagramVisitor : public utils::DeepFirstSearcher::VisitorInterface
{
public:
	RobotsDiagramVisitor(qrRepo::RepoApi const &repo
			, GeneratorCustomizer &customizer);
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
	void startSearch(Id const &startingBlock);

	/// Terminates current graph traversal
	void terminateSearch();

	/// Parses guard property of given link in repo-representation and returns
	/// corresponding value of LinkGuard enumeration
	LinkGuard guardOf(Id const &linkId) const;

	/// This method is called when traverser gets into a block with regular semantics
	virtual void visitRegular(Id const &id, QList<utils::DeepFirstSearcher::LinkInfo> const &links) = 0;

	/// This method is called when traverser gets into a block with final-block semantics
	virtual void visitFinal(Id const &id, QList<utils::DeepFirstSearcher::LinkInfo> const &links) = 0;

	/// This method is called when traverser gets into a block with if semantics
	virtual void visitConditional(Id const &id, QList<utils::DeepFirstSearcher::LinkInfo> const &links) = 0;

	/// This method is called when traverser gets into a block with loop semantics
	virtual void visitLoop(Id const &id, QList<utils::DeepFirstSearcher::LinkInfo> const &links) = 0;

	/// This method is called when traverser gets into a block with switch semantics
	virtual void visitSwitch(Id const &id, QList<utils::DeepFirstSearcher::LinkInfo> const &links) = 0;

	/// This method is called when traverser gets into a block with fork semantics
	virtual void visitFork(Id const &id, QList<utils::DeepFirstSearcher::LinkInfo> const &links) = 0;

	/// This method is called when traverser gets into a block with unknown semantics
	virtual void visitUnknown(Id const &id, QList<utils::DeepFirstSearcher::LinkInfo> const &links);

private:
	virtual void visit(Id const &nodeId, QList<utils::DeepFirstSearcher::LinkInfo> const &links);

	qrRepo::RepoApi const &mRepo;
	GeneratorCustomizer &mCustomizer;
	utils::DeepFirstSearcher mDfser;
};

}
}
}

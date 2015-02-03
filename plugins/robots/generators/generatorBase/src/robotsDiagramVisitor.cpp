#include "generatorBase/robotsDiagramVisitor.h"

using namespace generatorBase;
using namespace qReal;

RobotsDiagramVisitor::RobotsDiagramVisitor(qrRepo::RepoApi const &repo
		, GeneratorCustomizer &customizer)
	: mRepo(repo)
	, mCustomizer(customizer)
	, mDfser(repo)
{
}

RobotsDiagramVisitor::~RobotsDiagramVisitor()
{
}

void RobotsDiagramVisitor::startSearch(qReal::Id const &startingBlock)
{
	mDfser.startSearch(startingBlock, this);
}

void RobotsDiagramVisitor::terminateSearch()
{
	mDfser.terminateSearch();
}

RobotsDiagramVisitor::LinkGuard RobotsDiagramVisitor::guardOf(qReal::Id const &linkId) const
{
	QString const guardProperty = mRepo.property(linkId, "Guard").toString().toLower();
	if (guardProperty == "true") {
		return trueGuard;
	} else if (guardProperty == "false") {
		return falseGuard;
	} else if (guardProperty == "iteration") {
		return iterationGuard;
	} else if (!guardProperty.isEmpty()) {
		return threadIdGuard;
	}

	return emptyGuard;
}

// It can be stayed without overloading now
void RobotsDiagramVisitor::visitUnknown(qReal::Id const &id, QList<utils::DeepFirstSearcher::LinkInfo> const &links)
{
	Q_UNUSED(id)
	Q_UNUSED(links)
}

void RobotsDiagramVisitor::visit(qReal::Id const &nodeId, QList<utils::DeepFirstSearcher::LinkInfo> &links)
{
	switch (mCustomizer.semanticsOf(nodeId)) {
	case enums::semantics::regularBlock:
		visitRegular(nodeId, links);
		break;
	case enums::semantics::finalBlock:
		visitFinal(nodeId, links);
		break;
	case enums::semantics::conditionalBlock:
		visitConditional(nodeId, links);
		break;
	case enums::semantics::loopBlock:
		visitLoop(nodeId, links);
		break;
	case enums::semantics::switchBlock:
		visitSwitch(nodeId, links);
		break;
	case enums::semantics::forkBlock:
		visitFork(nodeId, links);
		break;
	case enums::semantics::joinBlock:
		visitJoin(nodeId, links);
		break;
	default:
		visitUnknown(nodeId, links);
		break;
	}
}

#include "deepFirstSearcher.h"

using namespace qReal::robots::generators;

DeepFirstSearcher::DeepFirstSearcher(LogicalModelAssistInterface const &model)
	: mModel(model)
{
}

void DeepFirstSearcher::startSearch(Id const &firstId, VisitorInterface *visitor)
{
	dfs(firstId, QList<VisitorInterface *>() << visitor);
}

void DeepFirstSearcher::startSearch(Id const &firstId, QList<VisitorInterface *> const &visitors)
{
	dfs(firstId, visitors);
}

void DeepFirstSearcher::dfs(Id const &id, QList<VisitorInterface *> const &visitors)
{
	mVisitedNodes << id;

	IdList const outgoingLinks = mModel.logicalRepoApi().outgoingLinks(id);
	QList<LinkInfo> linkInfos;

	foreach (Id const &link, outgoingLinks) {
		Id const connectedNode = mModel.logicalRepoApi().otherEntityFromLink(link, id);
		LinkGuard const guard = guardOf(link);
		LinkInfo info;
		info.linkId = link;
		info.target = connectedNode;
		info.guard = guard;
		info.connected = !connectedNode.isNull() && connectedNode != Id::rootId();
		info.targetVisited = mVisitedNodes.contains(connectedNode);
		linkInfos << info;
	}

	foreach (VisitorInterface * const visitor, visitors) {
		visitor->visit(id, linkInfos);
	}

	foreach (LinkInfo const &link, linkInfos) {
		if (!link.targetVisited && link.connected) {
			dfs(link.target, visitors);
		}
	}
}

DeepFirstSearcher::LinkGuard DeepFirstSearcher::guardOf(qReal::Id const &linkId) const
{
	QVariant const guardProperty = mModel.propertyByRoleName(linkId, "Guard").toString().toLower();

	if (guardProperty == QString::fromUtf8("истина")) {
		return trueGuard;
	} else if (guardProperty == QString::fromUtf8("ложь")) {
		return falseGuard;
	} else if (guardProperty == QString::fromUtf8("итерация")) {
		return iterationGuard;
	}

	return emptyGuard;
}

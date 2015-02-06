#include "deepFirstSearcher.h"

using namespace qReal;
using namespace utils;

DeepFirstSearcher::DeepFirstSearcher(const qrRepo::LogicalRepoApi &repo)
	: mRepo(repo)
{
}

void DeepFirstSearcher::startSearch(const Id &firstId, VisitorInterface *visitor)
{
	startSearch(firstId, QList<VisitorInterface *>() << visitor);
}

void DeepFirstSearcher::startSearch(const Id &firstId, QList<VisitorInterface *> const &visitors)
{
	foreach (VisitorInterface * const visitor, visitors) {
		visitor->beforeSearch();
	}

	mSearchTerminated = false;
	mVisitedNodes.clear();
	dfs(firstId, visitors);

	foreach (VisitorInterface * const visitor, visitors) {
		visitor->afterSearch();
	}
}

void DeepFirstSearcher::terminateSearch()
{
	mSearchTerminated = true;
}

void DeepFirstSearcher::dfs(const Id &id, QList<VisitorInterface *> const &visitors)
{
	mVisitedNodes << id;

	const IdList outgoingLinks = mRepo.outgoingLinks(id);
	QList<LinkInfo> linkInfos;

	for (const Id &link : outgoingLinks) {
		const Id connectedNode = mRepo.otherEntityFromLink(link, id);
		LinkInfo info;
		info.linkId = link;
		info.target = connectedNode;
		info.connected = !connectedNode.isNull() && connectedNode != Id::rootId();
		info.targetVisited = mVisitedNodes.contains(connectedNode);
		linkInfos << info;
	}

	for (VisitorInterface * const visitor : visitors) {
		visitor->visit(id, linkInfos);
	}

	for (LinkInfo const &link : linkInfos) {
		if (!link.targetVisited && link.connected && !mSearchTerminated) {
			dfs(link.target, visitors);
		}
	}
}


void DeepFirstSearcher::VisitorInterface::beforeSearch()
{
}

void DeepFirstSearcher::VisitorInterface::afterSearch()
{
}

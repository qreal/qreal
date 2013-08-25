#include "deepFirstSearcher.h"

using namespace qReal;
using namespace utils;

DeepFirstSearcher::DeepFirstSearcher(LogicalModelAssistInterface const &model)
	: mModel(model)
{
}

void DeepFirstSearcher::startSearch(Id const &firstId, VisitorInterface *visitor)
{

	startSearch(firstId, QList<VisitorInterface *>() << visitor);
}

void DeepFirstSearcher::startSearch(Id const &firstId, QList<VisitorInterface *> const &visitors)
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

void DeepFirstSearcher::dfs(Id const &id, QList<VisitorInterface *> const &visitors)
{
	mVisitedNodes << id;

	IdList const outgoingLinks = mModel.logicalRepoApi().outgoingLinks(id);
	QList<LinkInfo> linkInfos;

	foreach (Id const &link, outgoingLinks) {
		Id const connectedNode = mModel.logicalRepoApi().otherEntityFromLink(link, id);
		LinkInfo info;
		info.linkId = link;
		info.target = connectedNode;
		info.connected = !connectedNode.isNull() && connectedNode != Id::rootId();
		info.targetVisited = mVisitedNodes.contains(connectedNode);
		linkInfos << info;
	}

	foreach (VisitorInterface * const visitor, visitors) {
		visitor->visit(id, linkInfos);
	}

	foreach (LinkInfo const &link, linkInfos) {
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

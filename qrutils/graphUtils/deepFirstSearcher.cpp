/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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

	for (const LinkInfo &link : linkInfos) {
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

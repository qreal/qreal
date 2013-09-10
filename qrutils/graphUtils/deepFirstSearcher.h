#pragma once

#include <QtCore/QSet>

#include "../utilsDeclSpec.h"
#include "../../qrrepo/logicalRepoApi.h"

namespace utils {

class QRUTILS_EXPORT DeepFirstSearcher
{
public:
	struct LinkInfo
	{
	public:
		qReal::Id linkId;
		qReal::Id target;
		bool connected;
		bool targetVisited;
	};

	class QRUTILS_EXPORT VisitorInterface
	{
	public:
		virtual ~VisitorInterface() {}
		virtual void visit(qReal::Id const &nodeId, QList<LinkInfo> const &links) = 0;
		virtual void beforeSearch();
		virtual void afterSearch();
	};

	explicit DeepFirstSearcher(qrRepo::LogicalRepoApi const &repo);

	void startSearch(qReal::Id const &firstId, VisitorInterface *visitor);
	void startSearch(qReal::Id const &firstId, QList<VisitorInterface *> const &visitors);

	void terminateSearch();

private:
	void dfs(qReal::Id const &id, QList<VisitorInterface *> const &visitors);

	qrRepo::LogicalRepoApi const &mRepo;
	QSet<qReal::Id> mVisitedNodes;
	bool mSearchTerminated;
};

}

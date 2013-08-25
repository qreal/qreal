#pragma once

#include <QtCore/QSet>

#include "../../qrgui/toolPluginInterface/usedInterfaces/logicalModelAssistInterface.h"

namespace utils {

class DeepFirstSearcher
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

	class VisitorInterface
	{
	public:
		virtual ~VisitorInterface() {}
		virtual void visit(qReal::Id const &nodeId, QList<LinkInfo> const &links) = 0;
		virtual void beforeSearch();
		virtual void afterSearch();
	};

	explicit DeepFirstSearcher(qReal::LogicalModelAssistInterface const &model);

	void startSearch(qReal::Id const &firstId, VisitorInterface *visitor);
	void startSearch(qReal::Id const &firstId, QList<VisitorInterface *> const &visitors);

	void terminateSearch();

private:
	void dfs(qReal::Id const &id, QList<VisitorInterface *> const &visitors);

	qReal::LogicalModelAssistInterface const &mModel;
	QSet<qReal::Id> mVisitedNodes;
	bool mSearchTerminated;
};

}

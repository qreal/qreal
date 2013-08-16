#pragma once

#include <QtCore/QSet>

#include <logicalModelAssistInterface.h>

namespace qReal {
namespace robots {
namespace generators {

class DeepFirstSearcher
{
public:
	// TODO: move this enum to global robots difinitions and use it
	enum LinkGuard
	{
		emptyGuard = 0
		, iterationGuard
		, trueGuard
		, falseGuard
	};

	struct LinkInfo
	{
	public:
		Id linkId;
		Id target;
		LinkGuard guard;
		bool connected;
		bool targetVisited;
	};

	class VisitorInterface
	{
	public:
		virtual ~VisitorInterface() {}
		virtual void visit(Id const &nodeId, QList<LinkInfo> const &links) = 0;
	};

	explicit DeepFirstSearcher(LogicalModelAssistInterface const &model);

	void startSearch(Id const &firstId, VisitorInterface *visitor);
	void startSearch(Id const &firstId, QList<VisitorInterface *> const &visitors);

private:
	LinkGuard guardOf(Id const &linkId) const;
	void dfs(Id const &id, QList<VisitorInterface *> const &visitors);

	LogicalModelAssistInterface const &mModel;
	QSet<Id> mVisitedNodes;
};

}
}
}

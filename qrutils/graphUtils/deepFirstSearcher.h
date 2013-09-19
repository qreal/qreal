#pragma once

#include <QtCore/QSet>

#include "../utilsDeclSpec.h"
#include "../../qrrepo/logicalRepoApi.h"

namespace utils {

/// Traverses the model in repository with deep-first search
class QRUTILS_EXPORT DeepFirstSearcher
{
public:
	/// A storage for more comfortable processing visited nodes
	struct  QRUTILS_EXPORT LinkInfo
	{
	public:
		qReal::Id linkId;
		qReal::Id target;
		bool connected;
		bool targetVisited;
	};

	/// An interface for processing visited nodes
	class QRUTILS_EXPORT VisitorInterface
	{
	public:
		virtual ~VisitorInterface() {}

		/// Called every time when traverser gets into new node
		virtual void visit(qReal::Id const &nodeId, QList<LinkInfo> const &links) = 0;

		/// Called when model traversal is going to be started
		virtual void beforeSearch();

		/// Called when model traversal was finished or terminated
		virtual void afterSearch();
	};

	explicit DeepFirstSearcher(qrRepo::LogicalRepoApi const &repo);

	/// Starts walking though the model with reporting to visitor about all visited nodes
	void startSearch(qReal::Id const &firstId, VisitorInterface *visitor);

	/// Starts walking though the model with reporting to each of visitors about all visited nodes
	void startSearch(qReal::Id const &firstId, QList<VisitorInterface *> const &visitors);

	/// Terminates walking though the model in repo. Termination doesn`t occure immediately,
	/// it will be performed only after current node will be processed by all visitors
	void terminateSearch();

private:
	void dfs(qReal::Id const &id, QList<VisitorInterface *> const &visitors);

	qrRepo::LogicalRepoApi const &mRepo;
	QSet<qReal::Id> mVisitedNodes;
	bool mSearchTerminated;
};

}

#pragma once

#include "../toolPluginInterface/toolPluginInterface.h"

namespace qReal
{

class DiffPluginInterface : public ToolPluginInterface
{
public:
	virtual ~DiffPluginInterface() {}

	/// Must be implemented to show diff between working copy and HEAD revision
	/// in remote repository.
	virtual void showDiff(QString const &targetProject = QString()) = 0;

	/// Must be implemented to show diff between working copy and specified revision
	/// in remote repository.
	virtual void showDiff(int repoRevision, QString const &targetProject = QString()) = 0;

	/// Must be implemented to show diff between two specified revisions
	/// in remote repository.
	virtual void showDiff(int oldRepoRevision , int newRepoRevision
			, QString const &targetProject = QString()) = 0;

};

}

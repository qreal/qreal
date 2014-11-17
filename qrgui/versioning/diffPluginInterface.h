#pragma once
#include <qrgui/plugins/toolPluginInterface/toolPluginInterface.h>

namespace qReal
{

class DiffPluginInterface : public ToolPluginInterface
{
public:
	virtual ~DiffPluginInterface() {}

	/// Must be implemented to show diff between working copy and HEAD revision
	/// in remote repository.
	virtual void showDiff(QString const &targetProject, QWidget *parentWidget
						  , bool const &compactMode = false) = 0;

	/// Must be implemented to show diff between working copy and specified revision
	/// in remote repository.
	virtual void showDiff(QString repoRevision, QString const &targetProject
						  , QWidget *parentWidget, bool const &compactMode = false) = 0;

	/// Must be implemented to show diff between two specified revisions
	/// in remote repository.
	virtual void showDiff(QString oldRepoRevision , QString newRepoRevision
			, QString const &targetProject, QWidget *parentWidget, bool const &compactMode = false) = 0;

};

}

#pragma once

#include <qrutils/versioningUtils/briefVersioningInterface.h>
#include <qrgui/plugins/toolPluginInterface/toolPluginInterface.h>
#include <qrgui/plugins/pluginManager/editorManagerInterface.h>


namespace qReal
{

class DiffPluginInterface : public ToolPluginInterface
{
public:
	virtual ~DiffPluginInterface() {}

	/// Must be implemented to show diff between working copy and HEAD revision
	/// in remote repository.
	virtual void showDiff(
		const QString &targetProject
		, QWidget *parentWidget
		, bool compactMode = false
	) = 0;

	/// Must be implemented to show diff between working copy and specified revision
	/// in remote repository.
	virtual void showDiff(
		const QString &repoRevision
		, const QString &targetProject
		, QWidget *parentWidget
		, bool compactMode = false
	) = 0;

	/// Must be implemented to show diff between two specified revisions
	/// in remote repository.
	virtual void showDiff(
		const QString &oldRepoRevision
		, const QString &newRepoRevision
		, const QString &targetProject
		, QWidget *parentWidget
		, bool compactMode = false
	) = 0;

	/// For plugin, because plugin's configurator cannot give necessary objects
	virtual void configure(
		ProjectManagementInterface *projectManager
		, ErrorReporterInterface *errorReporter
		, qrRepo::WorkingCopyManagementInterface *workingCopyManager
		, BriefVersioningInterface *vcs
		, QWidget *parent, EditorManagerInterface *manager
	) = 0;

};

}

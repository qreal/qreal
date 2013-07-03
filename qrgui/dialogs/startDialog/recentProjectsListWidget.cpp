#include "recentProjectsListWidget.h"
#include "../../../qrkernel/settingsManager.h"

using namespace qReal;

RecentProjectsListWidget::RecentProjectsListWidget(QDialog *parent)
		: ListWidget(parent)
{
	QString recentProjects = SettingsManager::value("recentProjects").toString();

	QString recentExistingProjects = "";

	// Check existance of a project before adding it to the list
	foreach (QString const &project, recentProjects.split(";", QString::SkipEmptyParts)) {
		if (QFile::exists(project))
		{
			addItem(project.split("/").last().split("\\").last(), project, project);
			recentExistingProjects += project + ";";
		}
	}

	SettingsManager::setValue("recentProjects", recentExistingProjects);
	highlightFirstItem();
}

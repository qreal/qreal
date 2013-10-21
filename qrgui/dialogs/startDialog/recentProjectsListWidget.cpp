#include "recentProjectsListWidget.h"

#include <qrkernel/settingsManager.h>

using namespace qReal;

RecentProjectsListWidget::RecentProjectsListWidget(QDialog *parent)
		: ListWidget(parent)
{
	QString recentProjects = SettingsManager::value("recentProjects").toString();

	QString recentExistingProjects = "";

	// Check existance of a project before adding it to the list
	foreach (QString const &pathToProject, recentProjects.split(";", QString::SkipEmptyParts)) {
		if (QFile::exists(pathToProject)) {
			addItem(QFileInfo(pathToProject).fileName(), pathToProject, pathToProject);
			recentExistingProjects += pathToProject + ";";
		}
	}

	SettingsManager::setValue("recentProjects", recentExistingProjects);
	highlightFirstItem();
}

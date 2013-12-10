#include "recentProjectsListWidget.h"
#include <qrkernel/settingsManager.h>

using namespace qReal;

RecentProjectsListWidget::RecentProjectsListWidget(QWidget *parent)
		: ListWidget(parent)
{
	QString const recentProjects = SettingsManager::value("recentProjects").toString();
	foreach (QString const &project, recentProjects.split(";", QString::SkipEmptyParts)) {
		addItem(project.split("/").last().split("\\").last(), project, project);
	}

	highlightFirstItem();
}

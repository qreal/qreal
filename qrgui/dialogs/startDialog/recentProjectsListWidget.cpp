#include "recentProjectsListWidget.h"
#include "../../qrkernel/settingsManager.h"

using namespace qReal;

RecentProjectsListWidget::RecentProjectsListWidget(QDialog *parent)
	: ListWidget(parent)
{
	QString recentProjects = SettingsManager::value("recentProjects").toString();
	foreach (QString const &project, recentProjects.split(";", QString::SkipEmptyParts)) {
		addItem(project.split("/").last().split("\\").last(), project, project);
	}
}

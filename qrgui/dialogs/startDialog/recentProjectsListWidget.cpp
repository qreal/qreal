#include "recentProjectsListWidget.h"
#include "../../qrkernel/settingsManager.h"

qReal::RecentProjectsListWidget::RecentProjectsListWidget(QDialog *parent)
	: QListWidget(parent)
{
	QStringList recentProjects = SettingsManager::value("recentProjects").toString().split(";");
	addItems(recentProjects);
}

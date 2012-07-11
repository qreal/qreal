#include "recentProjectsListWidget.h"
#include "../../qrkernel/settingsManager.h"

using namespace qReal;

RecentProjectsListWidget::RecentProjectsListWidget(QDialog *parent)
	: ListWidget(parent)
{
	QStringList recentProjects = SettingsManager::value("recentProjects").toString().split(";");
	foreach (QString const &project, recentProjects) {
		addItem(project.split("/").last().split("\\").last(),	project);
	}

	connect(this, SIGNAL(userDataSelected(QString)), parent, SLOT(close()));
}

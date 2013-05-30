#pragma once

#include <QtWidgets/QDialog>

#include "../../mainwindow/projectManager/projectManager.h"

namespace qReal {

class MainWindow;

/**
 * @brief Start page dialog
 *
 * Start page dialog that allows the user to either open an existing project (from list of recent
 * projects or not), or create a new one and did not appear in the main window of application,
 * where there is no single project.
 */
class StartDialog : public QDialog
{
	Q_OBJECT

public:
	StartDialog(MainWindow *mainWindow, ProjectManager *projectManager);

private slots:
	void openRecentProject(QString const &fileName);
	void openExistingProject();
	void exitApp();
	void createProjectWithDiagram(QString const &idString);

private:
	static QSize const mMinimumSize;

	MainWindow *mMainWindow;
	ProjectManager *mProjectManager;
};

}

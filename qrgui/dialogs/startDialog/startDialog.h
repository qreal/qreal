#pragma once
#include <QtGui/QDialog>

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
	explicit StartDialog(MainWindow *mainWindow);
	void keyPressEvent(QKeyEvent *event);

	static const QSize minimumSize;

private slots:
	void openRecentProject(QString const &fileName);
	void openExistingProject();
	void createProjectWithDiagram(QString const &idString);

private:
	MainWindow *mMainWindow;
	ProjectManager *mProjectManager;
};

}


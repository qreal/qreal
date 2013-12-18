#pragma once

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>

#include "mainwindow/projectManager/projectManager.h"

namespace qReal {

class MainWindow;

/// @brief Start page dialog
/// Start page dialog that allows the user to either open an existing project (from list of recent
/// projects or not), or create a new one and did not appear in the main window of application,
/// where there is no single project.
class StartWidget : public QWidget
{
	Q_OBJECT

public:
	StartWidget(MainWindow *mainWindow, ProjectManager *projectManager);

	/// Shows or hides interpreter buttons
	void setVisibleForInterpreterButton(bool const visible);

signals:
	void closeStartTab(int index);

private slots:
	void openRecentProject(QString const &fileName);
	void openExistingProject();
	void createProjectWithDiagram(QString const &idString);
	void openInterpretedDiagram();
	void createInterpretedDiagram();

private:
	QPushButton *createCommandButton(QString const &text);
	QLayout *initRecentProjects(QString const &recentProjects);

	MainWindow *mMainWindow;
	ProjectManager *mProjectManager;
	int mProjectListSize;

	QPushButton *mOpenInterpreterButton;  // Has ownership.
	QPushButton *mCreateInterpreterButton;  // Has ownership.
};

}

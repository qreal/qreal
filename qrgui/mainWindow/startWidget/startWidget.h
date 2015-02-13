#pragma once

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>

#include "mainWindow/projectManager/projectManagerWrapper.h"

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

	/// Shows or hides interpreter buttons. Also this method can change layout of the
	/// buttons on start tab.
	void setVisibleForInterpreterButton(const bool visible);

private slots:
	void openRecentProject(const QString &fileName);
	void openExistingProject();
	void createProjectWithDiagram(const QString &idString);
	void openInterpretedDiagram();
	void createInterpretedDiagram();

private:
	QWidget *createMainWidget();
	QWidget *createHeader();
	QWidget *createRecentProjectsWidget();
	QWidget *createProjectsManagementWidget();

	QLayout *createRecentProjectsList(const QString &recentProjects);
	QWidget *createPluginsList();
	QWidget *createPluginButton(const Id &editor, const Id &diagram, QWidget * const bindedImage);

	void centralizeButton(QPushButton * const styledButton);

	virtual void paintEvent(QPaintEvent *event) override;

	MainWindow *mMainWindow;  // Doesn't have ownership.
	ProjectManager *mProjectManager;  // Doesn't have ownership.
	int mProjectListSize;

	QBoxLayout *mProjectsManagementLayout;  // Has ownership.
	QWidget *mRecentProjectsWidget;  // Has ownership.
	QPushButton *mNewProjectButton;  // Has ownership.
	QPushButton *mOpenProjectButton;  // Has ownership.
	QPushButton *mOpenInterpreterButton;  // Has ownership.
	QPushButton *mCreateInterpreterButton;  // Has ownership.
};

}

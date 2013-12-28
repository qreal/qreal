#pragma once

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>

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

private slots:
	void openRecentProject(QString const &fileName);
	void openExistingProject();
	void createProjectWithDiagram(QString const &idString);
	void openInterpretedDiagram();
	void createInterpretedDiagram();

private:
	QWidget *createMainWidget();
	QWidget *createHeader();
	QWidget *createRecentProjectsWidget();
	QWidget *createProjectsManagementWidget();

	QPushButton *createCommandButton(QString const &text, QString const &icon = QString()
			, QBoxLayout::Direction direction = QBoxLayout::LeftToRight);

	QLayout *createRecentProjectsList(QString const &recentProjects);
	QWidget *createPluginsList();
	QWidget *createPluginButton(Id const &editor, Id const &diagram);
	QWidget *createSpacer();

	virtual void paintEvent(QPaintEvent *event) override;

	MainWindow *mMainWindow;  // Doesn't have ownership.
	ProjectManager *mProjectManager;  // Doesn't have ownership.
	int mProjectListSize;

	QPushButton *mOpenInterpreterButton;  // Has ownership.
	QPushButton *mCreateInterpreterButton;  // Has ownership.
};

}

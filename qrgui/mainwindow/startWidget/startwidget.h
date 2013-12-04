#pragma once

#include <QWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QCommandLinkButton>

#include "../projectManager/projectManager.h"

namespace qReal {

class MainWindow;

/**
 * @brief Start page dialog
 *
 * Start page dialog that allows the user to either open an existing project (from list of recent
 * projects or not), or create a new one and did not appear in the main window of application,
 * where there is no single project.
 */
class StartWidget : public QWidget
{
	Q_OBJECT

public:
	explicit StartWidget(MainWindow *mainWindow, ProjectManager *projectManager);
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
	QCommandLinkButton *createCommandButton(QString const &text
							, QObject const *reciever, char const *slot, QKeySequence::StandardKey standartHotkey);
	void initRecentProjects();

    MainWindow *mMainWindow;
    ProjectManager *mProjectManager;
	int mProjectListSize;
	QVBoxLayout *mProjectsLayout;
	QHBoxLayout *mSessionsLayout;

	QCommandLinkButton *mInterpreterButton;  // Has ownership.
	QCommandLinkButton *mCreateInterpreterButton;  // Has ownership.
};

}

#pragma once

#include <QtWidgets/QCommandLinkButton>

#include "../managedClosableDialog.h"

#include "../../mainwindow/projectManager/projectManager.h"
#include "../../pluginManager/editorManagerInterface.h"

namespace qReal {

class MainWindow;

/**
 * @brief Start page dialog
 *
 * Start page dialog that allows the user to either open an existing project (from list of recent
 * projects or not), or create a new one and did not appear in the main window of application,
 * where there is no single project.
 */
class StartDialog : public ManagedClosableDialog
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param mainWindow QReal main window.
	/// @param projectManager ProjectManager instance that shall handle all project operations.
	explicit StartDialog(MainWindow &mainWindow, ProjectManager &projectManager);

	/// Destructor.
	~StartDialog();

	/// Makes visible or invisible buttons related to metamodel interpreter.
	void setVisibleForInterpreterButton(bool const visible);

private slots:
	void openRecentProject(QString const &fileName);
	void openExistingProject();
	void createProjectWithDiagram(QString const &idString);
	void exitApp();
	void openInterpretedDiagram();
	void createInterpretedDiagram();

private:
	static const QSize mMinimumSize;

	MainWindow &mMainWindow;
	ProjectManager &mProjectManager;
	QCommandLinkButton *mInterpreterButton;  // Has ownership.
	QCommandLinkButton *mCreateInterpreterButton;  // Has ownership.
};

}

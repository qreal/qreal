#pragma once

#include <QtWidgets/QDialog>
#include <QtWidgets/QCommandLinkButton>

#include "../../mainwindow/projectManager/projectManager.h"
#include "../../pluginManager/editorManagerInterface.h"

namespace qReal {

class MainWindow;

/**
* @brief Start page dialog
*
* Start page dialog that allows the user to either open an existing project (from list of recent
* projects or not), or create a new one and did not appear in the main window of
* application, where there is no single project.
*/

class StartDialog : public QDialog
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
	void exitApp();
	void openInterpretedDiagram();
	void createInterpretedDiagram();
	void createProjectWithDiagram(QString const &idString);

private:
	QCommandLinkButton *createCommandButton(QString const &text
			, QObject const *reciever, char const *slot, QKeySequence::StandardKey standartHotkey);

	static QSize const mMinimumSize;

	MainWindow &mMainWindow;
	ProjectManager &mProjectManager;
	QCommandLinkButton *mInterpreterButton;  // Has ownership.
	QCommandLinkButton *mCreateInterpreterButton;  // Has ownership.
};

}

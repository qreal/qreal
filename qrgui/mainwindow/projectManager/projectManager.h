#pragma once
#include <QtCore/QObject>
#include <QtCore/QString>

#include "autosaver.h"

namespace qReal {

class MainWindow;

/**
 * @brief Class that provides all of the work load project from a file, stored in the project file, and so on
 */
class ProjectManager : public QObject
{
	Q_OBJECT

public:
	explicit ProjectManager(MainWindow *mainWindow);

public slots:
	/// Tries to open an existing file, prompts you to save changes, if any. Checks for the presence of a file and the
	/// sufficiency of the plugins. In the event of any failure, leaving the project as is and returns false, o/w true.
	bool openExisting(const QString &fileName);
	/// Similarly openExisting(...), but offers the user select a file (by a dialog)
	bool suggestToOpenExisting();
	/// Creating a new project
	bool openNewWithDiagram();
	/// Creating a new diagramm
	void suggestToCreateDiagram(bool isClosable = true);
	/// Offers the user select a file (by a dialog) and import all diagrams from it into the current project.
	bool suggestToimport();

	void close();

	/// Save the project in the user file, if it specified. Otherwise save to autosave file
	void save();
	/// Save the project in the file with the name fileName, if it not empty (fileName). Otherwise return false only.
	bool saveAs(const QString &fileName);
	/// Similarly saveAs(...), but offers the user specified file location (by a dialog)
	bool suggestToSaveAs();
	/// Similarly save(), if specified project-file, similarly suggestToSaveAs(...) o/w
	bool saveOrSuggestToSaveAs();

public:
	/// Create an empty project
	bool openEmptyWithSuggestToSaveChanges();
	/// Try to open save file with name fileName, show messages is file non exist or plugins are missing and
	/// return false, or return true otherwise. fileName == "" (default value) will be create an empty project.
	bool open(QString const &fileName = "");
	bool suggestToSaveChangesOrCancel();
	void setUnsavedIndicator(bool isUnsaved);
	void reinitAutosaver();
	QString saveFilePath();
	void setSaveFilePath(QString const &filePath = "");

private:
	bool import(const QString &fileName);
	bool saveFileExists(QString const &fileName);
	bool pluginsEnough();
	QString missingPluginNames();

	void refreshWindowTitleAccordingToSaveFile();
	void refreshTitleModifiedSuffix();
	void refreshApplicationStateAfterSave();
	void refreshApplicationStateAfterOpen();
	int suggestToSaveOrCancelMessage();
	QString getOpenFileName(const QString &dialogWindowTitle);
	QString getSaveFileName(const QString &dialogWindowTitle);

	MainWindow *mMainWindow;
	Autosaver *mAutosaver;
	bool mUnsavedIndicator;
	QString mSaveFilePath;
};

}

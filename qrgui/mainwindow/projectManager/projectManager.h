#pragma once

#include <QtCore/QFileInfo>

#include "mainwindow/projectManager/projectManagementInterface.h"
#include "textEditor/textManagerInterface.h"
#include "versionsConverterManager.h"

namespace qReal {

class MainWindow;

/// ProjectManagementInterface implementation
class ProjectManager : public ProjectManagementInterface
{
	Q_OBJECT

public:
	explicit ProjectManager(MainWindow *mainWindow, TextManagerInterface *textManager);

public slots:
	bool openExisting(QString const &fileName);
	bool suggestToOpenExisting();
	bool openNewWithDiagram();
	void suggestToCreateDiagram(bool isClosable = true);
	bool suggestToImport();

	void close();

	void save();
	bool saveAs(QString const &fileName);
	bool suggestToSaveAs();
	bool saveOrSuggestToSaveAs();

	void setUnsavedIndicator(bool isUnsaved);

	/// Saves current project into given file without refreshing application state after it
	void saveTo(QString const &fileName);

public:
	bool openEmptyWithSuggestToSaveChanges();
	bool open(QString const &fileName = "");
	bool suggestToSaveChangesOrCancel();
	void reinitAutosaver();
	QString openFileName(QString const &dialogWindowTitle) const;
	QString saveFilePath() const;
	void setSaveFilePath(QString const &filePath = "");

	/// Prompts user to restore last session if it was incorrectly terminated
	/// and returns yes if he agrees. Otherwise returns false
	bool restoreIncorrectlyTerminated();

private:
	bool import(QString const &fileName);
	bool saveFileExists(QString const &fileName);
	bool pluginsEnough() const;
	QString missingPluginNames() const;

	bool checkVersions();
	bool checkForUnknownElements();

	void refreshWindowTitleAccordingToSaveFile();
	void refreshTitleModifiedSuffix();
	void refreshApplicationStateAfterSave();
	void refreshApplicationStateAfterOpen();

	int suggestToSaveOrCancelMessage();

	QString getOpenFileName(QString const &dialogWindowTitle);
	QString getSaveFileName(QString const &dialogWindowTitle);

	void fileNotFoundMessage(QString const &fileName) const;

	MainWindow *mMainWindow;
	TextManagerInterface *mTextManager;
	Autosaver *mAutosaver;
	bool mUnsavedIndicator;
	QString mSaveFilePath;
	bool mSomeProjectOpened;
	VersionsConverterManager mVersionsConverter;
};

}

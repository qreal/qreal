#pragma once

#include "projectManagementInterface.h"

namespace qReal {

class MainWindow;

/// ProjectManagementInterface implementation
class ProjectManager : public ProjectManagementInterface
{
	Q_OBJECT

public:
	explicit ProjectManager(MainWindow *mainWindow);

public slots:
	bool openExisting(const QString &fileName);
	bool suggestToOpenExisting();
	bool openNewWithDiagram();
	void suggestToCreateDiagram(bool isClosable = true);
	bool suggestToImport();

	void close();

	void save();
	bool saveAs(const QString &fileName);
	bool suggestToSaveAs();
	bool saveOrSuggestToSaveAs();

public:
	bool openEmptyWithSuggestToSaveChanges();
	bool open(QString const &fileName = "autosave.qrs");
	bool suggestToSaveChangesOrCancel();
	void setUnsavedIndicator(bool isUnsaved);
	void reinitAutosaver();
	QString saveFilePath() const;
	void setSaveFilePath(QString const &filePath = "");
	void saveGenCode(QString const &text);

private:
	bool import(const QString &fileName);
	bool saveFileExists(QString const &fileName);
	bool pluginsEnough();
	QString missingPluginNames() const;

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

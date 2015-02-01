#pragma once

#include <QtCore/QFileInfo>

#include <qrgui/systemFacade/components/projectManager.h>
#include "textEditor/textManagerInterface.h"
#include "versionsConverterManager.h"

namespace qReal {

class MainWindow;

/// Project manager that knows about GUI entities and manages their state in save files context.
class ProjectManagerWrapper : public ProjectManager
{
	Q_OBJECT

public:
	ProjectManagerWrapper(MainWindow *mainWindow, TextManagerInterface *textManager);

public slots:
	bool openNewWithDiagram() override;
	void suggestToCreateDiagram(bool isClosable = true) override;

	void close() override;

	void save() override;
	bool suggestToSaveAs() override;
	bool saveOrSuggestToSaveAs() override;

	void setUnsavedIndicator(bool isUnsaved) override;

public:
	bool open(const QString &fileName = QString()) override;
	bool suggestToSaveChangesOrCancel() override;

	void refreshWindowTitleAccordingToSaveFile();

	bool askQuestion(const QString &title, const QString &question) const override;

private:
	QString textFileFilters() const;

	bool checkVersions() override;

	void refreshTitleModifiedSuffix();
	void refreshApplicationStateAfterSave() override;
	void refreshApplicationStateAfterOpen() override;
	void refreshApplicationStateAfterSaveOrOpen();

	QString openFileName(const QString &dialogWindowTitle) const override;
	QString saveFileName(const QString &dialogWindowTitle) const override;

	int suggestToSaveOrCancelMessage();

	void showMessage(const QString &title, const QString &message) const override;

	MainWindow *mMainWindow;
	TextManagerInterface *mTextManager;
	VersionsConverterManager mVersionsConverter;
};

}

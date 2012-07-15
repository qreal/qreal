#pragma once
#include <QtCore/QObject>
#include <QtCore/QString>

namespace qReal {

class MainWindow;

}

namespace qReal {

class ProjectManager : public QObject
{
	Q_OBJECT

public:
	explicit ProjectManager(MainWindow *mainWindow);

public slots:
	bool openExisting(const QString &fileName);
	bool suggestToOpenExisting();
	bool openNewWithDiagram();
	void suggestToCreateDiagram(bool isNonClosable = false);
	bool suggestToimport();

	void close();

	bool save();
	bool saveAs(const QString &fileName);
	bool suggestToSaveAs();
	bool saveOrSuggestToSaveAs();

public:
	bool openEmptyWithSuggestToSaveChanges();
	bool open(QString const &fileName = "");
	bool suggestToSaveChangesOrCancel();

private:
	bool import(const QString &fileName);
	bool saveFileExists(QString const &fileName);
	bool pluginsEnough();
	QString missingPluginNames();
	void refreshWindowTitleAccordingToSaveFile();
	void refreshApplicationStateAfterSave();
	void refreshApplicationStateAfterOpen();
	QString getOpenFileName(const QString &dialogWindowTitle);
	QString getSaveFileName(const QString &dialogWindowTitle);

	MainWindow *mMainWindow;
};

}

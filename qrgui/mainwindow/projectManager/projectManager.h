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
	bool openNewWithDiagram();
	bool openExisting(const QString &fileName);
	bool suggestToOpenExisting();

	void close();

	bool saveAll();
	bool saveAs(const QString &fileName);
	bool suggestToSaveAs();

public:
	bool openEmptyWithSuggestToSaveChanges();
	bool open(QString const &fileName = "");
	bool suggestToSaveChangesOrCancel();

private:
	bool saveFileExists(QString const &fileName);
	bool pluginsEnough();
	QString missingPluginNames();
	void refreshWindowTitleAccordingToSaveFile();
	void refreshApplicationStateAfterSave();

	MainWindow *mMainWindow;
};

}

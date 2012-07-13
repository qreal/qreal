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

	bool openEmptyProject();
	bool open(QString const &fileName = "");
	void close();

	void saveAs(const QString &fileName);

	// openEmptyProject()
	bool suggestToSaveChangesOrCancel();

public slots:
	void saveAll();
	void suggestToSaveAs();

private:

	// open(...)
	bool saveFileExists(QString const &fileName);
	bool pluginsEnough();
	QString missingPluginNames();
	void refreshWindowTitleAccordingToSaveFile();

	// saveAll(), saveAs(...)
	void refreshApplicationStateAfterSave();

	MainWindow *mMainWindow;
};

}

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
	bool open(QString const &fileName);

private:
	bool saveFileExists(QString const &fileName);
	bool pluginsEnough();
	QString missingPluginNames();
	void refreshWindowTitleAccordingToSaveFile();

	MainWindow *mMainWindow;
};

}

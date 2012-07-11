#pragma once
#include <QtGui/QDialog>
#include <QtGui/QTabWidget>

namespace qReal {

class MainWindow;

}

namespace qReal {

class StartDialog : public QDialog
{
	Q_OBJECT
	
public:
	explicit StartDialog(MainWindow *mainWindow, QWidget *parent = 0);

public slots:
	void openRecentProject(QString const &fileName);
	void openExistingProject();
	void createProjectWithDiagram(QString const &idString);

private:
	MainWindow *mMainWindow;
	QTabWidget *mTabWidget;
};

}

#pragma once
#include <QtGui/QWidget>
#include <QtGui/QListWidget>

namespace qReal {

class MainWindow;

}

namespace qReal {

class SuggestToCreateDiagramWidget : public QWidget
{
	Q_OBJECT

public:
	explicit SuggestToCreateDiagramWidget(MainWindow *mainWindow, QDialog *parent = 0);

private:
	MainWindow *mMainWindow;
	QListWidget *mDiagramsListWidget;
};

}

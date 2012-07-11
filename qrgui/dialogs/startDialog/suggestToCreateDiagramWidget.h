#pragma once
#include <QtGui/QWidget>
#include <QtGui/QPushButton>
#include <QtGui/QListWidget>
#include "listWidget.h"

namespace qReal {

class MainWindow;

}

namespace qReal {

class SuggestToCreateDiagramWidget : public ListWidget
{
	Q_OBJECT

public:
	explicit SuggestToCreateDiagramWidget(MainWindow *mainWindow, QDialog *parent = 0);

private:
	MainWindow *mMainWindow;
};

}

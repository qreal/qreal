#pragma once
#include <QtGui/QWidget>
#include <QtGui/QPushButton>
#include <QtGui/QListWidget>
#include "../qrkernel/ids.h"
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
	void addItem(Id const &editor, Id const &diagram);
	MainWindow *mMainWindow;
};

}

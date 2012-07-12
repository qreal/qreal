#pragma once
#include <QDialog>
#include <QKeyEvent>
#include "startDialog/suggestToCreateDiagramWidget.h"

namespace qReal {

class MainWindow;

}

namespace qReal {

class SuggestToCreateDiagramDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SuggestToCreateDiagramDialog(MainWindow *mainWindow, bool isNonClosable = false);
	void keyPressEvent(QKeyEvent *event);

signals:
	void userDataSelected(QString const &userData);

private:
	SuggestToCreateDiagramWidget *mSuggestWidget;
	bool mIsNonClosable;
};

}

#include "suggestToCreateDiagramDialog.h"
#include "mainwindow/mainWindow.h"

using namespace qReal;

SuggestToCreateDiagramDialog::SuggestToCreateDiagramDialog(MainWindow *mainWindow, bool isNonClosable)
	: QDialog(mainWindow, isNonClosable ? Qt::WindowMinimizeButtonHint : Qt::Dialog)
	, mSuggestWidget(new SuggestToCreateDiagramWidget(mainWindow, this))
	, mIsNonClosable(isNonClosable)
{
	setFixedSize(275, 240);
	connect(mSuggestWidget, SIGNAL(userDataSelected(QString)), mainWindow, SLOT(createDiagram(QString)));
}

void SuggestToCreateDiagramDialog::keyPressEvent(QKeyEvent *event)
{
	if (mIsNonClosable && event->key() == Qt::Key_Escape) {
		return;
	}
	QDialog::keyPressEvent(event);
}

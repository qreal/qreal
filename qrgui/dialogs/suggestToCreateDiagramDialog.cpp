#include "suggestToCreateDiagramDialog.h"

using namespace qReal;

SuggestToCreateDiagramDialog::SuggestToCreateDiagramDialog(MainWindow *mainWindow, bool isNonClosable)
	: QDialog(NULL, isNonClosable ? Qt::WindowMinimizeButtonHint : Qt::Dialog)
	, mSuggestWidget(new SuggestToCreateDiagramWidget(mainWindow, this))
	, mIsNonClosable(isNonClosable)
{
	setFixedSize(275, 240);
	connect(mSuggestWidget, SIGNAL(userDataSelected(QString)), this, SIGNAL(userDataSelected(QString)));
}

void SuggestToCreateDiagramDialog::keyPressEvent(QKeyEvent *event)
{
	if (mIsNonClosable && event->key() == Qt::Key_Escape) {
		return;
	}
	QDialog::keyPressEvent(event);
}

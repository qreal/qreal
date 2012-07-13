#include <QtGui/QVBoxLayout>

#include "suggestToCreateDiagramDialog.h"
#include "mainwindow/mainWindow.h"

using namespace qReal;

SuggestToCreateDiagramDialog::SuggestToCreateDiagramDialog(MainWindow *mainWindow, bool isNonClosable)
	: QDialog(mainWindow, isNonClosable ? Qt::WindowMinimizeButtonHint : Qt::Dialog)
	, mIsNonClosable(isNonClosable)
{
	SuggestToCreateDiagramWidget *suggestWidget = new SuggestToCreateDiagramWidget(mainWindow, this);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(suggestWidget);
	setLayout(mainLayout);
	setWindowTitle(tr("Create diagram"));

	connect(suggestWidget, SIGNAL(userDataSelected(QString)), mainWindow, SLOT(createDiagram(QString)));
}

void SuggestToCreateDiagramDialog::keyPressEvent(QKeyEvent *event)
{
	if (mIsNonClosable && event->key() == Qt::Key_Escape) {
		return;
	}
	QDialog::keyPressEvent(event);
}

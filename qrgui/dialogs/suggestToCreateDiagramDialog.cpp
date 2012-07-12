#include <QtGui/QVBoxLayout>

#include "suggestToCreateDiagramDialog.h"
#include "mainwindow/mainWindow.h"

using namespace qReal;

SuggestToCreateDiagramDialog::SuggestToCreateDiagramDialog(MainWindow *mainWindow, bool isNonClosable)
	: QDialog(mainWindow, isNonClosable ? Qt::WindowMinimizeButtonHint : Qt::Dialog)
	, mIsNonClosable(isNonClosable)
{
	setWindowTitle(tr("Create diagram"));
	SuggestToCreateDiagramWidget *suggestWidget = new SuggestToCreateDiagramWidget(mainWindow, this);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(suggestWidget);
	setLayout(mainLayout);

	connect(suggestWidget, SIGNAL(userDataSelected(QString)), mainWindow, SLOT(createDiagram(QString)));
}

void SuggestToCreateDiagramDialog::keyPressEvent(QKeyEvent *event)
{
	if (mIsNonClosable && event->key() == Qt::Key_Escape) {
		return;
	}
	QDialog::keyPressEvent(event);
}

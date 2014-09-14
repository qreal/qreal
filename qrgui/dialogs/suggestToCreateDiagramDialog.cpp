#include "suggestToCreateDiagramDialog.h"

#include <QtWidgets/QVBoxLayout>

#include "mainWindow/mainWindow.h"

using namespace qReal;

SuggestToCreateDiagramDialog::SuggestToCreateDiagramDialog(MainWindow *mainWindow, bool isClosable)
		: ManagedClosableDialog(mainWindow, isClosable)
{
	SuggestToCreateDiagramWidget *suggestWidget = new SuggestToCreateDiagramWidget(mainWindow, this);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(suggestWidget);
	setLayout(mainLayout);
	setWindowTitle(tr("Create diagram"));

	connect(suggestWidget, SIGNAL(userDataSelected(QString)), mainWindow, SLOT(createDiagram(QString)));
	connect(suggestWidget, SIGNAL(userDataSelected(QString)), this, SLOT(forceClose()));
}

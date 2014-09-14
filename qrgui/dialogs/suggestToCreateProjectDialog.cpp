#include "suggestToCreateProjectDialog.h"

#include <QtWidgets/QVBoxLayout>

#include "mainWindow/mainWindow.h"

using namespace qReal;

SuggestToCreateProjectDialog::SuggestToCreateProjectDialog(MainWindow *mainWindow)
	: QDialog(mainWindow)
{
	SuggestToCreateDiagramWidget *suggestWidget = new SuggestToCreateDiagramWidget(mainWindow, this);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(suggestWidget);
	setLayout(mainLayout);
	setWindowTitle(tr("Create project"));

	connect(suggestWidget, SIGNAL(userDataSelected(QString)), mainWindow, SLOT(createProject(QString)));
	connect(suggestWidget, SIGNAL(userDataSelected(QString)), this, SLOT(accept()));
}

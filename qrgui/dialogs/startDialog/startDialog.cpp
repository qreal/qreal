#include <QtGui/QTabWidget>
#include <QtGui/QCommandLinkButton>
#include "startDialog.h"
#include "suggestToCreateDiagramWidget.h"
#include "recentProjectsListWidget.h"
#include "../mainwindow/mainWindow.h"

using namespace qReal;

StartDialog::StartDialog(MainWindow *mainWindow, QWidget *parent)
	: QDialog(parent)
	, mMainWindow(mainWindow)
	, mTabWidget(new QTabWidget)
{
	mTabWidget->setFixedSize(380, 300);

	RecentProjectsListWidget *recentProjects = new RecentProjectsListWidget(this);
	mTabWidget->addTab(recentProjects, tr("Recent projects"));
	SuggestToCreateDiagramWidget *diagrams = new SuggestToCreateDiagramWidget(mMainWindow, this);
	mTabWidget->addTab(diagrams, tr("New project with diagram"));

	QCommandLinkButton *quitLink = new QCommandLinkButton(tr("&Quit QReal"));
	QCommandLinkButton *helpLink = new QCommandLinkButton(tr("&Help - not implemented yet"));
	helpLink->setDisabled(true);

	QHBoxLayout *commandLinksLayout = new QHBoxLayout;
	commandLinksLayout->addWidget(helpLink);
	commandLinksLayout->addWidget(quitLink);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(mTabWidget);
	mainLayout->addLayout(commandLinksLayout);

	setLayout(mainLayout);

	connect(quitLink, SIGNAL(clicked()), qApp, SLOT(quit()));
	connect(recentProjects, SIGNAL(userDataSelected(QString)), this, SLOT(openRecentProject(QString)));
	connect(diagrams, SIGNAL(userDataSelected(QString)), this, SLOT(createProjectWithDiagram(QString)));
}

void StartDialog::openRecentProject(QString const &fileName)
{
	mMainWindow->open(fileName);
}

void StartDialog::createProjectWithDiagram(const QString &idString)
{
	mMainWindow->openEmptyProject();
	mMainWindow->createDiagram(idString);
}

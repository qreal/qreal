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
	setFixedSize(700, 384);
	mTabWidget->setFixedSize(480, 300);

	RecentProjectsListWidget *recentProjects = new RecentProjectsListWidget(this);
	mTabWidget->addTab(recentProjects, tr("Recent projects"));
	SuggestToCreateDiagramWidget *diagrams = new SuggestToCreateDiagramWidget(mMainWindow, this);
	mTabWidget->addTab(diagrams, tr("New project with diagram"));

	QCommandLinkButton *createNewProjectLink = new QCommandLinkButton(tr("&Create new project"));
	QCommandLinkButton *helpLink = new QCommandLinkButton(tr("&Help"));
	QCommandLinkButton *quitLink = new QCommandLinkButton(tr("&Quit QReal"));

	QVBoxLayout *commandLinksLayout = new QVBoxLayout;
	commandLinksLayout->addWidget(createNewProjectLink);
	commandLinksLayout->addWidget(helpLink);
	commandLinksLayout->addWidget(quitLink);
	commandLinksLayout->insertStretch(0);
	commandLinksLayout->insertStretch(-1);

	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addWidget(mTabWidget);
	mainLayout->addLayout(commandLinksLayout);

	setLayout(mainLayout);

	connect(createNewProjectLink, SIGNAL(clicked()), this, SLOT(createNewProject()));
	connect(helpLink, SIGNAL(clicked()), mMainWindow, SLOT(showHelp()));
	connect(quitLink, SIGNAL(clicked()), qApp, SLOT(quit()));
	connect(recentProjects, SIGNAL(userDataSelected(QString)), this, SLOT(openRecentProject(QString)));
	connect(diagrams, SIGNAL(userDataSelected(QString)), this, SLOT(createProjectWithDiagram(QString)));
}

void StartDialog::createNewProject()
{
	mMainWindow->openEmptyProject();
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

#include <QtGui/QTabWidget>
#include <QtGui/QCommandLinkButton>

#include "startDialog.h"
#include "suggestToCreateDiagramWidget.h"
#include "recentProjectsListWidget.h"
#include "../mainwindow/mainWindow.h"

using namespace qReal;

const QSize StartDialog::mMinimumSize = QSize(350, 200);

StartDialog::StartDialog(MainWindow *mainWindow, ProjectManager *projectManager)
		: ManagedClosableDialog(mainWindow, false)
		, mMainWindow(mainWindow)
		, mProjectManager(projectManager)
{
	setMinimumSize(mMinimumSize);
	QTabWidget *tabWidget = new QTabWidget;

	RecentProjectsListWidget *recentProjects = new RecentProjectsListWidget(this);
	tabWidget->addTab(recentProjects, tr("&Recent projects"));
	SuggestToCreateDiagramWidget *diagrams = new SuggestToCreateDiagramWidget(mMainWindow, this);
	tabWidget->addTab(diagrams, tr("&New project with diagram"));

	if (recentProjects->count() == 0) {
		tabWidget->setCurrentWidget(diagrams);
	}

	QCommandLinkButton *quitLink = new QCommandLinkButton(tr("&Quit QReal"));
	QCommandLinkButton *openLink = new QCommandLinkButton(tr("&Open existing project"));

	QHBoxLayout *commandLinksLayout = new QHBoxLayout;
	commandLinksLayout->addWidget(openLink);
	commandLinksLayout->addWidget(quitLink);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(tabWidget);
	mainLayout->addLayout(commandLinksLayout);

	setLayout(mainLayout);
	setWindowTitle(tr("Start page"));

	connect(openLink, SIGNAL(clicked()), this, SLOT(openExistingProject()));
	connect(quitLink, SIGNAL(clicked()), this, SLOT(exitApp()));
	connect(recentProjects, SIGNAL(userDataSelected(QString)), this, SLOT(openRecentProject(QString)));
	connect(diagrams, SIGNAL(userDataSelected(QString)), this, SLOT(createProjectWithDiagram(QString)));
}

void StartDialog::openRecentProject(QString const &fileName)
{
	if (mProjectManager->open(fileName)) {
		forceClose();
	}
}

void StartDialog::openExistingProject()
{
	if (mProjectManager->suggestToOpenExisting()) {
		forceClose();
	}
}

void StartDialog::createProjectWithDiagram(const QString &idString)
{
	mProjectManager->openEmptyWithSuggestToSaveChanges();
	mMainWindow->createDiagram(idString);
	forceClose();
}

void StartDialog::exitApp()
{
	forceClose();
	qApp->closeAllWindows();
}

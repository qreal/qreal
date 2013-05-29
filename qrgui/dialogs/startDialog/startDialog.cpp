#include <QtWidgets/QTabWidget>
#include <QtWidgets/QCommandLinkButton>

#include "startDialog.h"
#include "suggestToCreateDiagramWidget.h"
#include "recentProjectsListWidget.h"
#include "../mainwindow/mainWindow.h"

using namespace qReal;

const QSize StartDialog::mMinimumSize = QSize(350, 200);

StartDialog::StartDialog(MainWindow *mainWindow, ProjectManager *projectManager)
		: QDialog(mainWindow)
		, mMainWindow(mainWindow)
		, mProjectManager(projectManager)
{
	setMinimumSize(mMinimumSize);

	QTabWidget *tabWidget = new QTabWidget;

	RecentProjectsListWidget *recentProjects = new RecentProjectsListWidget(this);
	tabWidget->addTab(recentProjects, tr("&Recent projects"));

	Id const theOnlyDiagram = mMainWindow->manager()->theOnlyDiagram();
	if (theOnlyDiagram == Id()) {
		SuggestToCreateDiagramWidget *diagrams = new SuggestToCreateDiagramWidget(mMainWindow, this);
		tabWidget->addTab(diagrams, tr("&New project with diagram"));
		connect(diagrams, SIGNAL(userDataSelected(QString)), this, SLOT(createProjectWithDiagram(QString)));
		if (recentProjects->count() == 0) {
			tabWidget->setCurrentWidget(diagrams);
		}
	}

	QHBoxLayout *commandLinksLayout = new QHBoxLayout;

	if (theOnlyDiagram != Id()) {
		Id const editor = mMainWindow->manager()->editors()[0];
		QString const diagramIdString = mMainWindow->manager()->diagramNodeNameString(editor, theOnlyDiagram);

		QCommandLinkButton *newLink = new QCommandLinkButton(tr("&New project"));
		QSignalMapper *newProjectMapper = new QSignalMapper(this);
		newProjectMapper->setMapping(newLink, diagramIdString);
		connect(newProjectMapper, SIGNAL(mapped(QString)), this, SLOT(createProjectWithDiagram(QString)));
		connect(newLink, SIGNAL(clicked()), newProjectMapper, SLOT(map()));
		commandLinksLayout->addWidget(newLink);
	}

	QCommandLinkButton *openLink = new QCommandLinkButton(tr("&Open existing project"));
	connect(openLink, SIGNAL(clicked()), this, SLOT(openExistingProject()));
	commandLinksLayout->addWidget(openLink);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(tabWidget);
	mainLayout->addLayout(commandLinksLayout);

	setLayout(mainLayout);
	setWindowTitle(tr("Start page"));

	connect(recentProjects, SIGNAL(userDataSelected(QString)), this, SLOT(openRecentProject(QString)));
	connect(this, SIGNAL(rejected()), this, SLOT(exitApp()));
}

void StartDialog::openRecentProject(QString const &fileName)
{
	if (mProjectManager->open(fileName)) {
		accept();
	}
}

void StartDialog::openExistingProject()
{
	if (mProjectManager->suggestToOpenExisting()) {
		accept();
	}
}

void StartDialog::createProjectWithDiagram(QString const &idString)
{
	if (mMainWindow->createProject(idString)) {
		accept();
	}
}

void StartDialog::exitApp()
{
	qApp->closeAllWindows();
}

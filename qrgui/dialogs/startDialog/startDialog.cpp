#include <QtWidgets/QTabWidget>

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
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

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

		QSignalMapper *newProjectMapper = new QSignalMapper(this);
		QCommandLinkButton *newLink = createCommandButton(tr("New project")
				, newProjectMapper, SLOT(map()), QKeySequence::New);
		newProjectMapper->setMapping(newLink, diagramIdString);
		connect(newProjectMapper, SIGNAL(mapped(QString)), this, SLOT(createProjectWithDiagram(QString)));
		commandLinksLayout->addWidget(newLink);
	}

	commandLinksLayout->addWidget(createCommandButton(tr("Open existing project")
			, this, SLOT(openExistingProject()), QKeySequence::Open));

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

QCommandLinkButton *StartDialog::createCommandButton(QString const &text
		, QObject const *reciever, char const *slot, QKeySequence::StandardKey standartHotkey)
{
	QCommandLinkButton *result = new QCommandLinkButton(text);
	connect(result, SIGNAL(clicked()), reciever, slot);
	QAction *buttonAction = new QAction(this);
	buttonAction->setShortcuts(standartHotkey);
	connect(buttonAction, SIGNAL(triggered()), result, SLOT(animateClick()));
	addAction(buttonAction);
	result->setToolTip(QKeySequence(standartHotkey).toString());
	return result;
}

void StartDialog::exitApp()
{
	qApp->closeAllWindows();
}

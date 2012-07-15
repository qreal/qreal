#include <QtGui/QTabWidget>
#include <QtGui/QCommandLinkButton>

#include "startDialog.h"
#include "suggestToCreateDiagramWidget.h"
#include "recentProjectsListWidget.h"
#include "../mainwindow/mainWindow.h"

using namespace qReal;

const QSize StartDialog::minimumSize = QSize(350, 200);

StartDialog::StartDialog(MainWindow *mainWindow)
	: QDialog(mainWindow, Qt::WindowMaximizeButtonHint)
	, mMainWindow(mainWindow)
	, mProjectManager(mainWindow->projectManager())
{
	setMinimumSize(minimumSize);
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
	connect(quitLink, SIGNAL(clicked()), qApp, SLOT(closeAllWindows()));
	connect(recentProjects, SIGNAL(userDataSelected(QString)), this, SLOT(openRecentProject(QString)));
	connect(diagrams, SIGNAL(userDataSelected(QString)), this, SLOT(createProjectWithDiagram(QString)));
}

void StartDialog::openRecentProject(QString const &fileName)
{
	if (mProjectManager->open(fileName)) {
		close();
	}
}

void StartDialog::openExistingProject()
{
	if (mProjectManager->suggestToOpenExisting()) {
		close();
	}
}

void StartDialog::createProjectWithDiagram(const QString &idString)
{
	mProjectManager->openEmptyWithSuggestToSaveChanges();
	mMainWindow->createDiagram(idString);
	// This dialog will be closed by the SuggestToCreateDiagramWidget
}

void StartDialog::keyPressEvent(QKeyEvent *event)
{
	if (event->key() != Qt::Key_Escape) {
		QDialog::keyPressEvent(event);
	}
}

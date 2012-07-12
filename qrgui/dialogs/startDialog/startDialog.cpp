#include <QtGui/QTabWidget>
#include <QtGui/QCommandLinkButton>

#include "startDialog.h"
#include "suggestToCreateDiagramWidget.h"
#include "recentProjectsListWidget.h"
#include "../mainwindow/mainWindow.h"

using namespace qReal;

StartDialog::StartDialog(MainWindow *mainWindow)
	: QDialog(mainWindow, Qt::WindowMaximizeButtonHint)
	, mMainWindow(mainWindow)
	, mTabWidget(new QTabWidget)
{
	RecentProjectsListWidget *recentProjects = new RecentProjectsListWidget(this);
	mTabWidget->addTab(recentProjects, tr("Recent projects"));
	SuggestToCreateDiagramWidget *diagrams = new SuggestToCreateDiagramWidget(mMainWindow, this);
	mTabWidget->addTab(diagrams, tr("New project with diagram"));

	QCommandLinkButton *quitLink = new QCommandLinkButton(tr("&Quit QReal"));
	QCommandLinkButton *openLink = new QCommandLinkButton(tr("&Open existing project"));

	QHBoxLayout *commandLinksLayout = new QHBoxLayout;
	commandLinksLayout->addWidget(openLink);
	commandLinksLayout->addWidget(quitLink);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(mTabWidget);
	mainLayout->addLayout(commandLinksLayout);

	setLayout(mainLayout);

	connect(openLink, SIGNAL(clicked()), this, SLOT(openExistingProject()));
	connect(quitLink, SIGNAL(clicked()), qApp, SLOT(closeAllWindows()));
	connect(recentProjects, SIGNAL(userDataSelected(QString)), this, SLOT(openRecentProject(QString)));
	connect(diagrams, SIGNAL(userDataSelected(QString)), this, SLOT(createProjectWithDiagram(QString)));
}

void StartDialog::openRecentProject(QString const &fileName)
{
	if (!QFile::exists(fileName)) {
		QMessageBox fileNotFoundMessage(QMessageBox::Information, tr("File not found")
				, tr("File ") + fileName + tr(" not found. Try again")
				, QMessageBox::Ok, this);
		fileNotFoundMessage.exec();
		return;
	}
	if (mMainWindow->open(fileName)) {
		close();
	}
}

void StartDialog::openExistingProject()
{
	if (mMainWindow->openExistingProject()) {
		close();
	}
}

void StartDialog::createProjectWithDiagram(const QString &idString)
{
	mMainWindow->openEmptyProject();
	mMainWindow->createDiagram(idString);
}

void StartDialog::keyPressEvent(QKeyEvent *event)
{
	if (event->key() != Qt::Key_Escape) {
		QDialog::keyPressEvent(event);
	}
}

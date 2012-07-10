#include <QtGui/QTabWidget>
#include <QtGui/QCommandLinkButton>
#include "startDialog.h"
#include "suggestToCreateDiagramWidget.h"
#include "recentProjectsListWidget.h"
#include "../mainwindow/mainWindow.h"

qReal::StartDialog::StartDialog(MainWindow *mainWindow, QWidget *parent)
	: QDialog(parent)
	, mMainWindow(mainWindow)
	, mTabWidget(new QTabWidget)
{
	setFixedSize(700, 384);
	mTabWidget->addTab(new RecentProjectsListWidget(this), tr("Recent projects"));
	mTabWidget->addTab(new SuggestToCreateDiagramWidget(mMainWindow, this), tr("New project with diagram"));
	mTabWidget->setFixedSize(480, 300);

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
}

void qReal::StartDialog::createNewProject()
{
	mMainWindow->createProject();
	if (mMainWindow->mDiagramCreateFlag) {
		close();
	}
}

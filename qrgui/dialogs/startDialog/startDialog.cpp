#include <QtGui/QTabWidget>
#include <QtGui/QCommandLinkButton>

#include "startDialog.h"
#include "suggestToCreateDiagramWidget.h"
#include "recentProjectsListWidget.h"
#include "../mainwindow/mainWindow.h"
#include "../models/models.h"

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
	QCommandLinkButton *openIDLink = new QCommandLinkButton(tr("&Open interpreted diagram"));
	QCommandLinkButton *createIDLink = new QCommandLinkButton(tr("&Create interpreted diagram"));

	QHBoxLayout *openIDLinkLayout = new QHBoxLayout;
	openIDLinkLayout->addWidget(openIDLink);
	mInterpreterButton = openIDLink;

	QHBoxLayout *createIDLinkLayout = new QHBoxLayout;
	createIDLinkLayout->addWidget(createIDLink);
	mCreateInterpreterButton = createIDLink;

	QHBoxLayout *commandLinksLayout = new QHBoxLayout;
	commandLinksLayout->addWidget(openLink);
	commandLinksLayout->addWidget(quitLink);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(tabWidget);
	mainLayout->addLayout(openIDLinkLayout);
	mainLayout->addLayout(createIDLinkLayout);
	mainLayout->addLayout(commandLinksLayout);

	setLayout(mainLayout);
	setWindowTitle(tr("Start page"));

	connect(openLink, SIGNAL(clicked()), this, SLOT(openExistingProject()));
	connect(quitLink, SIGNAL(clicked()), this, SLOT(exitApp()));
	connect(openIDLink, SIGNAL(clicked()), this, SLOT(openInterpretedDiagram()));
	connect(createIDLink, SIGNAL(clicked()), this, SLOT(createInterpretedDiagram()));
	connect(recentProjects, SIGNAL(userDataSelected(QString)), this, SLOT(openRecentProject(QString)));
	connect(diagrams, SIGNAL(userDataSelected(QString)), this, SLOT(createProjectWithDiagram(QString)));
}

void StartDialog::setVisibleForInterpreterButton(bool const value)
{
	mInterpreterButton->setVisible(value);
	mCreateInterpreterButton->setVisible(value);
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
	mProjectManager->clearAutosaveFile();
	mProjectManager->openEmptyWithSuggestToSaveChanges();
	mMainWindow->createDiagram(idString);
	forceClose();
}

void StartDialog::exitApp()
{
	forceClose();
	qApp->closeAllWindows();
}

void StartDialog::openInterpretedDiagram()
{
	hide();
	QString fileName = mProjectManager->getOpenFileName(tr("Select file with metamodel to open"));
	ProxyEditorManager *editorManagerProxy = mMainWindow->proxyManager();
	if (!fileName.isEmpty() && mProjectManager->open(fileName)) {
		editorManagerProxy->setProxyManager(new InterpreterEditorManager(fileName));
		QStringList interpreterDiagramsList;
		foreach (Id editor, editorManagerProxy->editors()) {
			foreach (Id diagram, editorManagerProxy->diagrams(editor)) {
				QString const diagramNodeName = editorManagerProxy->diagramNodeName(editor.editor(), diagram.diagram());
				if (diagramNodeName.isEmpty()) {
					continue;
				}
				interpreterDiagramsList.append("qrm:/" + editor.editor() + "/" + diagram.diagram() + "/" + diagramNodeName);
			}
		}
		foreach (QString interpreterIdString, interpreterDiagramsList) {
			mMainWindow->models()->repoControlApi().exterminate();
			mMainWindow->models()->reinit();
			mMainWindow->loadMetamodel();
			mMainWindow->createDiagram(interpreterIdString);
		}
		forceClose();
	} else {
		show();
		editorManagerProxy->setProxyManager(new EditorManager());
	}
}

void StartDialog::createInterpretedDiagram()
{
	hide();
	ProxyEditorManager *editorManagerProxy = mMainWindow->proxyManager();
	editorManagerProxy->setProxyManager(new InterpreterEditorManager(""));
	bool ok;
	QString name = QInputDialog::getText(this, tr("Enter the diagram name:"), tr("diagram name:"), QLineEdit::Normal, "", &ok);
	while (ok && name.isEmpty()) {
		name = QInputDialog::getText(this, tr("Enter the diagram name:"), tr("diagram name:"), QLineEdit::Normal, "", &ok);
	}
	if (ok) {
		QPair<Id, Id> editorAndDiagram = editorManagerProxy->createEditorAndDiagram(name);
		mMainWindow->addEditorElementsToPalette(editorAndDiagram.first, editorAndDiagram.second);
		mMainWindow->models()->repoControlApi().exterminate();
		mMainWindow->models()->reinit();
		mMainWindow->loadMetamodel();
		forceClose();
	} else {
		show();
		editorManagerProxy->setProxyManager(new EditorManager());
	}
}

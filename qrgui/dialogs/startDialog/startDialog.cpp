#include <QtWidgets/QTabWidget>

#include "startDialog.h"
#include "suggestToCreateDiagramWidget.h"
#include "recentProjectsListWidget.h"
#include "../../mainwindow/mainWindow.h"

using namespace qReal;

const QSize StartDialog::mMinimumSize = QSize(400, 200);

StartDialog::StartDialog(MainWindow &mainWindow, ProjectManager &projectManager)
		: QDialog(&mainWindow, false)
		, mMainWindow(mainWindow)
		, mProjectManager(projectManager)
{
	setMinimumSize(mMinimumSize);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

	QTabWidget *tabWidget = new QTabWidget;

	RecentProjectsListWidget *recentProjects = new RecentProjectsListWidget(this);
	tabWidget->addTab(recentProjects, tr("&Recent projects"));

	Id const theOnlyDiagram = mMainWindow.editorManager().theOnlyDiagram();
	if (theOnlyDiagram.isNull()) {
		SuggestToCreateDiagramWidget *diagrams = new SuggestToCreateDiagramWidget(&mMainWindow, this);
		tabWidget->addTab(diagrams, tr("&New project with diagram"));
		connect(diagrams, SIGNAL(userDataSelected(QString)), this, SLOT(createProjectWithDiagram(QString)));
		if (recentProjects->count() == 0) {
			tabWidget->setCurrentWidget(diagrams);
		}
	}

	QCommandLinkButton *openIDLink = new QCommandLinkButton(tr("&Open interpreted diagram"));
	QCommandLinkButton *createIDLink = new QCommandLinkButton(tr("&Create interpreted diagram"));

	QHBoxLayout *openIDLinkLayout = new QHBoxLayout;
	openIDLinkLayout->addWidget(openIDLink);
	mInterpreterButton = openIDLink;

	QHBoxLayout *createIDLinkLayout = new QHBoxLayout;
	createIDLinkLayout->addWidget(createIDLink);
	mCreateInterpreterButton = createIDLink;

	QHBoxLayout *commandLinksLayout = new QHBoxLayout;

	if (theOnlyDiagram != Id()) {
		Id const editor = mMainWindow.editorManager().editors()[0];
		QString const diagramIdString = mMainWindow.editorManager().diagramNodeNameString(editor, theOnlyDiagram);

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
	mainLayout->addLayout(openIDLinkLayout);
	mainLayout->addLayout(createIDLinkLayout);
	mainLayout->addLayout(commandLinksLayout);

	setLayout(mainLayout);
	setWindowTitle(tr("Start page"));

	connect(openIDLink, SIGNAL(clicked()), this, SLOT(openInterpretedDiagram()));
	connect(createIDLink, SIGNAL(clicked()), this, SLOT(createInterpretedDiagram()));

	connect(recentProjects, SIGNAL(userDataSelected(QString)), this, SLOT(openRecentProject(QString)));
	connect(this, SIGNAL(rejected()), this, SLOT(exitApp()));
}

StartDialog::~StartDialog()
{
	delete mInterpreterButton;
	delete mCreateInterpreterButton;
}

void StartDialog::setVisibleForInterpreterButton(bool const visible)
{
	mInterpreterButton->setVisible(visible);
	mCreateInterpreterButton->setVisible(visible);
}

void StartDialog::openRecentProject(QString const &fileName)
{
	if (mProjectManager.open(fileName)) {
		accept();
	}
}

void StartDialog::openExistingProject()
{
	if (mProjectManager.suggestToOpenExisting()) {
		accept();
	}
}

void StartDialog::createProjectWithDiagram(QString const &idString)
{
	if (mMainWindow.createProject(idString)) {
		accept();
	}
}

QCommandLinkButton *StartDialog::createCommandButton(QString const &text
		, QObject const *reciever, char const *slot, QKeySequence::StandardKey standartHotkey)
{
	QCommandLinkButton * const result = new QCommandLinkButton(text);
	connect(result, SIGNAL(clicked()), reciever, slot);
	QAction * const buttonAction = new QAction(this);
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

void StartDialog::openInterpretedDiagram()
{
	hide();
	QString const fileName = mProjectManager.openFileName(tr("Select file with metamodel to open"));
	ProxyEditorManager &editorManagerProxy = mMainWindow.editorManagerProxy();

	if (!fileName.isEmpty() && mProjectManager.open(fileName)) {
		editorManagerProxy.setProxyManager(new InterpreterEditorManager(fileName));
		QStringList interpreterDiagramsList;
		foreach (Id const &editor, editorManagerProxy.editors()) {
			foreach (Id const &diagram, editorManagerProxy.diagrams(editor)) {
				QString const diagramNodeName = editorManagerProxy.diagramNodeName(editor.editor(), diagram.diagram());
				if (diagramNodeName.isEmpty()) {
					continue;
				}

				interpreterDiagramsList.append("qrm:/" + editor.editor() + "/"
						+ diagram.diagram() + "/" + diagramNodeName);
			}
		}

		foreach (QString const &interpreterIdString, interpreterDiagramsList) {
			// TODO: ???
			mMainWindow.models()->repoControlApi().exterminate();
			mMainWindow.models()->reinit();
			mMainWindow.loadPlugins();
			mMainWindow.createDiagram(interpreterIdString);
		}

		accept();
	} else {
		show();
		editorManagerProxy.setProxyManager(new EditorManager());
	}
}

void StartDialog::createInterpretedDiagram()
{
	hide();
	ProxyEditorManager &editorManagerProxy = mMainWindow.editorManagerProxy();
	editorManagerProxy.setProxyManager(new InterpreterEditorManager(""));
	bool ok = false;
	QString name = QInputDialog::getText(this, tr("Enter the diagram name:"), tr("diagram name:"), QLineEdit::Normal, "", &ok);
	while (ok && name.isEmpty()) {
		name = QInputDialog::getText(this, tr("Enter the diagram name:"), tr("diagram name:"), QLineEdit::Normal, "", &ok);
	}

	if (ok) {
		QPair<Id, Id> editorAndDiagram = editorManagerProxy.createEditorAndDiagram(name);
		mMainWindow.addEditorElementsToPalette(editorAndDiagram.first, editorAndDiagram.second);
		mMainWindow.models()->repoControlApi().exterminate();
		mMainWindow.models()->reinit();
		mMainWindow.loadPlugins();
		accept();
	} else {
		show();
		editorManagerProxy.setProxyManager(new EditorManager());
	}
}

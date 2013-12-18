#include "startWidget.h"

#include <qrkernel/settingsManager.h>

#include "dialogs/suggestToCreateDiagramDialog.h"
#include "mainwindow/mainWindow.h"
#include "suggestToCreateDiagramWidget.h"
#include "recentProjectsListWidget.h"
#include "recentProjectItem.h"
#include "brandManager/brandManager.h"

using namespace qReal;

StartWidget::StartWidget(MainWindow *mainWindow, ProjectManager *projectManager)
	: QWidget()
	, mMainWindow(mainWindow)
	, mProjectManager(projectManager)
	, mProjectListSize(5)
{
	QPalette pal(palette());
	QColor const color = QColor::fromHsl(180, 50, 240, 255);
	pal.setColor(QPalette::Background, color);
	setAutoFillBackground(true);
	setPalette(pal);

	QHBoxLayout *mainLayout = new QHBoxLayout;
	QVBoxLayout *sessionsLayout = new QVBoxLayout;

	mainLayout->addLayout(sessionsLayout);
	QWidget *verticalLineWidget = new QWidget;
	verticalLineWidget->setFixedWidth(1);
	verticalLineWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	verticalLineWidget->setStyleSheet(QString("background-color: #c0c0c0;"));
	mainLayout->addWidget(verticalLineWidget);

	QString const recentProjects = SettingsManager::value("recentProjects").toString();
	if (!recentProjects.isEmpty()) {
		mainLayout->addLayout(initRecentProjects(recentProjects));
	}

	setLayout(mainLayout);

	QPushButton * const openProjectButton = createCommandButton(tr("&Open existing project"));
	connect(openProjectButton, &QPushButton::clicked, this, &StartWidget::openExistingProject);
	sessionsLayout->addWidget(openProjectButton);

	QSignalMapper *closeTabMapper = new QSignalMapper(this);
	connect(closeTabMapper, SIGNAL(mapped(int)), this, SIGNAL(closeStartTab(int)), Qt::QueuedConnection);

	mOpenInterpreterButton = createCommandButton(tr("Open &interpreted diagram"));
	mCreateInterpreterButton = createCommandButton(tr("&Create interpreted diagram"));
	connect(mOpenInterpreterButton , SIGNAL(clicked()), closeTabMapper, SLOT(map()));
	connect(mCreateInterpreterButton, SIGNAL(clicked()), closeTabMapper, SLOT(map()));
	connect(mOpenInterpreterButton , SIGNAL(clicked()), this, SLOT(openInterpretedDiagram()));
	connect(mCreateInterpreterButton, SIGNAL(clicked()), this, SLOT(createInterpretedDiagram()));

	mOpenInterpreterButton = mOpenInterpreterButton ;
	mCreateInterpreterButton = mCreateInterpreterButton;

	sessionsLayout->addWidget(mCreateInterpreterButton);
	sessionsLayout->addWidget(mOpenInterpreterButton);

	Id const theOnlyDiagram = mMainWindow->editorManager().theOnlyDiagram();
	if (!theOnlyDiagram.isNull()) {
		Id const editor = mMainWindow->editorManager().editors()[0];
		QString const diagramIdString = mMainWindow->editorManager().diagramNodeNameString(editor, theOnlyDiagram);

		QPushButton *newProjectButton = createCommandButton(tr("&New project"));
		QSignalMapper *newProjectMapper = new QSignalMapper(this);
		newProjectMapper->setMapping(newProjectButton, diagramIdString);
		connect(newProjectButton, SIGNAL(clicked()), newProjectMapper, SLOT(map()));
		connect(newProjectMapper, SIGNAL(mapped(QString)), this, SLOT(createProjectWithDiagram(QString)));

		sessionsLayout->addWidget(newProjectButton);
	} else {
		if (!mMainWindow->editorManager().editors().isEmpty()) {
			SuggestToCreateDiagramWidget *suggestWidget = new SuggestToCreateDiagramWidget(mainWindow, this);
			closeTabMapper->setMapping(suggestWidget, 0);
			connect(suggestWidget, SIGNAL(userDataSelected(QString)), mainWindow, SLOT(createDiagram(QString)));
			connect(suggestWidget, SIGNAL(userDataSelected(QString)), closeTabMapper, SLOT(map()));
			sessionsLayout->insertWidget(0, suggestWidget);
		} else {
			openProjectButton->hide();
		}
	}

	sessionsLayout->addStretch(0);
}

void StartWidget::openRecentProject(QString const &fileName)
{
	if (mProjectManager->open(fileName)) {
		emit closeStartTab(0);
	}
}

void StartWidget::openExistingProject()
{
	if (mProjectManager->suggestToOpenExisting()) {
		emit closeStartTab(0);
	}
}

void StartWidget::createProjectWithDiagram(QString const &idString)
{
	if (mMainWindow->createProject(idString)){
		emit closeStartTab(0);
	}
}

QLayout *StartWidget::initRecentProjects(QString const &recentProjects)
{
	QVBoxLayout * const mainLayout = new QVBoxLayout;
	QVBoxLayout * const recentProjectsLayout = new QVBoxLayout;

	QLabel * const recentProjectsLabel = new QLabel(tr("Recent projects"));
	recentProjectsLabel->setWordWrap(true);
	recentProjectsLabel->setStyleSheet(BrandManager::styles()->headerLevel1Style());

	mainLayout->addWidget(recentProjectsLabel);
	mainLayout->addLayout(recentProjectsLayout);

	mainLayout->addStretch(0);

	int i = 0;
	for (QString const &project : recentProjects.split(";", QString::SkipEmptyParts)) {
		QString const name = project.split("/").last().split("\\").last();
		RecentProjectItem *projectWidget = new RecentProjectItem(this, name, project);
		recentProjectsLayout->addWidget(projectWidget);

		++i;
		if (i >= mProjectListSize) {
			break;
		}
	}

	return mainLayout;
}

void StartWidget::openInterpretedDiagram()
{
	hide();
	QString const fileName = mProjectManager->openFileName(tr("Select file with metamodel to open"));
	ProxyEditorManager &editorManagerProxy = mMainWindow->editorManagerProxy();

	if (!fileName.isEmpty() && mProjectManager->open(fileName)) {
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
					mMainWindow->models()->repoControlApi().exterminate();
					mMainWindow->models()->reinit();
					mMainWindow->loadPlugins();
					mMainWindow->createDiagram(interpreterIdString);
			}
	} else {
			show();
			editorManagerProxy.setProxyManager(new EditorManager());
	}
}

void StartWidget::createInterpretedDiagram()
{
	hide();
	ProxyEditorManager &editorManagerProxy = mMainWindow->editorManagerProxy();
	editorManagerProxy.setProxyManager(new InterpreterEditorManager(""));
	bool ok = false;
	QString name = QInputDialog::getText(this, tr("Enter the diagram name:"), tr("diagram name:")
					, QLineEdit::Normal, "", &ok);
	while (ok && name.isEmpty()) {
		name = QInputDialog::getText(this, tr("Enter the diagram name:"), tr("diagram name:")
						, QLineEdit::Normal, "", &ok);
	}

	if (ok) {
		QPair<Id, Id> editorAndDiagram = editorManagerProxy.createEditorAndDiagram(name);
		mMainWindow->addEditorElementsToPalette(editorAndDiagram.first, editorAndDiagram.second);
		mMainWindow->models()->repoControlApi().exterminate();
		mMainWindow->models()->reinit();
		mMainWindow->loadPlugins();
	} else {
		show();
		editorManagerProxy.setProxyManager(new EditorManager());
	}
}

QPushButton *StartWidget::createCommandButton(QString const &text)
{
	QPushButton * const result = new QPushButton(text);
	result->setStyleSheet(BrandManager::styles()->commandButtonStyle());
	return result;
}

void StartWidget::setVisibleForInterpreterButton(bool const visible)
{
	mOpenInterpreterButton->setVisible(visible);
	mCreateInterpreterButton->setVisible(visible);
}

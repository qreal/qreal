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
	: mMainWindow(mainWindow)
	, mProjectManager(projectManager)
	, mProjectListSize(5)  // TODO: Why 5?
{
	setStyleSheet(BrandManager::styles()->startTabBackroundStyle());


	QHBoxLayout *mainLayout = new QHBoxLayout;
	QVBoxLayout *projectsManagementLayout = new QVBoxLayout;

	mainLayout->addLayout(projectsManagementLayout);

	QString const recentProjects = SettingsManager::value("recentProjects").toString();
	if (!recentProjects.isEmpty() && !mMainWindow->editorManager().editors().isEmpty()) {
		QFrame *verticalLine = new QFrame;
		verticalLine->setFrameShape(QFrame::VLine);
		verticalLine->setFrameShadow(QFrame::Sunken);
		verticalLine->setLineWidth(5);
		mainLayout->addWidget(verticalLine);
		mainLayout->addLayout(createRecentProjectsList(recentProjects));
	}

	mainLayout->setStretch(0, 20);
	mainLayout->setStretch(1, 0);
	mainLayout->setStretch(2, 10);
	setLayout(mainLayout);

	QPushButton * const openProjectButton = createCommandButton(tr("&Open existing project"));
	connect(openProjectButton, &QPushButton::clicked, this, &StartWidget::openExistingProject);
	projectsManagementLayout->addWidget(openProjectButton);

	QSignalMapper *closeTabMapper = new QSignalMapper(this);
	connect(closeTabMapper, SIGNAL(mapped(int)), this, SIGNAL(closeStartTab(int)), Qt::QueuedConnection);

	mInterpreterButtonsSeparator = new QFrame;
	mInterpreterButtonsSeparator->setFrameShape(QFrame::HLine);
	mInterpreterButtonsSeparator->setFrameShadow(QFrame::Sunken);
	mInterpreterButtonsSeparator->setLineWidth(3);
	mOpenInterpreterButton = createCommandButton(tr("Open &interpreted\n diagram"));
	mCreateInterpreterButton = createCommandButton(tr("&Create interpreted\n diagram"));
	connect(mOpenInterpreterButton , SIGNAL(clicked()), closeTabMapper, SLOT(map()));
	connect(mCreateInterpreterButton, SIGNAL(clicked()), closeTabMapper, SLOT(map()));
	connect(mOpenInterpreterButton , SIGNAL(clicked()), this, SLOT(openInterpretedDiagram()));
	connect(mCreateInterpreterButton, SIGNAL(clicked()), this, SLOT(createInterpretedDiagram()));

	projectsManagementLayout->addWidget(mInterpreterButtonsSeparator);
	projectsManagementLayout->addWidget(mCreateInterpreterButton);
	projectsManagementLayout->addWidget(mOpenInterpreterButton);

	Id const theOnlyDiagram = mMainWindow->editorManager().theOnlyDiagram();
	if (!theOnlyDiagram.isNull()) {
		Id const editor = mMainWindow->editorManager().editors()[0];
		QString const diagramIdString = mMainWindow->editorManager().diagramNodeNameString(editor, theOnlyDiagram);

		QPushButton *newProjectButton = createCommandButton(tr("&New project"));
		projectsManagementLayout->addWidget(newProjectButton);

		newProjectButton->setDefault(true);

		QSignalMapper *newProjectMapper = new QSignalMapper(this);
		newProjectMapper->setMapping(newProjectButton, diagramIdString);
		connect(newProjectButton, SIGNAL(clicked()), newProjectMapper, SLOT(map()));
		connect(newProjectMapper, SIGNAL(mapped(QString)), this, SLOT(createProjectWithDiagram(QString)));

	} else {
		if (!mMainWindow->editorManager().editors().isEmpty()) {
			QLabel *createProjectLabel = new QLabel(tr("New project:"));
			createProjectLabel->setStyleSheet(BrandManager::styles()->headerLevel1Style());

			QWidget * const pluginsWidget = createPluginsList();

			projectsManagementLayout->insertWidget(0, createProjectLabel);
			projectsManagementLayout->insertWidget(1, pluginsWidget);
		} else {
			openProjectButton->hide();
		}
	}

	projectsManagementLayout->addStretch(0);
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
	if (mMainWindow->createProject(idString)) {
		emit closeStartTab(0);
	}
}

QLayout *StartWidget::createRecentProjectsList(QString const &recentProjects)
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

QWidget *StartWidget::createPluginsList() const
{
	QVBoxLayout *mainLayout = new QVBoxLayout;
	foreach (Id const &editor, mMainWindow->editorManager().editors()) {
		Id const editorTmpId = Id::loadFromString("qrm:/" + editor.editor());
		foreach (Id const &diagram, mMainWindow->editorManager().diagrams(editorTmpId)) {
			QWidget * const pluginWidget = createPluginButton(editor, diagram);
			mainLayout->addWidget(pluginWidget);
		}
	}

	mainLayout->addStretch();

	QWidget *innerWidget = new QWidget;
	innerWidget->setLayout(mainLayout);

	QScrollArea *parentArea = new QScrollArea;
	parentArea->setFrameShape(QFrame::Panel);
	parentArea->setFrameShadow(QFrame::Sunken);
	parentArea->setWidgetResizable(true);
	parentArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	parentArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	parentArea->setWidget(innerWidget);

	return parentArea;
}

QWidget *StartWidget::createPluginButton(Id const &editor, Id const &diagram) const
{
	EditorManagerInterface const &editorManagerInterface = mMainWindow->editorManager();

	QString const diagramName = editorManagerInterface.diagramName(editor.editor(), diagram.diagram());
	QString const diagramNodeName = editorManagerInterface.diagramNodeName(editor.editor(), diagram.diagram());

	if (diagramNodeName.isEmpty()) {
		return nullptr;
	}

	QPushButton *result = new QPushButton(diagramName);
	result->setFocusPolicy(Qt::StrongFocus);
	result->setStyleSheet(BrandManager::styles()->createDiagramButtonStyle());
	result->setFlat(true);
	result->setToolTip(tr("Editor: ") + editor.editor() + tr("; Diagram: ") + diagram.diagram());

	QSignalMapper *pluginMapper = new QSignalMapper(result);
	pluginMapper->setMapping(result, "qrm:/" + editor.editor() + "/" + diagram.diagram() + "/" + diagramNodeName);
	connect(result, SIGNAL(clicked()), pluginMapper, SLOT(map()));
	connect(pluginMapper, SIGNAL(mapped(QString)), mMainWindow, SLOT(createDiagram(QString)));

	return result;
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
	mInterpreterButtonsSeparator->setVisible(visible);
	mOpenInterpreterButton->setVisible(visible);
	mCreateInterpreterButton->setVisible(visible);
}

void StartWidget::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);

	QStyleOption styleOption;
	styleOption.initFrom(this);
	QPainter painter(this);
	style()->drawPrimitive(QStyle::PE_Widget, &styleOption, &painter, this);
}

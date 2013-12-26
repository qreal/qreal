#include "startWidget.h"

#include <qrkernel/settingsManager.h>

#include "dialogs/suggestToCreateDiagramDialog.h"
#include "mainwindow/mainWindow.h"
#include "suggestToCreateDiagramWidget.h"
#include "recentProjectsListWidget.h"
#include "brandManager/brandManager.h"

using namespace qReal;

StartWidget::StartWidget(MainWindow *mainWindow, ProjectManager *projectManager)
	: mMainWindow(mainWindow)
	, mProjectManager(projectManager)
	, mProjectListSize(5)  // TODO: Why 5?
{
	setStyleSheet(BrandManager::styles()->startTabBackgroundStyle());

	QWidget * const header = createHeader();
	QWidget * const recentProjects = createRecentProjectsWidget();
	QWidget * const projectsManagement = createProjectsManagementWidget();

	QVBoxLayout * mainLayout = new QVBoxLayout;
	QHBoxLayout * contentsLayout = new QHBoxLayout;

	mainLayout->addWidget(header);
	mainLayout->addLayout(contentsLayout);
	contentsLayout->addWidget(recentProjects);
	contentsLayout->addWidget(projectsManagement);
	contentsLayout->setStretch(0, 10);
	contentsLayout->setStretch(1, 20);

	setLayout(mainLayout);
}

QWidget *StartWidget::createHeader()
{
	QLabel * const appName = new QLabel(BrandManager::applicationName());
	appName->setStyleSheet(BrandManager::styles()->startTabLabelLevel1Style());

	QLabel * const appLogo = new QLabel;
	appLogo->setPixmap(QPixmap::fromImage(BrandManager::applicationLogo()));
	appLogo->setScaledContents(true);
	appLogo->setFixedSize(200, 100);

	QHBoxLayout * const headerLayout = new QHBoxLayout;
	headerLayout->addWidget(appName);
	headerLayout->addStretch();
	headerLayout->addWidget(appLogo);

	QWidget * const header = new QWidget;
	header->setStyleSheet(BrandManager::styles()->startTabHeaderBackgroundStyle());
	header->setLayout(headerLayout);

	return header;
}

QWidget *StartWidget::createRecentProjectsWidget()
{
	QString const recentProjects = SettingsManager::value("recentProjects").toString();
	if (recentProjects.isEmpty() || mMainWindow->editorManager().editors().isEmpty()) {
		return nullptr;
	}

	QWidget * const result = new QWidget;
	result->setStyleSheet(BrandManager::styles()->startTabRecentProjectsBackgroundStyle());
	result->setLayout(createRecentProjectsList(recentProjects));
	return result;
}

QWidget *StartWidget::createProjectsManagementWidget()
{
	QVBoxLayout * const projectsManagementLayout = new QVBoxLayout;

	QLabel * const createProjectLabel = new QLabel(tr("Getting started"));
	createProjectLabel->setStyleSheet(BrandManager::styles()->startTabLabelLevel2Style());
	projectsManagementLayout->addWidget(createProjectLabel);
	projectsManagementLayout->addWidget(createSpacer());

	QPushButton * const openProjectButton = createCommandButton(tr("&Open existing project"));
	connect(openProjectButton, &QPushButton::clicked, this, &StartWidget::openExistingProject);
	projectsManagementLayout->addWidget(openProjectButton);
	projectsManagementLayout->addWidget(createSpacer());

	Id const theOnlyDiagram = mMainWindow->editorManager().theOnlyDiagram();
	if (!theOnlyDiagram.isNull()) {
		Id const editor = mMainWindow->editorManager().editors()[0];
		QString const diagramIdString = mMainWindow->editorManager().diagramNodeNameString(editor, theOnlyDiagram);

		QPushButton * const newProjectButton = createCommandButton(tr("&New project"));
		projectsManagementLayout->addWidget(newProjectButton);

		QSignalMapper *newProjectMapper = new QSignalMapper(this);
		newProjectMapper->setMapping(newProjectButton, diagramIdString);
		connect(newProjectButton, SIGNAL(clicked()), newProjectMapper, SLOT(map()));
		connect(newProjectMapper, SIGNAL(mapped(QString)), this, SLOT(createProjectWithDiagram(QString)));
	} else {
		if (!mMainWindow->editorManager().editors().isEmpty()) {
			QWidget * const pluginsWidget = createPluginsList();

			projectsManagementLayout->addWidget(pluginsWidget);
		} else {
			openProjectButton->hide();
		}
	}

	mOpenInterpreterButton = createCommandButton(tr("Open &interpreted diagram"));
	mCreateInterpreterButton = createCommandButton(tr("&Create interpreted diagram"));
	connect(mOpenInterpreterButton, SIGNAL(clicked()), this, SLOT(openInterpretedDiagram()));
	connect(mCreateInterpreterButton, SIGNAL(clicked()), this, SLOT(createInterpretedDiagram()));

	projectsManagementLayout->addWidget(createSpacer());
	projectsManagementLayout->addWidget(mCreateInterpreterButton);
	projectsManagementLayout->addWidget(mOpenInterpreterButton);

	projectsManagementLayout->addStretch(0);

	QWidget * const result = new QWidget;
	result->setLayout(projectsManagementLayout);
	result->setStyleSheet(BrandManager::styles()->startTabProjectsManagementBackgroundStyle());
	return result;
}

void StartWidget::openRecentProject(QString const &fileName)
{
	mProjectManager->open(fileName);
}

void StartWidget::openExistingProject()
{
	mProjectManager->suggestToOpenExisting();
}

void StartWidget::createProjectWithDiagram(QString const &idString)
{
	mMainWindow->createProject(idString);
}

QLayout *StartWidget::createRecentProjectsList(QString const &recentProjects)
{
	QVBoxLayout * const mainLayout = new QVBoxLayout;
	QVBoxLayout * const recentProjectsLayout = new QVBoxLayout;
	recentProjectsLayout->setContentsMargins(0, 0, 0, 0);

	QLabel * const recentProjectsLabel = new QLabel(tr("Recent projects"));
	recentProjectsLabel->setWordWrap(true);
	recentProjectsLabel->setStyleSheet(BrandManager::styles()->startTabLabelLevel2Style());

	mainLayout->addWidget(recentProjectsLabel);
	mainLayout->addWidget(createSpacer());
	mainLayout->addLayout(recentProjectsLayout);

	mainLayout->addStretch(0);

	QSignalMapper * const projectNameMapper = new QSignalMapper(this);
	connect(projectNameMapper, SIGNAL(mapped(QString)), this, SLOT(openRecentProject(QString)));

	int i = 0;
	for (QString const &project : recentProjects.split(";", QString::SkipEmptyParts)) {
		QString const name = project.split("/").last().split("\\").last();
		QPushButton * const projectItem = createCommandButton(name);
		projectItem->setToolTip(project);
		recentProjectsLayout->addWidget(projectItem);

		projectNameMapper->setMapping(projectItem, project);
		connect(projectItem, SIGNAL(clicked()), projectNameMapper, SLOT(map()));

		++i;
		if (i >= mProjectListSize) {
			break;
		}
	}

	return mainLayout;
}

QWidget *StartWidget::createPluginsList()
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
	mainLayout->setContentsMargins(0, 0, 0, 0);

	QWidget *innerWidget = new QWidget;
	innerWidget->setLayout(mainLayout);

	QScrollArea *parentArea = new QScrollArea;
	parentArea->setFrameShape(QFrame::NoFrame);
	parentArea->setWidgetResizable(true);
	parentArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	parentArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	parentArea->setWidget(innerWidget);

	return parentArea;
}

QWidget *StartWidget::createPluginButton(Id const &editor, Id const &diagram)
{
	EditorManagerInterface const &editorManagerInterface = mMainWindow->editorManager();

	QString const diagramName = editorManagerInterface.diagramName(editor.editor(), diagram.diagram());
	QString const diagramNodeName = editorManagerInterface.diagramNodeName(editor.editor(), diagram.diagram());

	if (diagramNodeName.isEmpty()) {
		return nullptr;
	}

	QPushButton * const result = createCommandButton(tr("Create ") + diagramName);
	result->setFocusPolicy(Qt::StrongFocus);
	result->setStyleSheet(BrandManager::styles()->startTabButtonStyle());
	result->setToolTip(tr("Editor: ") + editor.editor() + tr("; Diagram: ") + diagram.diagram());

	QSignalMapper *pluginMapper = new QSignalMapper(result);
	pluginMapper->setMapping(result, "qrm:/" + editor.editor() + "/" + diagram.diagram() + "/" + diagramNodeName);
	connect(result, SIGNAL(clicked()), pluginMapper, SLOT(map()));
	connect(pluginMapper, SIGNAL(mapped(QString)), mMainWindow, SLOT(createDiagram(QString)));

	return result;
}

QWidget *StartWidget::createSpacer()
{
	QWidget * const result = new QWidget;
	result->setFixedHeight(30);
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
	result->setFlat(true);
	result->setStyleSheet(BrandManager::styles()->startTabButtonStyle());
	return result;
}

void StartWidget::setVisibleForInterpreterButton(bool const visible)
{
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

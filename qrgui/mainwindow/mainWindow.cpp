#include "mainWindow.h"
#include "ui_mainWindow.h"

#include <QtCore/QProcess>
#include <QtGui/QDialog>
#include <QtGui/QPrinter>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QListWidget>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QPrintDialog>
#include <QtGui/QProgressBar>
#include <QtGui/QListWidgetItem>
#include <QtCore/QPluginLoader>

#include <QtSvg/QSvgGenerator>

#include <QtCore/QDebug>
#include <QAbstractButton>

#include "errorReporter.h"

#include "../editorPluginInterface/editorInterface.h"
#include "shapeEdit/shapeEdit.h"
#include "propertyEditorProxyModel.h"
#include "../dialogs/gesturesShow/gesturesWidget.h"

#include "../models/models.h"
#include "../view/editorView.h"
#include "../umllib/element.h"
#include "../dialogs/pluginDialog.h"
#include "../dialogs/checkoutDialog.h"
#include "../generators/xmi/xmiHandler.h"
#include "../generators/java/javaHandler.h"
#include "../pluginManager/listenerManager.h"
#include "../../qrkernel/settingsManager.h"

#include "../../qrkernel/timeMeasurer.h"
#include "dotRunner.h"

using namespace qReal;

QString const unsavedDir = "unsaved";

MainWindow::MainWindow()
		: mUi(new Ui::MainWindowUi)
		, mCloseEvent(NULL)
		, mModels(NULL)
		, mListenerManager(NULL)
		, mPropertyModel(mEditorManager)
		, mGesturesWidget(NULL)
		, mRootIndex(QModelIndex())
		, mErrorReporter(NULL)
		, mIsFullscreen(false)
		, mTempDir(qApp->applicationDirPath() + "/" + unsavedDir)
		, mPreferencesDialog(this)
		, mHelpBrowser(NULL)
		, mIsNewProject(true)
		, mUnsavedProjectIndicator(false)
		, mRecentProjectsLimit(5)
		, mRecentProjectsMapper(new QSignalMapper())
{
	mCodeTabManager = new QMap<EditorView*, CodeArea*>();

	TimeMeasurer timeMeasurer("MainWindow::MainWindow");
	timeMeasurer.doNothing(); //to avoid the unused variables problem

	bool showSplash = SettingsManager::value("Splashscreen", true).toBool();

	QSplashScreen* splash =
			new QSplashScreen(QPixmap(":/icons/kroki3.PNG"), Qt::SplashScreen | Qt::WindowStaysOnTopHint);

	QProgressBar *progress = createProgressBar(splash);

	QDir imagesDir(SettingsManager::value("pathToImages", "/someWeirdDirectoryName").toString());
	if (!imagesDir.exists()) {
		SettingsManager::setValue("pathToImages", qApp->applicationDirPath() + "/images/iconset1");
	}

	// =========== Step 1: splash screen loaded, progress bar initialized ===========

	progress->setValue(5);

	mUi->setupUi(this);

	if (showSplash) {
		splash->show();
		QApplication::processEvents();
	}
	else {
		mUi->actionShowSplash->setChecked(false);
	}

	initRecentProjectsMenu();
	initToolManager();
	initTabs();

	// =========== Step 2: Ui is ready, splash screen shown ===========

	progress->setValue(20);

	initMiniMap();
	initGridProperties();

	// =========== Step 3: Ui connects are done ===========

	progress->setValue(40);

	initDocks();

	SettingsManager::setValue("temp", mTempDir);
	QDir dir(qApp->applicationDirPath());
	if (!dir.cd(mTempDir))
		QDir().mkdir(mTempDir);

	QFileInfo saveFile(SettingsManager::value("saveFile", mSaveFile).toString());

	if (saveFile.exists())
		mSaveFile = saveFile.absoluteFilePath();

	mModels = new models::Models(saveFile.absoluteFilePath(), mEditorManager);

	mErrorReporter = new gui::ErrorReporter(mUi->errorListWidget, mUi->errorDock);
	mErrorReporter->updateVisibility(SettingsManager::value("warningWindow", true).toBool());

	mPreferencesDialog.init(mUi->actionShow_grid, mUi->actionShow_alignment, mUi->actionSwitch_on_grid, mUi->actionSwitch_on_alignment);

	// =========== Step 4: Property editor and model explorers are initialized ===========

	progress->setValue(60);
	loadPlugins();
	initToolPlugins();
	showMaximized();

	// =========== Step 5: Plugins are loaded ===========

	progress->setValue(70);

	initWindowTitle();

	if (!SettingsManager::value("maximized", true).toBool()) {
		showNormal();
		resize(SettingsManager::value("size", QSize(1024, 800)).toSize());
		move(SettingsManager::value("pos", QPoint(0, 0)).toPoint());
	}

	// =========== Step 6: Save loaded, models initialized ===========

	progress->setValue(80);

	if (!checkPluginsAndReopen(splash))
		return;

	mGesturesWidget = new GesturesWidget();

	initExplorers();

	connectActions();

	// =========== Step 7: Save consistency checked, interface is initialized with models ===========

	progress->setValue(100);

	if (showSplash)
		splash->close();
	delete splash;

	mIsNewProject = (mSaveFile.isEmpty() || mSaveFile == mTempDir + ".qrs");

	if (mModels->graphicalModel()->rowCount() > 0) {
		openNewTab(mModels->graphicalModel()->index(0, 0, QModelIndex()));
	}

	if (SettingsManager::value("diagramCreateSuggestion", true).toBool())
		suggestToCreateDiagram();

	mDocksVisibility.clear();

	if (mIsNewProject)
		saveAs(mTempDir);

	setAutoSaveParameters();
	connect(&mAutoSaveTimer, SIGNAL(timeout()), this, SLOT(autosave()));
	connectWindowTitle();

}

void MainWindow::connectActions()
{
	mUi->actionShow_grid->setChecked(SettingsManager::value("ShowGrid", true).toBool());
	mUi->actionShow_alignment->setChecked(SettingsManager::value("ShowAlignment", true).toBool());
	mUi->actionSwitch_on_grid->setChecked(SettingsManager::value("ActivateGrid", true).toBool());
	mUi->actionSwitch_on_alignment->setChecked(SettingsManager::value("ActivateAlignment", true).toBool());
	connect(mUi->actionQuit, SIGNAL(triggered()), this, SLOT(close()));

	connect(mUi->actionShowSplash, SIGNAL(toggled(bool)), this, SLOT (toggleShowSplash(bool)));
	connect(mUi->actionOpen, SIGNAL(triggered()), this, SLOT(openNewProject()));
	connect(mUi->actionSave, SIGNAL(triggered()), this, SLOT(saveAll()));
	connect(mUi->actionSave_as, SIGNAL(triggered()), this, SLOT(saveProjectAs()));
	connect(mUi->actionSave_diagram_as_a_picture, SIGNAL(triggered()), this, SLOT(saveDiagramAsAPicture()));
	connect(mUi->actionPrint, SIGNAL(triggered()), this, SLOT(print()));
	connect(mUi->actionMakeSvg, SIGNAL(triggered()), this, SLOT(makeSvg()));
	connect(mUi->actionNewProject, SIGNAL(triggered()), this, SLOT(createProject()));
	connect(mUi->actionCloseProject, SIGNAL(triggered()), this, SLOT(closeProjectAndSave()));
	connect(mUi->actionImport, SIGNAL(triggered()), this, SLOT(importProject()));
	connect(mUi->actionDeleteFromDiagram, SIGNAL(triggered()), this, SLOT(deleteFromDiagram()));

	//	connect(mUi->actionExport_to_XMI, SIGNAL(triggered()), this, SLOT(exportToXmi()));
	//	connect(mUi->actionGenerate_to_Java, SIGNAL(triggered()), this, SLOT(generateToJava()));
	connect(mUi->actionPreferences, SIGNAL(triggered()), this, SLOT(showPreferencesDialog()));

	//	connect(mUi->actionParse_Java_Libraries, SIGNAL(triggered()), this, SLOT(parseJavaLibraries()));

	connect(mUi->actionPlugins, SIGNAL(triggered()), this, SLOT(settingsPlugins()));
	connect(mUi->actionShow_grid, SIGNAL(toggled(bool)), this, SLOT(showGrid(bool)));
	connect(mUi->actionShow_alignment, SIGNAL(toggled(bool)), this, SLOT(showAlignment(bool)));
	connect(mUi->actionSwitch_on_grid, SIGNAL(toggled(bool)), this, SLOT(switchGrid(bool)));
	connect(mUi->actionSwitch_on_alignment, SIGNAL(toggled(bool)), this, SLOT(switchAlignment(bool)));

	connect(mUi->actionHelp, SIGNAL(triggered()), this, SLOT(showHelp()));
	connect(mUi->actionAbout, SIGNAL(triggered()), this, SLOT(showAbout()));
	connect(mUi->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

	connect(mUi->actionShow, SIGNAL(triggered()), this, SLOT(showGestures()));

	connect(mUi->actionFullscreen, SIGNAL(triggered()), this, SLOT(fullscreen()));

}

QModelIndex MainWindow::rootIndex() const
{
	return mRootIndex;
}

void MainWindow::keyPressEvent(QKeyEvent *keyEvent)
{
	if (keyEvent->modifiers() == Qt::AltModifier && keyEvent->key() == Qt::Key_X) {
		close();
	} else if (keyEvent->key() == Qt::Key_F2
			|| (keyEvent->modifiers() == Qt::ControlModifier && keyEvent->key() == Qt::Key_S))
	{
		saveAll();
	} else if (keyEvent->modifiers() == Qt::ControlModifier && keyEvent->key() == Qt::Key_W) {
		closeTab(mUi->tabs->currentIndex());
	} else if (keyEvent->key() == Qt::Key_F1){
		showHelp();
	}
}

MainWindow::~MainWindow()
{
	QDir().rmdir(mTempDir);
	delete mListenerManager;
	delete mErrorReporter;
	delete mHelpBrowser;
	SettingsManager::instance()->saveData();
	delete mRecentProjectsMenu;
	delete mRecentProjectsMapper;
	delete mGesturesWidget;
	delete mModels;
}

EditorManager* MainWindow::manager()
{
	return &mEditorManager;
}

void MainWindow::finalClose()
{
	mCloseEvent->accept();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	if (mUnsavedProjectIndicator) {
		switch (openSaveOfferDialog()) {
		case QMessageBox::AcceptRole:
			saveAll();
			break;
		case QMessageBox::RejectRole:
			event->ignore();
			return;
		}
	}
	mCloseEvent = event;
	SettingsManager::setValue("maximized", isMaximized());
	SettingsManager::setValue("size", size());
	SettingsManager::setValue("pos", pos());
}

void MainWindow::loadPlugins()
{
	foreach (Id const editor, mEditorManager.editors()) {
		foreach (Id const diagram, mEditorManager.diagrams(editor)) {
			mUi->paletteTree->addEditorElements(mEditorManager, editor, diagram);
		}
	}
	mUi->paletteTree->initDone();
}

void MainWindow::adjustMinimapZoom(int zoom)
{
	mUi->minimapView->resetMatrix();
	mUi->minimapView->scale(0.01 * zoom, 0.01 * zoom);
}

void MainWindow::selectItemWithError(Id const &id)
{
	if (id == Id::rootId())
		return;

	setIndexesOfPropertyEditor(id);
	centerOn(id);
}

void MainWindow::selectItem(Id const &id)
{
	if (id == Id::rootId())
		return;

	setIndexesOfPropertyEditor(id);
	centerOn(id);
}

void MainWindow::activateItemOrDiagram(QModelIndex const &idx, bool bl, bool isSetSel)
{
	QModelIndex const parent = idx.parent();
	int const numTab = getTabIndex(idx);

	if (numTab != -1)
		mUi->tabs->setCurrentIndex(numTab);
	else
		openNewTab(idx);

	if (mUi->tabs->isEnabled()) {
		if (parent == mModels->graphicalModelAssistApi().rootIndex()) {
			getCurrentTab()->mvIface()->setRootIndex(idx);
		} else {
			getCurrentTab()->mvIface()->setRootIndex(parent);
			// select this item on diagram
			getCurrentTab()->scene()->clearSelection();
			Element * const e = (static_cast<EditorViewScene *>(getCurrentTab()->scene()))->getElem(idx.data(roles::idRole).value<Id>());
			if (e) {
				e->setColorRect(bl);
				if (isSetSel)
					e->setSelected(true);
			} else {
				Q_ASSERT(false);
			}
		}
	}
}

void MainWindow::activateItemOrDiagram(Id const &id, bool bl, bool isSetSel)
{
	if (mModels->graphicalModelAssistApi().isGraphicalId(id)) {
		activateItemOrDiagram(mModels->graphicalModelAssistApi().indexById(id), bl, isSetSel);
	} else {
		IdList const graphicalIds = mModels->graphicalModelAssistApi().graphicalIdsByLogicalId(id);
		if (graphicalIds.count() == 0) {
			return;
		}
		activateItemOrDiagram(mModels->graphicalModelAssistApi().indexById(graphicalIds[0]), bl, isSetSel);
	}
}

void MainWindow::sceneSelectionChanged()
{
	if (!getCurrentTab()) {
		return;
	}

	QList<Element*> elements;
	QList<Element*> selected;
	QList<QGraphicsItem*> items = getCurrentTab()->scene()->items();

	foreach (QGraphicsItem* item, items) {
		Element* element = dynamic_cast<Element*>(item);
		if (element) {
			elements.append(element);
			if (element->isSelected()) {
				selected.append(element);
				element->selectionState(true);
			} else {
				element->selectionState(false);
				element->singleSelectionState(false);
			}
		}
	}

	if (selected.isEmpty()) {
		mUi->graphicalModelExplorer->setCurrentIndex(QModelIndex());
		mPropertyModel.clearModelIndexes();
	} else if (selected.length() > 1) {
		foreach(Element* notSingleSelected, selected) {
			notSingleSelected->singleSelectionState(false);
		}
	} else {
		Element* const singleSelected = selected.at(0);
		singleSelected->singleSelectionState(true);
		setIndexesOfPropertyEditor(singleSelected->id());

		QModelIndex const index = mModels->graphicalModelAssistApi().indexById(singleSelected->id());
		if (index.isValid()) {
			mUi->graphicalModelExplorer->setCurrentIndex(index);
		}
	}
}

QString MainWindow::getWorkingFile(QString const &dialogWindowTitle, bool save)
{
	QString fileName;
	QDir const lastSaveDir = QFileInfo(mSaveFile).absoluteDir();

	if (save)
		fileName = QFileDialog::getSaveFileName(this, dialogWindowTitle
				, lastSaveDir.absolutePath(), tr("QReal Save File(*.qrs)"));
	else
		fileName = QFileDialog::getOpenFileName(this, dialogWindowTitle
				, lastSaveDir.absolutePath(), tr("QReal Save File(*.qrs)"));
	SettingsManager::setValue("saveFile", fileName);
	mSaveFile = fileName;

	refreshRecentProjectsList(fileName);

	return fileName;
}

bool MainWindow::checkPluginsAndReopen(QSplashScreen* const splashScreen)
{
	IdList missingPlugins = mEditorManager.checkNeededPlugins(mModels->logicalRepoApi(), mModels->graphicalRepoApi());
	bool haveMissingPlugins = !missingPlugins.isEmpty();
	bool loadingCancelled = false;

	while (haveMissingPlugins && !loadingCancelled) {

		QString text = tr("These plugins are not present, but needed to load the save:\n");
		foreach (Id const id, missingPlugins)
			text += id.editor() + "\n";
		text += tr("Do you want to create new project?");

		QMessageBox::StandardButton const button = QMessageBox::question(this
				, tr("Some plugins are missing"), text, QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

		if (splashScreen)
			splashScreen->close();

		if (button == QMessageBox::Yes) {
			if (!openNewProject())
				loadingCancelled = true;
		}
		else
			loadingCancelled = true;
		missingPlugins = mEditorManager.checkNeededPlugins(
				mModels->logicalRepoApi(), mModels->graphicalRepoApi());
		haveMissingPlugins = !missingPlugins.isEmpty();
	}

	if (loadingCancelled) {
		return false;
	}

	return true;
}

bool MainWindow::importProject()
{
	return import(getWorkingFile(tr("Select file with a save to import"), false));
}

bool MainWindow::import(QString const &fileName)
{
	if (!QFile(fileName).exists()) {
		return false;
	}
	mModels->repoControlApi().importFromDisk(fileName);
	mModels->reinit();
	return true;
}

bool MainWindow::openNewProject()
{
	if (mUnsavedProjectIndicator) {
		switch (openSaveOfferDialog()) {
		case QMessageBox::AcceptRole:
			saveAll();
			break;
		case QMessageBox::RejectRole:
			return false;
		}
	}
	return open(getWorkingFile(tr("Select file with a save to open"), false));
}

void MainWindow::refreshRecentProjectsList(QString const &fileName)
{
	QString previousString =  SettingsManager::value("recentProjects").toString();
	QStringList previousList = previousString.split(";", QString::SkipEmptyParts);
	previousList.removeOne(fileName);
	if (previousList.size() == mRecentProjectsLimit)
		previousList.removeLast();
	previousList.push_front(fileName);
	previousString = "";
	QStringListIterator iterator(previousList);
	while (iterator.hasNext())
		previousString = previousString + iterator.next() + ";";
	SettingsManager::setValue("recentProjects", previousString);
}

void MainWindow::openRecentProjectsMenu()
{
	//SettingsManager::setValue("recentProjects", (QString) "");
	mRecentProjectsMenu->clear();
	QString const stringList = SettingsManager::value("recentProjects").toString();
	QStringList const recentProjects = stringList.split(";", QString::SkipEmptyParts);
	foreach (QString projectPath, recentProjects) {
		mRecentProjectsMenu->addAction(projectPath);
		QObject::connect(mRecentProjectsMenu->actions().last(), SIGNAL(triggered()), mRecentProjectsMapper, SLOT(map()));
		mRecentProjectsMapper->setMapping(mRecentProjectsMenu->actions().last(), projectPath);
	}

	QObject::connect(mRecentProjectsMapper, SIGNAL(mapped(const QString &)), this, SLOT(saveAllAndOpen(const QString &)));
}

void MainWindow::saveAllAndOpen(QString const &dirName)
{
	saveAll();
	open(dirName);
}

bool MainWindow::open(QString const &fileName)
{
	if (!QFile(fileName).exists() && fileName != "") {
		return false;
	}

	refreshRecentProjectsList(fileName);

	closeProject();

	mModels->repoControlApi().open(fileName);
	mModels->reinit();

	if (!checkPluginsAndReopen(NULL))
		return false;
	mPropertyModel.setSourceModels(mModels->logicalModel(), mModels->graphicalModel());
	mUi->graphicalModelExplorer->setModel(mModels->graphicalModel());
	mUi->logicalModelExplorer->setModel(mModels->logicalModel());

	connectWindowTitle();
	mSaveFile = fileName;
	QString windowTitle = mToolManager.customizer()->windowTitle();
	if (!fileName.isEmpty()) {
		setWindowTitle(windowTitle + " - " + mSaveFile);
	}
	else
		setWindowTitle(windowTitle + " - unsaved project");
	return true;
}

void MainWindow::closeAllTabs()
{
	int const tabCount = mUi->tabs->count();
	for (int i = 0; i < tabCount; i++)
		closeTab(i);
	disconnectWindowTitle();
}

void MainWindow::setShape(QString const &data, QPersistentModelIndex const &index, int const &role)
{
	// const_cast here is ok, since we need to set a shape in a correct model, and
	// not going to use this index anymore.
	QAbstractItemModel * const model = const_cast<QAbstractItemModel *>(index.model());
	model->setData(index, data, role);
}

void MainWindow::print()
{
	QPrinter printer(QPrinter::HighResolution);
	QPrintDialog dialog(&printer, this);
	if (dialog.exec() == QDialog::Accepted) {
		QPainter painter(&printer);
		getCurrentTab()->scene()->render(&painter);
	}
}

void MainWindow::makeSvg()
{
	QSvgGenerator newSvg;

	QString fileName = QFileDialog::getSaveFileName(this);
	if (fileName.isEmpty())
		return;

	newSvg.setFileName(fileName);
	newSvg.setSize(QSize(800, 600));

	QPainter painter(&newSvg);
	getCurrentTab()->scene()->render(&painter);
}

void MainWindow::settingsPlugins()
{
	PluginDialog dialog(mEditorManager , this);
	dialog.exec();
}

void MainWindow::deleteFromExplorer(bool isLogicalModel)
{
	QModelIndex const index = isLogicalModel
			? (mUi->logicalModelExplorer->currentIndex())
			: (mUi->graphicalModelExplorer->currentIndex());

	if (!index.isValid()) {
		return;
	}
	EditorView const * const view = getCurrentTab();
	EditorViewScene* scene = NULL;
	if (view) {
		scene = dynamic_cast<EditorViewScene*>(view->scene());
	}

	IdList graphicalIdList;
	if (isLogicalModel) {
		Id const logicalId = mModels->logicalModelAssistApi().idByIndex(index);
		graphicalIdList = mModels->graphicalModelAssistApi().graphicalIdsByLogicalId(logicalId);
	} else {
		Id const graphicalId = mModels->graphicalModelAssistApi().idByIndex(index);
		graphicalIdList.append(graphicalId);
	}

	QList<NodeElement*> itemsToArrangeLinks;
	foreach (Id const &graphicalId, graphicalIdList) {
		bool const tabClosed = closeTab(mModels->graphicalModelAssistApi().indexById(graphicalId));
		if (scene && !tabClosed) {
			QGraphicsItem const * const item = scene->getElem(graphicalId);
			EdgeElement const * const edge = dynamic_cast<EdgeElement const *>(item);
			if (edge) {
				itemsToArrangeLinks.append(edge->src());
				itemsToArrangeLinks.append(edge->dst());
			}
		}
	}

	PropertyEditorModel* propertyEditorModel = dynamic_cast<PropertyEditorModel*>(mUi->propertyEditor->model());
	if (propertyEditorModel && propertyEditorModel->isCurrentIndex(index)) {
		propertyEditorModel->clearModelIndexes();
		mUi->propertyEditor->setRootIndex(QModelIndex());
	}

	if (isLogicalModel) {
		mModels->logicalModel()->removeRow(index.row(), index.parent());
	} else {
		mModels->graphicalModel()->removeRow(index.row(), index.parent());
	}

	foreach (NodeElement *item, itemsToArrangeLinks) {
		if (item) {
			item->arrangeLinks();
		}
	}
}

void MainWindow::deleteFromScene()
{
	foreach (QGraphicsItem *item, getCurrentTab()->scene()->selectedItems())
		deleteFromScene(item);
}

void MainWindow::deleteFromScene(QGraphicsItem *target)
{
	Element *elem = dynamic_cast<Element *>(target);
	if (elem) {
		QPersistentModelIndex const index = mModels->graphicalModelAssistApi().indexById(elem->id());
		if (index.isValid()) {
			NodeElement* const node = dynamic_cast<NodeElement*>(elem);
			if (node) {
				node->highlightEdges();
			}
			EdgeElement const * const edge = dynamic_cast<EdgeElement const *>(elem);
			NodeElement* source = NULL;
			NodeElement* destination = NULL;
			if (edge) {
				source = edge->src();
				destination = edge->dst();
			}
			PropertyEditorModel* propertyEditorModel = static_cast<PropertyEditorModel*>(mUi->propertyEditor->model());
			if (propertyEditorModel->isCurrentIndex(index))
				propertyEditorModel->clearModelIndexes();
			mUi->propertyEditor->setRootIndex(QModelIndex());
			mModels->graphicalModel()->removeRow(index.row(), index.parent());
			if (edge) {
				if (source) {
					source->arrangeLinks();
				}
				if (destination) {
					destination->arrangeLinks();
				}
			}
		}
		if (getCurrentTab() != NULL && getCurrentTab()->scene() != NULL)
			getCurrentTab()->scene()->invalidate();
	}
}

void MainWindow::deleteFromDiagram()
{
	if (mModels->graphicalModel()) {
		if (mUi->graphicalModelExplorer->hasFocus()) {
			deleteFromExplorer(false);
		} else if (getCurrentTab() != NULL && getCurrentTab()->hasFocus()) {
			deleteFromScene();
		}
	}
	if (mModels->logicalModel()) {
		if (mUi->logicalModelExplorer->hasFocus()) {
			deleteFromExplorer(true);
		}
	}

	if (getCurrentTab() != NULL && getCurrentTab()->scene() != NULL) {
		getCurrentTab()->scene()->invalidate();
	}
}
void MainWindow::editWindowTitle()
{
	if (!mUnsavedProjectIndicator){
		setWindowTitle(windowTitle() + " [modified]");
		mUnsavedProjectIndicator = true;
	}
}

void MainWindow::showAbout()
{
	QMessageBox::about(this, tr("About QReal:Robots"),
			tr("<b>QReal:Robots<b><br><br><a href=\"http://qreal.ru/\">http://qreal.ru/</a>"));
}

void MainWindow::showHelp()
{
	// FIXME: ":/qreal-robots.qhc" doesn't work for some reason
	QHelpEngine * const helpEngine = new QHelpEngine("./qreal-robots.qhc");
	helpEngine->setupData();

	helpEngine->setCurrentFilter("QReal:Robots");

	mHelpBrowser = new HelpBrowser(helpEngine);
	mHelpBrowser->setSource(helpEngine->linksForIdentifier("QReal")["QReal:Robots"]);

	QSplitter * const helpPanel = new QSplitter(Qt::Horizontal);
	helpPanel->setGeometry(QRect(50, 50, 1000, 800));
	helpPanel->setWindowTitle("QReal:Robots Help Center");

	helpPanel->insertWidget(0, helpEngine->contentWidget());
	helpPanel->insertWidget(1, mHelpBrowser);
	helpPanel->setStretchFactor(1, 1);
	helpPanel->show();

	connect(helpEngine->contentWidget(), SIGNAL(linkActivated(const QUrl &)), mHelpBrowser, SLOT(setSource(const QUrl &)));
}

void MainWindow::toggleShowSplash(bool show)
{
	SettingsManager::setValue("Splashscreen", show);
}

void MainWindow::checkoutDialogOk()
{
}

void MainWindow::checkoutDialogCancel()
{
}

void MainWindow::exportToXmi()
{
	generators::XmiHandler xmi(mModels->logicalRepoApi());

	QString const fileName = QFileDialog::getSaveFileName(this);
	if (fileName.isEmpty())
		return;

	QString const errors = xmi.exportToXmi(fileName);

	if (!errors.isEmpty()) {
		QMessageBox::warning(this, tr("errors"), "Some errors occured. Export may be incorrect. Errors list: \n" + errors);
	} else {
		QMessageBox::information(this, tr("finished"), "Export is finished");
	}
}

void MainWindow::generateToJava()
{
	generators::JavaHandler java(mModels->logicalRepoApi());

	QString const dirName = QFileDialog::getExistingDirectory(this);
	if (dirName.isEmpty())
		return;

	QString const errors = java.generateToJava(dirName);

	if (!errors.isEmpty()) {
		QMessageBox::warning(this, tr("errors"), "Some errors occured. Export may be incorrect. Errors list: \n" + errors);
	} else {
		QMessageBox::information(this, tr("finished"), "Export is finished");
	}
}

void MainWindow::parseJavaLibraries()
{
	generators::JavaHandler java(mModels->logicalRepoApi());

	QString const dirName = QFileDialog::getExistingDirectory(this);
	if (dirName.isEmpty()) {
		return;
	}

	QString const errors = java.parseJavaLibraries(dirName);

	if (!errors.isEmpty()) {
		QMessageBox::warning(this, tr("errors"), tr("Some errors occured. Export may be incorrect. Errors list:") + " \n" + errors);
	} else {
		QMessageBox::information(this, tr("finished"), tr("Parsing is finished"));
	}
}

bool MainWindow::unloadPlugin(QString const &pluginName)
{
	if (mEditorManager.editors().contains(Id(pluginName))) {
		IdList const diagrams = mEditorManager.diagrams(Id(pluginName));

		if (!mEditorManager.unloadPlugin(pluginName)) {
			return false;
		}
		foreach (Id const &diagram, diagrams) {
			mUi->paletteTree->deleteEditor(diagram);
		}
	}
	return true;
}

bool MainWindow::loadPlugin(QString const &fileName, QString const &pluginName)
{
	if (!mEditorManager.loadPlugin(fileName)) {
		return false;
	}

	foreach (Id const &diagram, mEditorManager.diagrams(Id(pluginName))) {
		mUi->paletteTree->addEditorElements(mEditorManager, Id(pluginName), diagram);
	}
	mUi->paletteTree->initDone();
	return true;
}

bool MainWindow::pluginLoaded(QString const &pluginName)
{
	return mEditorManager.editors().contains(Id(pluginName));
}

EditorView * MainWindow::getCurrentTab()
{
	return dynamic_cast<EditorView *>(mUi->tabs->currentWidget());
}

void MainWindow::changeMiniMapSource(int index)
{
	if (index != -1) {
		mUi->tabs->setEnabled(true);
		EditorView *editorView = getCurrentTab();
		setConnectActionZoomTo(mUi->tabs->currentWidget());

		if (editorView != NULL && (static_cast<EditorViewScene*>(editorView->scene()))->mainWindow() != NULL) {
			mUi->minimapView->setScene(editorView->scene());
			getCurrentTab()->mvIface()->setModel(mModels->graphicalModel());
			getCurrentTab()->mvIface()->setLogicalModel(mModels->logicalModel());
			mRootIndex = editorView->mvIface()->rootIndex();
		}
	} else {
		mUi->tabs->setEnabled(false);
		mUi->minimapView->setScene(0);;
	}
}

void qReal::MainWindow::closeTab(int index)
{
	QWidget *widget = mUi->tabs->widget(index);
	CodeArea *possibleCodeTab = static_cast<CodeArea *>(widget);
	EditorView * deletingCodeTab = NULL;
	foreach (EditorView *diagram, mCodeTabManager->keys())
		if (mCodeTabManager->value(diagram) == possibleCodeTab)
			deletingCodeTab = diagram;
	if (deletingCodeTab != NULL)
		mCodeTabManager->remove(deletingCodeTab);
	mUi->tabs->removeTab(index);
	delete widget;
}

/*
void MainWindow::exterminate()
{
 closeAllTabs();
 mModels->repoControlApi().exterminate();
 mModels->reinit();

 PropertyEditorModel* pModel = dynamic_cast<PropertyEditorModel*>(mUi->propertyEditor->model());
 pModel->clearModelIndexes();
 mUi->propertyEditor->setRootIndex(QModelIndex());
}
*/

void MainWindow::showPreferencesDialog()
{
	disconnect(&mPreferencesDialog);
	if (getCurrentTab() != NULL) {
		connect(&mPreferencesDialog, SIGNAL(gridChanged()), getCurrentTab(), SLOT(invalidateScene()));
		connect(&mPreferencesDialog, SIGNAL(iconsetChanged()), this, SLOT(updatePaletteIcons()));
		connect(&mPreferencesDialog, SIGNAL(settingsApplied()), this, SLOT(applySettings()));
		connect(&mPreferencesDialog, SIGNAL(fontChanged()), this, SLOT(setSceneFont()));
	}
	mPreferencesDialog.exec();
	mToolManager.updateSettings();
	setAutoSaveParameters();
}

void MainWindow::openSettingsDialog(QString const &tab)
{
	mPreferencesDialog.switchCurrentTab(tab);
	showPreferencesDialog();
}

void MainWindow::setSceneFont()
{
	if (SettingsManager::value("CustomFont", true).toBool()) {
		QFont font;
		font.fromString(SettingsManager::value("CurrentFont", "ololo").toString());
		getCurrentTab()->scene()->setFont(font);
		getCurrentTab()->scene()->update();
	} else {
		getCurrentTab()->scene()->setFont(QFont(QFontDatabase::applicationFontFamilies(
				QFontDatabase::addApplicationFont(QDir::currentPath() + "/times.ttf")).at(0), 9));
		getCurrentTab()->scene()->update();
	}
}

void MainWindow::openShapeEditor(QPersistentModelIndex const &index, int role, QString const &propertyValue)
{
	ShapeEdit *shapeEdit = new ShapeEdit(index, role);
	if (!propertyValue.isEmpty())
		shapeEdit->load(propertyValue);

	// Here we are going to actually modify model to set a value of a shape.
	QAbstractItemModel *model = const_cast<QAbstractItemModel *>(index.model());
	model->setData(index, propertyValue, role);
	connect(shapeEdit, SIGNAL(shapeSaved(QString, QPersistentModelIndex const &, int const &)),
			this, SLOT(setShape(QString, QPersistentModelIndex const &, int const &)));

	mUi->tabs->addTab(shapeEdit, tr("Shape Editor"));
	mUi->tabs->setCurrentWidget(shapeEdit);
	setConnectActionZoomTo(shapeEdit);
}

void MainWindow::openShapeEditor()
{
	ShapeEdit * const shapeEdit = new ShapeEdit;
	mUi->tabs->addTab(shapeEdit, tr("Shape Editor"));
	mUi->tabs->setCurrentWidget(shapeEdit);
	setConnectActionZoomTo(shapeEdit);
}

void MainWindow::disconnectZoom(QGraphicsView* view)
{
	disconnect(mUi->actionZoom_In, SIGNAL(triggered()), view, SLOT(zoomIn()));
	disconnect(mUi->actionZoom_Out, SIGNAL(triggered()), view, SLOT(zoomOut()));
}

void MainWindow::connectZoom(QGraphicsView* view)
{
	connect(mUi->actionZoom_In, SIGNAL(triggered()), view, SLOT(zoomIn()));
	connect(mUi->actionZoom_Out, SIGNAL(triggered()), view, SLOT(zoomOut()));
}

void MainWindow::disconnectActionZoomTo(QWidget* widget)
{
	EditorView * const view = dynamic_cast<EditorView *>(widget);
	if (view != NULL)
		disconnectZoom(view);
	else {
		ShapeEdit *const shapeWidget = dynamic_cast<ShapeEdit *>(widget);
		if (shapeWidget != NULL)
			disconnectZoom(shapeWidget->getView());
	}
}

void MainWindow::connectActionZoomTo(QWidget* widget)
{
	EditorView * const view = (dynamic_cast<EditorView *>(widget));
	if (view != NULL)
		connectZoom(view);
	else {
		ShapeEdit * const shapeWidget = (dynamic_cast<ShapeEdit *>(widget));
		if (shapeWidget != NULL)
			connectZoom(shapeWidget->getView());
	}
}

void MainWindow::setConnectActionZoomTo(QWidget* widget)
{
	for (int i = 0; i < mUi->tabs->count(); i++)
		disconnectActionZoomTo(mUi->tabs->widget(i));

	connectActionZoomTo(widget);
}

void MainWindow::centerOn(Id const &id)
{
	if (mEditorManager.isDiagramNode(id))
		return;

	EditorView* const view = getCurrentTab();
	EditorViewScene* const scene = dynamic_cast<EditorViewScene*>(view->scene());
	Element* const element = scene->getElem(id);

	scene->clearSelection();
	if (element != NULL) {
		element->setSelected(true);
		view->ensureElementVisible(element);
	}
}

void MainWindow::propertyEditorScrollTo(QModelIndex const &index)
{
	mUi->propertyEditor->scrollTo(index);
}

void MainWindow::graphicalModelExplorerClicked(QModelIndex const &index)
{
	Id const id = mModels->graphicalModelAssistApi().idByIndex(index);
	setIndexesOfPropertyEditor(id);
	openNewTab(index);
	centerOn(id);
}

void MainWindow::logicalModelExplorerClicked(QModelIndex const &index)
{
	Id const logicalId = mModels->logicalModelAssistApi().idByIndex(index);
	IdList graphicalIds = mModels->graphicalModelAssistApi().graphicalIdsByLogicalId(logicalId);
	if (!graphicalIds.empty()) {
		// By now we will select first graphical representation of selected element.
		// In the future it may be needed to make this more intellectual, like
		// selecting the representation in current tab.
		Id const graphicalId = graphicalIds.first();
		QModelIndex const graphicalIndex = mModels->graphicalModelAssistApi().indexById(graphicalId);
		graphicalModelExplorerClicked(graphicalIndex);
	} else {
		setIndexesOfPropertyEditor(logicalId);
		EditorView* const view = getCurrentTab();
		if (view != NULL) {
			EditorViewScene* const scene = dynamic_cast<EditorViewScene*>(view->scene());
			scene->clearSelection();
		}
	}
}

void MainWindow::openNewTab(QModelIndex const &arg)
{
	QModelIndex index = arg;
	while (index.parent() != QModelIndex()) {
		index = index.parent();
	}

	int tabNumber = -1;
	for (int i = 0; i < mUi->tabs->count(); i++) {
		EditorView *tab = (dynamic_cast<EditorView *>(mUi->tabs->widget(i)));
		if (tab != NULL && tab->mvIface()->rootIndex() == index) {
			tabNumber = i;
			break;
		}
	}
	if (tabNumber != -1) {
		mUi->tabs->setCurrentIndex(tabNumber);
	} else {
		EditorView * const view = new EditorView(this);
		initCurrentTab(view, index);
		mUi->tabs->addTab(view, index.data().toString());
		mUi->tabs->setCurrentWidget(view);
	}

	// changing of palette active editor
	if (SettingsManager::value("PaletteTabSwitching", true).toBool()) {
		int i = 0;
		foreach (const QString &name, mUi->paletteTree->editorsNames()) {
			Id const id = mModels->graphicalModelAssistApi().idByIndex(index);
			Id const diagramId = Id(id.editor(), id.diagram());
			QString const diagramName = mEditorManager.friendlyName(diagramId);
			if (diagramName == name) {
				mUi->paletteTree->setComboBoxIndex(i);
				break;
			}
			i++;
		}
	}
}

void MainWindow::initCurrentTab(EditorView *const tab, const QModelIndex &rootIndex)
{
	if (!tab) {
		return;
	}

	tab->setMainWindow(this);
	QModelIndex const index = rootIndex;

	tab->mvIface()->setAssistApi(mModels->graphicalModelAssistApi(), mModels->logicalModelAssistApi());

	tab->mvIface()->setModel(mModels->graphicalModel());
	if (tab->sceneRect() == QRectF(0, 0, 0, 0)) {
		tab->setSceneRect(0, 0, 1, 1);
	}

	tab->mvIface()->setLogicalModel(mModels->logicalModel());
	tab->mvIface()->setRootIndex(index);

	// Connect after setModel etc. because of signal selectionChanged was sent when there were old indexes
	connect(tab->scene(), SIGNAL(selectionChanged()), SLOT(sceneSelectionChanged()));
	connect(mUi->actionAntialiasing, SIGNAL(toggled(bool)), tab, SLOT(toggleAntialiasing(bool)));
	connect(mUi->actionOpenGL_Renderer, SIGNAL(toggled(bool)), tab, SLOT(toggleOpenGL(bool)));
	connect(mModels->graphicalModel(), SIGNAL(rowsAboutToBeMoved(QModelIndex, int, int, QModelIndex, int))
			, tab->mvIface(), SLOT(rowsAboutToBeMoved(QModelIndex, int, int, QModelIndex, int)));
	connect(mModels->graphicalModel(), SIGNAL(rowsMoved(QModelIndex, int, int, QModelIndex, int))
			, tab->mvIface(), SLOT(rowsMoved(QModelIndex, int, int, QModelIndex, int)));

	setShortcuts(tab);
}

void MainWindow::setShortcuts(EditorView * const tab)
{
	EditorViewScene *scene = dynamic_cast<EditorViewScene *>(tab->scene());
	if (scene) {
		// Add shortcut - select all
		QAction *selectAction = new QAction(tab);
		selectAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_A));
		connect(selectAction, SIGNAL(triggered()), scene, SLOT(selectAll()));
		tab->addAction(selectAction);
	}
}

void MainWindow::currentTabChanged(int newIndex)
{
	changeMiniMapSource(newIndex);

	bool const isEditorTab = getCurrentTab() != NULL;

	if (!isEditorTab) {
		mToolManager.activeTabChanged(Id());
	} else if (getCurrentTab()->mvIface() != NULL) {
		Id const currentTabId = getCurrentTab()->mvIface()->rootId();
		mToolManager.activeTabChanged(currentTabId);
	}

	mUi->actionZoom_In->setEnabled(isEditorTab);
	mUi->actionZoom_Out->setEnabled(isEditorTab);

	emit rootDiagramChanged();
}

void MainWindow::updateTabName(Id const &id)
{
	for (int i = 0; i < mUi->tabs->count(); i++) {
		EditorView * const tab = (static_cast<EditorView *>(mUi->tabs->widget(i)));
		if (tab->mvIface()->rootIndex() == mModels->graphicalModelAssistApi().indexById(id)) {
			mUi->tabs->setTabText(i, mModels->graphicalModelAssistApi().name(id));
			return;
		}
	}
}

bool MainWindow::closeTab(QModelIndex const &graphicsIndex)
{
	for (int i = 0; i < mUi->tabs->count(); i++) {
		EditorView * const tab = (static_cast<EditorView *>(mUi->tabs->widget(i)));
		if (tab->mvIface()->rootIndex() == graphicsIndex) {
			closeTab(i);
			return true;
		}
	}
	return false;
}

ListenerManager *MainWindow::listenerManager()
{
	return mListenerManager;
}

void MainWindow::showGrid(bool isChecked)
{
	SettingsManager::setValue("ShowGrid", isChecked);
	setShowGrid(isChecked);
}

void MainWindow::showAlignment(bool isChecked)
{
	SettingsManager::setValue("ShowAlignment", isChecked);
	setShowAlignment(isChecked);
}

void MainWindow::switchGrid(bool isChecked)
{
	SettingsManager::setValue("ActivateGrid", isChecked);
	setSwitchGrid(isChecked);
}

void MainWindow::switchAlignment(bool isChecked)
{
	SettingsManager::setValue("ActivateAlignment", isChecked);
	setSwitchAlignment(isChecked);
}

void MainWindow::setShowGrid(bool isChecked)
{
	for (int i = 0; i < mUi->tabs->count(); i++) {
		EditorView *const tab = (dynamic_cast<EditorView *>(mUi->tabs->widget(i)));
		if (tab != NULL)
			tab->setDrawSceneGrid(isChecked);
	}
}

void MainWindow::setShowAlignment(bool isChecked)
{
	for (int i = 0; i < mUi->tabs->count(); i++) {
		EditorView * const tab = (dynamic_cast<EditorView *>(mUi->tabs->widget(i)));
		if (tab != NULL) {
			QList<QGraphicsItem *> const list = tab->scene()->items();
			foreach (QGraphicsItem * const item, list) {
				NodeElement * const nodeItem = dynamic_cast<NodeElement*>(item);
				if (nodeItem != NULL)
					nodeItem->showAlignment(isChecked);
			}
		}
	}
}

void MainWindow::setSwitchGrid(bool isChecked)
{
	for (int i = 0; i < mUi->tabs->count(); i++) {
		EditorView  const *tab = (dynamic_cast<EditorView *>(mUi->tabs->widget(i)));
		if (tab != NULL) {
			QList<QGraphicsItem *> const list = tab->scene()->items();
			foreach (QGraphicsItem *const item, list) {
				NodeElement * const nodeItem = dynamic_cast<NodeElement*>(item);
				if (nodeItem != NULL)
					nodeItem->switchGrid(isChecked);
			}
		}
	}
}

void MainWindow::setSwitchAlignment(bool isChecked)
{
	for (int i = 0; i < mUi->tabs->count(); i++) {
		EditorView *const tab = (dynamic_cast<EditorView *>(mUi->tabs->widget(i)));
		if (tab != NULL) {
			QList<QGraphicsItem *> const list = tab->scene()->items();
			foreach (QGraphicsItem * const item, list) {
				NodeElement * const nodeItem = dynamic_cast<NodeElement*>(item);
				if (nodeItem != NULL)
					nodeItem->switchAlignment(isChecked);
			}
		}
	}
}

void MainWindow::showGestures()
{
	mGesturesWidget = new GesturesWidget(); // why create another one here?
	mUi->tabs->addTab(mGesturesWidget, tr("Gestures Show"));
	mUi->tabs->setCurrentWidget(mGesturesWidget);
	connect(mGesturesWidget, SIGNAL(currentElementChanged()), this, SIGNAL(currentIdealGestureChanged()));
	emit gesturesShowed();
}

GesturesPainterInterface * MainWindow::gesturesPainter()
{
	return mGesturesWidget;
}

void MainWindow::suggestToCreateDiagram()
{
	if (mModels->logicalModel()->rowCount() > 0)
		return;

	QDialog dialog;
	QVBoxLayout vLayout;
	QHBoxLayout hLayout;
	dialog.setLayout(&vLayout);
	dialog.setMinimumSize(320, 240);
	dialog.setMaximumSize(320, 240);
	dialog.setWindowTitle(tr("Choose new diagram"));

	QLabel label(tr("There is no existing diagram,\n choose diagram you want work with:"));
	QListWidget diagramsListWidget;
	diagramsListWidget.setParent(&dialog);

	int i = 0;
	foreach(Id editor, manager()->editors()) {
		foreach(Id diagram, manager()->diagrams(Id::loadFromString("qrm:/" + editor.editor()))) {
			QString const diagramName = mEditorManager.editorInterface(editor.editor())->diagramName(diagram.diagram());
			QString const diagramNodeName = mEditorManager.editorInterface(editor.editor())->diagramNodeName(diagram.diagram());
			if (diagramNodeName.isEmpty()) {
				continue;
			}
			mDiagramsList.append("qrm:/" + editor.editor() + "/" + diagram.diagram() + "/" + diagramNodeName);
			diagramsListWidget.addItem(diagramName);
			i++;
		}
	}

	QPushButton cancelButton;
	cancelButton.setText(tr("Cancel"));
	QPushButton okButton;
	okButton.setText(tr("Done"));

	QObject::connect(&diagramsListWidget, SIGNAL(currentRowChanged(int)), this, SLOT(diagramInCreateListSelected(int)));
	QObject::connect(&diagramsListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(setDiagramCreateFlag()));
	QObject::connect(&diagramsListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), &dialog, SLOT(close()));
	QObject::connect(&dialog, SIGNAL(destroyed()), this, SLOT(diagramInCreateListDeselect()));

	QObject::connect(&cancelButton, SIGNAL(clicked()), &dialog, SLOT(close()));

	QObject::connect(&okButton, SIGNAL(clicked()), this, SLOT(setDiagramCreateFlag()));
	QObject::connect(&okButton, SIGNAL(clicked()), &dialog, SLOT(close()));

	diagramsListWidget.setCurrentRow(0);
	mDiagramCreateFlag = false;

	vLayout.addWidget(&label);
	vLayout.addWidget(&diagramsListWidget);

	hLayout.addWidget(&okButton);
	hLayout.addWidget(&cancelButton);

	vLayout.addLayout(&hLayout);

	dialog.exec();
}

void MainWindow::setDiagramCreateFlag()
{
	mDiagramCreateFlag = true;
}

void MainWindow::diagramInCreateListDeselect()
{
	if (!mDiagramCreateFlag) {
		deleteFromExplorer(true);
	}
}

void MainWindow::diagramInCreateListSelected(int num)
{
	deleteFromExplorer(false);
	deleteFromExplorer(true);
	createDiagram(mDiagramsList.at(num));
}

void MainWindow::createDiagram(QString const &idString)
{
	Id const created = mModels->graphicalModelAssistApi().createElement(Id::rootId(), Id::loadFromString(idString));
	QModelIndex const index = mModels->graphicalModelAssistApi().indexById(created);
	mUi->graphicalModelExplorer->setCurrentIndex(index);
	Id const logicalIdCreated = mModels->graphicalModelAssistApi().logicalId(created);
	QModelIndex const logicalIndex = mModels->logicalModelAssistApi().indexById(logicalIdCreated);
	mUi->logicalModelExplorer->setCurrentIndex(logicalIndex);
	openNewTab(index);
}

void MainWindow::saveAll()
{
	if (mSaveFile.isEmpty() || mIsNewProject) {
		saveProjectAs();
		return;
	}
	mModels->repoControlApi().saveAll();
	mUnsavedProjectIndicator = false;
	setWindowTitle(mToolManager.customizer()->windowTitle() + " - " + mSaveFile);
	SettingsManager::setValue("saveFile", mSaveFile);
}

void MainWindow::saveProjectAs()
{
	saveAs(getWorkingFile(tr("Select file to save current model to"), true));
}

void MainWindow::saveAs(QString const &fileName)
{
	if (fileName.isEmpty()) {
		return;
	}
	mSaveFile = fileName;
	mUnsavedProjectIndicator = false;
	mIsNewProject = (mSaveFile == mTempDir);
	mModels->repoControlApi().saveTo(mSaveFile);
	if (!mSaveFile.endsWith(".qrs", Qt::CaseInsensitive))
		mSaveFile += ".qrs";
	setWindowTitle(mToolManager.customizer()->windowTitle() + " - " + mSaveFile);
	SettingsManager::setValue("saveFile", mSaveFile);
}

int MainWindow::getTabIndex(const QModelIndex &index)
{
	for (int i = 0; i < mUi->tabs->count(); ++i) {
		EditorView * const editor = dynamic_cast<EditorView *>(mUi->tabs->widget(i));
		if (!editor)
			continue;
		if (index.parent() == editor->mvIface()->rootIndex())
			return i;
	}
	return -1;
}

void MainWindow::setIndexesOfPropertyEditor(Id const &id)
{
	if (mModels->graphicalModelAssistApi().isGraphicalId(id)) {
		Id const logicalId = mModels->graphicalModelAssistApi().logicalId(id);
		QModelIndex const logicalIndex = mModels->logicalModelAssistApi().indexById(logicalId);
		QModelIndex const graphicalIndex = mModels->graphicalModelAssistApi().indexById(id);
		mPropertyModel.setModelIndexes(logicalIndex, graphicalIndex);
	} else if (mModels->logicalModelAssistApi().isLogicalId(id)) {
		QModelIndex const logicalIndex = mModels->logicalModelAssistApi().indexById(id);
		mPropertyModel.setModelIndexes(logicalIndex, QModelIndex());
	} else {
		mPropertyModel.clearModelIndexes();
	}
}

QAction *MainWindow::actionDeleteFromDiagram() const
{
	return mUi->actionDeleteFromDiagram;
}

void qReal::MainWindow::on_actionNew_Diagram_triggered()
{
	if (getCurrentTab() == NULL || getCurrentTab()->mvIface() == NULL)
		return;

	Id const diagram = getCurrentTab()->mvIface()->rootId();  // Or some other way to find current diagram. For example, by current tab in palette.
	createDiagram(diagram.type().toString());
}

void MainWindow::highlight(Id const &graphicalId, bool exclusive)
{
	EditorView* const view = getCurrentTab();
	if (!view)
		return;

	EditorViewScene* const scene = dynamic_cast<EditorViewScene*>(view->scene());
	Element* const element = scene->getElem(graphicalId);
	scene->highlight(graphicalId, exclusive);
	view->ensureElementVisible(element);
}

void MainWindow::dehighlight(Id const &graphicalId)
{
	EditorView* const view = getCurrentTab();
	if (!view)
		return;

	EditorViewScene* const scene = dynamic_cast<EditorViewScene*>(view->scene());
	if (graphicalId == Id())
		scene->dehighlight();
	else
		scene->dehighlight(graphicalId);
}

void MainWindow::dehighlight()
{
	dehighlight(Id());
}

ErrorReporterInterface *MainWindow::errorReporter()
{
	return mErrorReporter;
}

void MainWindow::updatePaletteIcons()
{
	mUi->graphicalModelExplorer->viewport()->update();
	mUi->logicalModelExplorer->viewport()->update();

	Id const currentId = mUi->paletteTree->currentEditor();
	mUi->paletteTree->recreateTrees();

	loadPlugins();

	mUi->paletteTree->setActiveEditor(currentId);
	mUi->paletteTree->setComboBox(currentId);
}

void MainWindow::applySettings()
{
	getCurrentTab()->invalidateScene();
	mErrorReporter->updateVisibility(SettingsManager::value("warningWindow", true).toBool());
}

void MainWindow::hideDockWidget(QDockWidget *dockWidget, const QString &name)
{
	mDocksVisibility[name] = !dockWidget->isHidden();
	if (mDocksVisibility[name]) {
		dockWidget->hide();
	}
}

void MainWindow::showDockWidget(QDockWidget *dockWidget, QString const &name)
{
	if (mDocksVisibility[name]) {
		dockWidget->show();
	}
}

void MainWindow::fullscreen()
{
	mIsFullscreen = !mIsFullscreen;

	if (mIsFullscreen) {
		hideDockWidget(mUi->minimapDock, "minimap");
		hideDockWidget(mUi->graphicalModelDock, "graphicalModel");
		hideDockWidget(mUi->logicalModelDock, "logicalModel");
		hideDockWidget(mUi->propertyDock, "propertyEditor");
		hideDockWidget(mUi->errorDock, "errorReporter");
	} else {
		showDockWidget(mUi->minimapDock, "minimap");
		showDockWidget(mUi->graphicalModelDock, "graphicalModel");
		showDockWidget(mUi->logicalModelDock, "logicalModel");
		showDockWidget(mUi->propertyDock, "propertyEditor");
		showDockWidget(mUi->errorDock, "errorReporter");
	}
}

void MainWindow::createProject()
{
	if (mUnsavedProjectIndicator) {
		switch (openSaveOfferDialog()) {
		case QMessageBox::AcceptRole:
			saveAll();
			break;
		case QMessageBox::RejectRole:
			return;
		}
	}
	open("");
	if (SettingsManager::value("diagramCreateSuggestion", true).toBool())
		suggestToCreateDiagram();

}

QString MainWindow::getNextDirName(QString const &name)
{
	QStringList parts = name.split("_");
	bool isInt = false;
	int version = parts.last().toInt(&isInt);

	if (parts.size() < 2 || !isInt)
		return name + "_2";

	parts.last() = QString::number(++version);
	return parts.join("_");
}

Id MainWindow::activeDiagram()
{
	return getCurrentTab() && getCurrentTab()->mvIface() ? getCurrentTab()->mvIface()->rootId() : Id();
}

void MainWindow::initToolPlugins()
{
	mToolManager.init(PluginConfigurator(mModels->repoControlApi()
			, mModels->graphicalModelAssistApi()
			, mModels->logicalModelAssistApi()
			, *this
			));

	QList<ActionInfo> const actions = mToolManager.actions();
	foreach (ActionInfo const action, actions) {
		if (action.isAction()) {
			if (action.toolbarName() == "file")
				mUi->fileToolbar->addAction(action.action());
			else if (action.toolbarName() == "interpreters")
				mUi->interpreterToolbar->addAction(action.action());
			else if (action.toolbarName() == "generators")
				mUi->generatorsToolbar->addAction(action.action());
		}
	}

	foreach (ActionInfo const action, actions) {
		if (action.menuName() == "tools") {
			if (action.isAction()) {
				mUi->menuTools->addAction(action.action());
			} else {
				mUi->menuTools->addMenu(action.menu());
			}
		}
	}

	if (mUi->parsersToolbar->actions().isEmpty())
		mUi->parsersToolbar->hide();

	if (mUi->generatorsToolbar->actions().isEmpty())
		mUi->generatorsToolbar->hide();

	if (mUi->interpreterToolbar->actions().isEmpty())
		mUi->interpreterToolbar->hide();

	QList<QPair<QString, PreferencesPage *> > const preferencesPages = mToolManager.preferencesPages();
	typedef QPair<QString, PreferencesPage *> PageDescriptor;
	foreach (PageDescriptor const page, preferencesPages)
		mPreferencesDialog.registerPage(page.first, page.second);
}

void MainWindow::showErrors(gui::ErrorReporter const * const errorReporter)
{
	errorReporter->showErrors(mUi->errorListWidget, mUi->errorDock);
}

bool MainWindow::showConnectionRelatedMenus() const
{
	return mToolManager.customizer()->showConnectionRelatedMenus();
}

void MainWindow::showInTextEditor(QString const &title, QString const &text)
{
	if (dynamic_cast<EditorView *>(getCurrentTab()) != NULL) {
		if (!mCodeTabManager->contains(getCurrentTab())) {
			CodeArea * const area = new CodeArea();
			area->document()->setPlainText(text);

			area->show();

			mCodeTabManager->insert(getCurrentTab(), area);

			mUi->tabs->addTab(area, title);
			mUi->tabs->setCurrentWidget(area);
		}
		else
			mUi->tabs->setCurrentWidget(mCodeTabManager->value(getCurrentTab()));
	}
}

void MainWindow::reinitModels()
{
	closeAllTabs();

	mModels->reinit();

	PropertyEditorModel* pModel = dynamic_cast<PropertyEditorModel*>(mUi->propertyEditor->model());
	pModel->clearModelIndexes();
	mUi->propertyEditor->setRootIndex(QModelIndex());
}

QWidget *MainWindow::windowWidget()
{
	return this;
}

void MainWindow::setAutoSaveParameters()
{
	if (!SettingsManager::value("autoSave", true).toBool()) {
		mAutoSaveTimer.stop();
		return;
	}

	mAutoSaveTimer.setInterval(SettingsManager::value("autoSaveInterval", 60 * 10).toInt() * 1000); // in ms
	mAutoSaveTimer.start();
}

void MainWindow::autosave()
{
	if (mIsNewProject)
		saveAs(mTempDir);
	else
		saveAll();
}

QProgressBar *MainWindow::createProgressBar(QSplashScreen* splash)
{
	QProgressBar *progress = new QProgressBar(splash);
	progress->move(20, 270);
	progress->setFixedWidth(600);
	progress->setFixedHeight(15);
	progress->setRange(0, 100);
	return progress;
}

void MainWindow::initToolManager()
{
	if (mToolManager.customizer()) {
		setWindowTitle(mToolManager.customizer()->windowTitle());
		mUi->logicalModelDock->setVisible(mToolManager.customizer()->showLogicalModelExplorer());
		setWindowIcon(mToolManager.customizer()->applicationIcon());
	}
}

void MainWindow::initMiniMap()
{
	connect(mUi->minimapZoomSlider, SIGNAL(valueChanged(int)), this, SLOT(adjustMinimapZoom(int)));
	mUi->minimapView->setRenderHint(QPainter::Antialiasing, true);
	adjustMinimapZoom(mUi->minimapZoomSlider->value());
}

void MainWindow::initTabs()
{
	mUi->tabs->setTabsClosable(true);
	mUi->tabs->setMovable(true);
	connect(mUi->tabs, SIGNAL(currentChanged(int)), this, SLOT(currentTabChanged(int)));
	connect(mUi->tabs, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
}

void MainWindow::initDocks()
{
	mUi->paletteDock->setWidget(mUi->paletteTree);
	mUi->errorDock->setWidget(mUi->errorListWidget);
	mUi->errorListWidget->init(this);
	mUi->errorDock->setVisible(false);
}

void MainWindow::initGridProperties()
{
	mUi->actionSwitch_on_grid->blockSignals(false);
	mUi->actionSwitch_on_grid->setChecked(SettingsManager::value("ActivateGrid", true).toBool());

	mUi->actionShow_grid->blockSignals(false);
	mUi->actionShow_grid->setChecked(SettingsManager::value("ShowGrid", true).toBool());
}

void MainWindow::initWindowTitle()
{
	QString windowTitle = mToolManager.customizer()->windowTitle();
	if (windowTitle.isEmpty())
		windowTitle = "QReal";

	if (mSaveFile.isEmpty()) {
		setWindowTitle(windowTitle + " - " + "unsaved project");
	}
	else {
		setWindowTitle(windowTitle + " - " + mSaveFile);
	}
}

void MainWindow::initExplorers()
{
	mUi->propertyEditor->init(this, &mModels->logicalModelAssistApi());
	mUi->propertyEditor->setModel(&mPropertyModel);

	mUi->graphicalModelExplorer->addAction(mUi->actionDeleteFromDiagram);
	mUi->graphicalModelExplorer->setModel(mModels->graphicalModel());

	mUi->logicalModelExplorer->addAction(mUi->actionDeleteFromDiagram);
	mUi->logicalModelExplorer->setModel(mModels->logicalModel());

	mPropertyModel.setSourceModels(mModels->logicalModel(), mModels->graphicalModel());

	connect(&mModels->graphicalModelAssistApi(), SIGNAL(nameChanged(Id const &)), this, SLOT(updateTabName(Id const &)));
	connect(mUi->graphicalModelExplorer, SIGNAL(clicked(QModelIndex const &)), this, SLOT(graphicalModelExplorerClicked(QModelIndex)));
	connect(mUi->logicalModelExplorer, SIGNAL(clicked(QModelIndex const &)), this, SLOT(logicalModelExplorerClicked(QModelIndex)));
}

void MainWindow::initRecentProjectsMenu()
{
	mRecentProjectsMenu = new QMenu(tr("Recent projects"), mUi->menu_File);
	mUi->menu_File->insertMenu(mUi->menu_File->actions().at(1), mRecentProjectsMenu);
	connect(mRecentProjectsMenu, SIGNAL(aboutToShow()), this, SLOT(openRecentProjectsMenu()));
}

void MainWindow::saveDiagramAsAPictureToFile(const QString &fileName)
{
	if (fileName.isEmpty())
		return;
	QRectF const sceneRect = getCurrentTab()->scene()->itemsBoundingRect();

	QImage image(sceneRect.size().toSize(), QImage::Format_RGB32);
	QPainter painter(&image);

	QBrush brush(Qt::SolidPattern);
	brush.setColor(Qt::white);
	painter.setBrush(brush);
	painter.setPen(QPen(Qt::white));
	QRectF realBoundingRect = getCurrentTab()->scene()->sceneRect();
	realBoundingRect.moveTo(QPointF());
	painter.drawRect(realBoundingRect);

	getCurrentTab()->scene()->render(&painter);
	image.save(fileName);
}

void MainWindow::saveDiagramAsAPicture()
{
	QString const fileName = QFileDialog::getSaveFileName(this,  tr("Save File"), "", tr("Images (*.png *.jpg)"));
	saveDiagramAsAPictureToFile(fileName);
}

void MainWindow::connectWindowTitle()
{
	connect(mModels->graphicalModel(), SIGNAL(dataChanged(QModelIndex,QModelIndex))
			, this, SLOT(editWindowTitle()));
	connect(mModels->logicalModel(), SIGNAL(dataChanged(QModelIndex,QModelIndex))
			, this, SLOT(editWindowTitle()));
	connect(mModels->graphicalModel(), SIGNAL(rowsInserted ( const QModelIndex &, int, int))
			, this, SLOT(editWindowTitle()));
	connect(mModels->logicalModel(), SIGNAL(rowsInserted ( const QModelIndex &, int, int))
			, this, SLOT(editWindowTitle()));
	connect(mModels->graphicalModel(), SIGNAL(rowsMoved(const QModelIndex, int, int, const QModelIndex &, int ))
			, this, SLOT(editWindowTitle()));
	connect(mModels->logicalModel(), SIGNAL(rowsMoved(const QModelIndex, int, int, const QModelIndex &, int ))
			, this, SLOT(editWindowTitle()));
	connect(mModels->graphicalModel(), SIGNAL(rowsRemoved(const QModelIndex, int, int ))
			, this, SLOT(editWindowTitle()));
	connect(mModels->logicalModel(), SIGNAL(rowsRemoved (const QModelIndex &, int , int ))
			, this, SLOT(editWindowTitle()));
}

void MainWindow::disconnectWindowTitle()
{
	disconnect(mModels->graphicalModel(), SIGNAL(dataChanged(QModelIndex,QModelIndex))
			, this, SLOT(editWindowTitle()));
	disconnect(mModels->logicalModel(), SIGNAL(dataChanged(QModelIndex,QModelIndex))
			, this, SLOT(editWindowTitle()));
	disconnect(mModels->graphicalModel(), SIGNAL(rowsInserted ( const QModelIndex &, int, int))
			, this, SLOT(editWindowTitle()));
	disconnect(mModels->logicalModel(), SIGNAL(rowsInserted ( const QModelIndex &, int, int))
			, this, SLOT(editWindowTitle()));
	disconnect(mModels->graphicalModel(), SIGNAL(rowsMoved(const QModelIndex, int, int, const QModelIndex &, int ))
			, this, SLOT(editWindowTitle()));
	disconnect(mModels->logicalModel(), SIGNAL(rowsMoved(const QModelIndex, int, int, const QModelIndex &, int ))
			, this, SLOT(editWindowTitle()));
	disconnect(mModels->graphicalModel(), SIGNAL(rowsRemoved(const QModelIndex, int, int ))
			, this, SLOT(editWindowTitle()));
	disconnect(mModels->logicalModel(), SIGNAL(rowsRemoved (const QModelIndex &, int , int ))
			, this, SLOT(editWindowTitle()));

	mUnsavedProjectIndicator = false;
}

int MainWindow::openSaveOfferDialog()
{
	QMessageBox offerSave(this);
	offerSave.setWindowTitle(tr("Save"));
	offerSave.addButton(tr("Save"), QMessageBox::AcceptRole);
	offerSave.addButton(tr("Cancel"), QMessageBox::RejectRole);
	offerSave.addButton(tr("Discard"), QMessageBox::DestructiveRole);
	offerSave.setText(tr("Do you want to save current project?"));
	return offerSave.exec();
}

void MainWindow::closeProjectAndSave()
{
	if (mUnsavedProjectIndicator) {
		switch (openSaveOfferDialog()) {
		case QMessageBox::AcceptRole:
			saveAll();
			break;
		case QMessageBox::RejectRole:
			return;
		}
	}
	closeProject();
}

void MainWindow::closeProject()
{
	static_cast<PropertyEditorModel*>(mUi->propertyEditor->model())->clearModelIndexes();
	mUi->graphicalModelExplorer->setModel(NULL);
	mUi->logicalModelExplorer->setModel(NULL);
	if (getCurrentTab())
		static_cast<EditorViewScene*>(getCurrentTab()->scene())->clearScene();
	closeAllTabs();
	setWindowTitle(mToolManager.customizer()->windowTitle());
}

void qReal::MainWindow::arrangeElementsByDotRunner(const QString &algorithm, const QString &absolutePathToDotFiles)
{
	Id diagramId = activeDiagram();
	DotRunner *runner = new DotRunner(diagramId,
		mModels->graphicalModelAssistApi(), mModels->logicalModelAssistApi(),
		mEditorManager, absolutePathToDotFiles);
	runner->run(algorithm);
	reinitModels();
}

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

#include "../generators/editorGenerator/editorGenerator.h"

#include "errorReporter.h"

#include "../editorPluginInterface/editorInterface.h"
#include "shapeEdit/shapeEdit.h"
#include "propertyEditorProxyModel.h"
#include "../dialogs/gesturesShow/gesturesWidget.h"

#include "../models/models.h"
#include "../view/editorView.h"
#include "../umllib/element.h"
#include "../dialogs/pluginDialog.h"
#include "../parsers/xml/xmlParser.h"
#include "../dialogs/checkoutDialog.h"
#include "../generators/xmi/xmiHandler.h"
#include "../generators/java/javaHandler.h"
#include "../parsers/hascol/hascolParser.h"
#include "../pluginManager/listenerManager.h"
#include "../generators/hascol/hascolGenerator.h"
#include "../generators/editorGenerator/editorGenerator.h"
#include "../generators/nxtOSEK/nxtOSEKRobotGenerator.h"
#include "../interpreters/visualDebugger/visualDebugger.h"
#include "../../qrkernel/settingsManager.h"

#include "../../qrkernel/timeMeasurer.h"

#include "../../qrmc/metaCompiler.h"

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
		, mVisualDebugger(NULL)
		, mErrorReporter(NULL)
		, mIsFullscreen(false)
		, mTempDir(qApp->applicationDirPath() + "/" + unsavedDir)
		, mPreferencesDialog(this)
		, mNxtToolsPresent(false)
		, mHelpBrowser(NULL)
		, mIsNewProject(true)
		, mUnsavedProjectIndicator(false)
		, mRecentProjectsLimit(5)
		, mRecentProjectsMapper(new QSignalMapper())
{
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
	if (!dir.cd("temp"))
		QDir().mkdir(mTempDir);

	QFileInfo saveFile(SettingsManager::value("saveFile", mSaveFile).toString());

	if (saveFile.exists())
		mSaveFile = saveFile.absoluteFilePath();


	mModels = new models::Models(saveFile.absoluteFilePath(), mEditorManager);

	mErrorReporter = new gui::ErrorReporter(mUi->errorListWidget, mUi->errorDock);
	mErrorReporter->updateVisibility(SettingsManager::value("warningWindow", true).toBool());

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
	initDebugger();

	mFlashTool = new gui::NxtFlashTool(mErrorReporter);
	connect(mFlashTool, SIGNAL(showErrors(gui::ErrorReporter*const)), this, SLOT(showErrors(gui::ErrorReporter*const)));

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

	mPreferencesDialog.init(mUi->actionShow_grid, mUi->actionShow_alignment, mUi->actionSwitch_on_grid, mUi->actionSwitch_on_alignment);

	checkNxtTools();
	mUi->actionUpload_Program->setVisible(mNxtToolsPresent);
	mUi->actionFlash_Robot->setVisible(mNxtToolsPresent);

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

	connect(mUi->actionCheckout, SIGNAL(triggered()), this, SLOT(doCheckout()));
	connect(mUi->actionCommit, SIGNAL(triggered()), this, SLOT(doCommit()));
	//	connect(mUi->actionExport_to_XMI, SIGNAL(triggered()), this, SLOT(exportToXmi()));
	//	connect(mUi->actionGenerate_to_Java, SIGNAL(triggered()), this, SLOT(generateToJava()));
	//	connect(mUi->actionGenerate_to_Hascol, SIGNAL(triggered()), this, SLOT(generateToHascol()));
	//	connect(mUi->actionShape_Edit, SIGNAL(triggered()), this, SLOT(openShapeEditor()));
	connect(mUi->actionGenerate_Editor, SIGNAL(triggered()), this, SLOT(generateEditor()));
	//	connect(mUi->actionGenerate_Editor_qrmc, SIGNAL(triggered()), this, SLOT(generateEditorWithQRMC()));
	connect(mUi->actionParse_Editor_xml, SIGNAL(triggered()), this, SLOT(parseEditorXml()));
	connect(mUi->actionPreferences, SIGNAL(triggered()), this, SLOT(showPreferencesDialog()));
	connect(mUi->actionFlash_Robot, SIGNAL(triggered()), this, SLOT(flashRobot()));
	connect(mUi->actionUpload_Program, SIGNAL(triggered()), this, SLOT(uploadProgram()));
	connect(mUi->actionCode, SIGNAL(triggered()), this, SLOT(generateRobotSourceCode()));

	//	connect(mUi->actionParse_Hascol_sources, SIGNAL(triggered()), this, SLOT(parseHascol()));
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

	connectDebugActions();
	//	connect(mUi->actionRun, SIGNAL(triggered()), this, SLOT(run()));
	//	connect(mUi->actionStop_Running, SIGNAL(triggered()), this, SLOT(stop()));
	//	connect(mUi->actionStop_Robot, SIGNAL(triggered()), this, SLOT(stopRobot()));

	//	connect(mUi->actionConnect_To_Robot, SIGNAL(triggered()), this, SLOT(connectToRobot()));

	//	connect(mUi->actionRobot_Settings, SIGNAL(triggered()), this, SLOT(showRobotSettingsDialog()));

	//	connect(mUi->actionFullscreen, SIGNAL(triggered()), this, SLOT(fullscreen()));

	//	connect(mUi->actionShow2Dmodel, SIGNAL(triggered()), this, SLOT(showD2ModelWidget()));
}

void MainWindow::connectDebugActions()
{
	//	connect(mUi->actionDebug, SIGNAL(triggered()), this, SLOT(debug()));
	//	connect(mUi->actionDebug_Single_step, SIGNAL(triggered()), this, SLOT(debugSingleStep()));
	//	connect(mUi->actionGenerate_and_build, SIGNAL(triggered()), this, SLOT(generateAndBuild()));
	//	connect(mUi->actionStart_debugger, SIGNAL(triggered()), this, SLOT(startDebugger()));
	//	connect(mUi->actionRun, SIGNAL(triggered()), this, SLOT(runProgramWithDebugger()));
	//	connect(mUi->actionKill, SIGNAL(triggered()), this, SLOT(killProgramWithDebugger()));
	//	connect(mUi->actionClose_all, SIGNAL(triggered()), this, SLOT(closeDebuggerProcessAndThread()));
	//	connect(mUi->actionCont, SIGNAL(triggered()), this, SLOT(goToNextBreakpoint()));
	//	connect(mUi->actionNext, SIGNAL(triggered()), this, SLOT(goToNextInstruction()));
	//	connect(mUi->actionSet_Breakpoints, SIGNAL(triggered()), this, SLOT(placeBreakpointsInDebugger()));
	//	connect(mUi->actionConfigure, SIGNAL(triggered()), this, SLOT(configureDebugger()));
	//	connect(mUi->actionBreak_main, SIGNAL(triggered()), this, SLOT(setBreakpointAtStart()));
	//	connect(mUi->actionStart_debugging, SIGNAL(triggered()), this, SLOT(startDebugging()));
	//	connect(mUi->tabs, SIGNAL(currentChanged(int)), this, SLOT(checkEditorForDebug(int)));
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
			mUi->paletteToolbox->addDiagramType(diagram, mEditorManager.friendlyName(diagram));
			mUi->paletteToolbox->addSortedItemTypes(mEditorManager, diagram);
		}
	}
	mUi->paletteToolbox->initDone();
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
		close();
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
	if (!QFile(fileName).exists()) // || (!mSaveFile.isEmpty() && fileName.isEmpty()))
		if (!(!mSaveFile.isEmpty() && fileName.isEmpty()))
			return false;

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
	QModelIndex index = isLogicalModel ? (mUi->logicalModelExplorer->currentIndex())
			: (mUi->graphicalModelExplorer->currentIndex());
	QList<NodeElement*> itemsToArrangeLinks;
	EditorView const * const view = getCurrentTab();
	EditorViewScene* scene = NULL;
	if (view) {
		scene = dynamic_cast<EditorViewScene*>(view->scene());
	}

	if (isLogicalModel) {
		Id const logicalId = mModels->logicalModelAssistApi().idByIndex(index);
		IdList const graphicalIdList = mModels->graphicalModelAssistApi().graphicalIdsByLogicalId(logicalId);
		foreach (Id graphicalId, graphicalIdList) {
			closeTab(mModels->graphicalModelAssistApi().indexById(graphicalId));
			if (scene) {
				QGraphicsItem const * const item = scene->getElem(graphicalId);
				EdgeElement const * const edge = dynamic_cast<EdgeElement const *>(item);
				if (edge) {
					itemsToArrangeLinks.append(edge->src());
					itemsToArrangeLinks.append(edge->dst());
				}
			}
		}
	} else {
		closeTab(index);
		Id const graphicalId = mModels->graphicalModelAssistApi().idByIndex(index);
		if (scene) {
			QGraphicsItem const * const item = scene->getElem(graphicalId);
			EdgeElement const * const edge = dynamic_cast<EdgeElement const *>(item);
			if (edge) {
				itemsToArrangeLinks.append(edge->src());
				itemsToArrangeLinks.append(edge->dst());
			}
		}
	}

	if (index.isValid()) {  // TODO: why only here?
		PropertyEditorModel* propertyEditorModel = static_cast<PropertyEditorModel*>(mUi->propertyEditor->model());
		if (propertyEditorModel->isCurrentIndex(index)) {
			propertyEditorModel->clearModelIndexes();
			mUi->propertyEditor->setRootIndex(QModelIndex());
		}

		if (isLogicalModel) {
			mModels->logicalModel()->removeRow(index.row(), index.parent());
		}
		else {
			mModels->graphicalModel()->removeRow(index.row(), index.parent());
		}
		foreach (NodeElement *item, itemsToArrangeLinks) {
			if (item) {
				item->arrangeLinks();
			}
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
			tr("Contacts:<br><br>"
			"se.math.spbu.ru/SE/qreal"));
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

void MainWindow::doCheckout()
{
//	QString path;
//	QString url;
//	CheckoutDialog *dialog = new CheckoutDialog(this);
//	connect(dialog, SIGNAL(accepted()), this, SLOT(checkoutDialogOk()));
//	connect(dialog, SIGNAL(rejected()), this, SLOT(checkoutDialogCancel()));
//	dialog->show();
//	if (dialog->Accepted)
//	{
//		path = dialog->getDir();
//		url = dialog->getUrl();
//	}
}

void MainWindow::doCommit()
{
//	QString select = tr("Select directory to commit");
//	QString path = QFileDialog::getExistingDirectory(this, select);

//	if (path.isEmpty())
//		return;
	/*	char* p;
 QByteArray p1 = path.toAscii();
 p = p1.data();
 SvnClient client(p, "", "");
//	client.commit(path, )
 QString *messag = new QString;
 int revision = client.commit(*messag);
 if (revision > 0)
 {
  QString success = tr("Committed successfully to revision ");
  QMessageBox::information(this, tr("Success"), success.append(QString(revision)));
 }
 else
  QMessageBox::information(this, tr("Error"), *messag);*/
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
	if (dirName.isEmpty())
		return;

	QString const errors = java.parseJavaLibraries(dirName);

	if (!errors.isEmpty()) {
		QMessageBox::warning(this, tr("errors"), "Some errors occured. Export may be incorrect. Errors list: \n" + errors);
	} else {
		QMessageBox::information(this, tr("finished"), "Parsing is finished");
	}
}

void MainWindow::generateToHascol()
{
	generators::HascolGenerator hascolGenerator(mModels->logicalRepoApi());

	gui::ErrorReporter& errors = hascolGenerator.generate();
	errors.showErrors(mUi->errorListWidget, mUi->errorDock);
}

void MainWindow::generateEditor()
{
	generators::EditorGenerator editorGenerator(mModels->logicalRepoApi());

	QDir dir(".");

	QHash<Id, QPair<QString, QString> > metamodelList = editorGenerator.getMetamodelList();
	foreach (Id const key, metamodelList.keys()) {
		QString const metamodelFullName = metamodelList[key].first;
		QString const pathToQRealRoot = metamodelList[key].second;
		dir.mkpath(metamodelFullName);
		QFileInfo metamodelFileInfo(metamodelFullName);
		QString metamodelName = metamodelFileInfo.baseName();
		gui::ErrorReporter& errors = editorGenerator.generateEditor(key, metamodelFullName + "/" + metamodelName, pathToQRealRoot);

		if (errors.showErrors(mUi->errorListWidget, mUi->errorDock)) {
			if (QMessageBox::question(this
					, tr("loading.."), QString(tr("Do you want to load generated editor %1?")).arg(metamodelName),
					QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
			{
				return;
			}
			loadNewEditor(metamodelFullName, metamodelName
					, SettingsManager::value("pathToQmake", "").toString()
					, SettingsManager::value("pathToMake", "").toString()
					, SettingsManager::value("pluginExtension", "").toString()
					, SettingsManager::value("prefix", "").toString());
		}
	}
}

/*
void MainWindow::generateEditorWithQRMC()
{
 qrmc::MetaCompiler metaCompiler(qApp->applicationDirPath() + "/../qrmc", mSaveDir);

 IdList const metamodels = mModels->logicalRepoApi().children(Id::rootId());

 QProgressBar *progress = new QProgressBar(this);
 progress->show();
 int const progressBarWidth = 240;
 int const progressBarHeight = 20;

 QApplication::processEvents();
 QRect screenRect = qApp->desktop()->availableGeometry();
 progress->move(screenRect.width() / 2 - progressBarWidth / 2, screenRect.height() / 2 - progressBarHeight / 2);
 progress->setFixedWidth(progressBarWidth);
 progress->setFixedHeight(progressBarHeight);
 progress->setRange(0, 100);

 int forEditor = 60 / metamodels.size();

 foreach (Id const key, metamodels) {
  QString const objectType = mModels->logicalRepoApi().typeName(key);
  if (objectType == "MetamodelDiagram") {
   QString name = mModels->logicalRepoApi().stringProperty(key, "name of the directory");
   if (QMessageBox::question(this, tr("loading.."), QString(tr("Do you want to compile and load editor %1?")).arg(name),
  QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
   {
 continue;
   }

   progress->setValue(5);

   if (!metaCompiler.compile(name)) { // generating source code for all metamodels
 QMessageBox::warning(this, tr("error"), tr("Cannot generate source code for editor ") + name);
 qDebug() << "compilation failed";
 continue;
   }
   progress->setValue(20);

   QProcess builder;
   builder.setWorkingDirectory("../qrmc/plugins");
   builder.start(SettingsManager::value("pathToQmake", "").toString());
   qDebug()  << "qmake";
   if ((builder.waitForFinished()) && (builder.exitCode() == 0)) {
 progress->setValue(40);

 builder.start(SettingsManager::value("pathToMake", "").toString());

 bool finished = builder.waitForFinished(100000);
 qDebug()  << "make";
 if (finished && (builder.exitCode() == 0)) {
  qDebug()  << "make ok";

  progress->setValue(progress->value() + forEditor/2);

  QString normalizedName = name.at(0).toUpper() + name.mid(1);
  if (!name.isEmpty()) {
   if (mEditorManager.editors().contains(Id(normalizedName))) {
 foreach (Id const diagram, mEditorManager.diagrams(Id(normalizedName)))
  mUi->paletteToolbox->deleteDiagramType(diagram);

 if (!mEditorManager.unloadPlugin(normalizedName)) {
  QMessageBox::warning(this, tr("error"), tr("cannot unload plugin ") + normalizedName);
  progress->close();
  delete progress;
  continue;
 }
   }

   if (mEditorManager.loadPlugin(SettingsManager::value("prefix", "").toString() + name + "." + SettingsManager::value("pluginExtension", "").toString())) {
 foreach (Id const diagram, mEditorManager.diagrams(Id(normalizedName))) {
  mUi->paletteToolbox->addDiagramType(diagram, mEditorManager.friendlyName(diagram));
  mUi->paletteToolbox->addSortedItemTypes(mEditorManager, diagram);
   }
  }
  progress->setValue(progress->value() + forEditor/2);
 }
   }
   mUi->paletteToolbox->initDone();
   progress->setValue(100);

  }
 }
 if (progress->value() != 100)
  QMessageBox::warning(this, tr("error"), tr("cannot load new editor"));
 progress->setValue(100);
 progress->close();
 delete progress;
}
*/


void MainWindow::loadNewEditor(const QString &directoryName
		, const QString &metamodelName
		, const QString &commandFirst
		, const QString &commandSecond
		, const QString &extension
		, const QString &prefix)
{
	int const progressBarWidth = 240;
	int const progressBarHeight = 20;

	if ((commandFirst == "") || (commandSecond == "") || (extension == "")) {
		QMessageBox::warning(this, tr("error"), tr("please, fill compiler settings"));
		return;
	}

	QString const normalizeDirName = metamodelName.at(0).toUpper() + metamodelName.mid(1);

	QProgressBar * const progress = new QProgressBar(this);
	progress->show();

	QApplication::processEvents();

	QRect const screenRect = qApp->desktop()->availableGeometry();
	progress->move(screenRect.width() / 2 - progressBarWidth / 2, screenRect.height() / 2 - progressBarHeight / 2);
	progress->setFixedWidth(progressBarWidth);
	progress->setFixedHeight(progressBarHeight);
	progress->setRange(0, 100);
	progress->setValue(5);

	if (mEditorManager.editors().contains(Id(normalizeDirName))) {
		IdList const diagrams = mEditorManager.diagrams(Id(normalizeDirName));

		if (!mEditorManager.unloadPlugin(normalizeDirName)) {
			QMessageBox::warning(this, tr("error"), tr("cannot unload plugin"));
			progress->close();
			delete progress;
			return;
		}
		foreach (Id const diagram, diagrams)
			mUi->paletteToolbox->deleteDiagramType(diagram);
	}
	progress->setValue(20);

	QProcess builder;
	builder.setWorkingDirectory(directoryName);
	builder.start(commandFirst);

	if ((builder.waitForFinished()) && (builder.exitCode() == 0)) {
		progress->setValue(60);
		builder.start(commandSecond);
		if (builder.waitForFinished() && (builder.exitCode() == 0)) {
			progress->setValue(80);
			if (mEditorManager.loadPlugin(prefix + metamodelName + "." + extension)) {
				foreach (Id const diagram, mEditorManager.diagrams(Id(normalizeDirName))) {
					mUi->paletteToolbox->addDiagramType(diagram, mEditorManager.friendlyName(diagram));
					mUi->paletteToolbox->addSortedItemTypes(mEditorManager, diagram);
				}
				mUi->paletteToolbox->initDone();
				progress->setValue(100);
			}
		}
	}

	if (progress->value() != 100)
		QMessageBox::warning(this, tr("error"), tr("cannot load new editor"));
	progress->setValue(100);
	progress->close();
	delete progress;
}

void MainWindow::parseEditorXml()
{
	if (!mEditorManager.editors().contains(Id("MetaEditor"))) {
		QMessageBox::warning(this, tr("error"), tr("required plugin is not loaded"));
		return;
	}
	QDir dir(".");
	QString directoryName = ".";
	while (dir.cdUp()) {
		QFileInfoList infoList = dir.entryInfoList(QDir::Dirs);
		foreach (QFileInfo const directory, infoList){
			if (directory.baseName() == "qrxml") {
				directoryName = directory.absolutePath() + "/qrxml";
			}
		}
	}
	QString const fileName = QFileDialog::getOpenFileName(this, tr("Select xml file to parse"), directoryName, "XML files (*.xml)");
	if (fileName == "")
		return;

	parsers::XmlParser parser(mModels->mutableLogicalRepoApi(), mEditorManager);

	parser.parseFile(fileName);

	parser.loadIncludeList(fileName);

	mModels->reinit();
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
	emit rootDiagramChanged();
}

void qReal::MainWindow::closeTab(int index)
{
	QWidget *widget = mUi->tabs->widget(index);
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

void MainWindow::parseHascol()
{
	QStringList const fileNames = QFileDialog::getOpenFileNames(this, tr("Select Hascol files to parse"), ".", "*.md;;*.*");
	if (fileNames.empty())
		return;

	parsers::HascolParser parser(mModels->mutableLogicalRepoApi(), mEditorManager);
	gui::ErrorReporter& errors = parser.parse(fileNames);

	errors.showErrors(mUi->errorListWidget, mUi->errorDock);

	mModels->reinit();
}

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

void MainWindow::openNewTab(const QModelIndex &arg)
{
	QModelIndex index = arg;
	while (index.parent() != QModelIndex())
		index = index.parent();

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
		mUi->tabs->addTab(view, index.data().toString());
		mUi->tabs->setCurrentWidget(view);
		initCurrentTab(index);
	}

	// changing of palette active editor
	if (SettingsManager::value("PaletteTabSwitching", true).toBool()) {
		int i = 0;
		foreach(QString name, mUi->paletteToolbox->getTabNames()) {
			Id const id = mModels->graphicalModelAssistApi().idByIndex(index);
			Id const diagramId = Id(id.editor(), id.diagram());
			QString const diagramName = mEditorManager.friendlyName(diagramId);
			if (diagramName == name) {
				mUi->paletteToolbox->getComboBox()->setCurrentIndex(i);
				break;
			}
			i++;
		}
	}
}

void MainWindow::initCurrentTab(QModelIndex const &rootIndex)
{
	getCurrentTab()->setMainWindow(this);
	QModelIndex const index = rootIndex;

	getCurrentTab()->mvIface()->setAssistApi(mModels->graphicalModelAssistApi(), mModels->logicalModelAssistApi());

	getCurrentTab()->mvIface()->setModel(mModels->graphicalModel());
	getCurrentTab()->mvIface()->setLogicalModel(mModels->logicalModel());
	getCurrentTab()->mvIface()->setRootIndex(index);
	changeMiniMapSource(mUi->tabs->currentIndex());

	/*connect after setModel etc. because of signal selectionChanged was sent when there were old indexes*/
	connect(getCurrentTab()->scene(), SIGNAL(selectionChanged()), SLOT(sceneSelectionChanged()));
	connect(mUi->actionAntialiasing, SIGNAL(toggled(bool)), getCurrentTab(), SLOT(toggleAntialiasing(bool)));
	connect(mUi->actionOpenGL_Renderer, SIGNAL(toggled(bool)), getCurrentTab(), SLOT(toggleOpenGL(bool)));
	connect(mModels->graphicalModel(), SIGNAL(rowsAboutToBeMoved(QModelIndex, int, int, QModelIndex, int))
			, getCurrentTab()->mvIface(), SLOT(rowsAboutToBeMoved(QModelIndex, int, int, QModelIndex, int)));
	connect(mModels->graphicalModel(), SIGNAL(rowsMoved(QModelIndex, int, int, QModelIndex, int))
			, getCurrentTab()->mvIface(), SLOT(rowsMoved(QModelIndex, int, int, QModelIndex, int)));
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

void MainWindow::closeTab(QModelIndex const &graphicsIndex)
{
	for (int i = 0; i < mUi->tabs->count(); i++) {
		EditorView * const tab = (static_cast<EditorView *>(mUi->tabs->widget(i)));
		if (tab->mvIface()->rootIndex() == graphicsIndex) {
			closeTab(i);
			return;
		}
	}
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
	mGesturesWidget = new GesturesWidget();
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
			if (diagramNodeName.isEmpty())
				continue;
			mDiagramsList.append("qrm:/" + editor.editor() + "/" + diagram.diagram() + "/" + diagramNodeName);
			diagramsListWidget.addItem(diagramName);
			i++;
		}
	}

	QPushButton cancelButton;
	cancelButton.setText(tr("Cancel"));
	QPushButton okButton;
	okButton.setText(tr("Done"));

	QObject::connect(&diagramsListWidget,SIGNAL(currentRowChanged(int)),this,SLOT(diagramInCreateListSelected(int)));
	QObject::connect(&diagramsListWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(setDiagramCreateFlag()));
	QObject::connect(&diagramsListWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),&dialog,SLOT(close()));
	QObject::connect(&dialog,SIGNAL(destroyed()),this,SLOT(diagramInCreateListDeselect()));

	QObject::connect(&cancelButton,SIGNAL(clicked()),&dialog,SLOT(close()));

	QObject::connect(&okButton,SIGNAL(clicked()),this,SLOT(setDiagramCreateFlag()));
	QObject::connect(&okButton,SIGNAL(clicked()),&dialog,SLOT(close()));

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
	if (!mDiagramCreateFlag)
		deleteFromExplorer(true);
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
	if (fileName.isEmpty())
		return;
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

void MainWindow::debug()
{
	EditorView * const editor = dynamic_cast<EditorView *>(mUi->tabs->widget(mUi->tabs->currentIndex()));
	mVisualDebugger->setEditor(editor);
	if (mVisualDebugger->canDebug(VisualDebugger::fullDebug)) {
		mVisualDebugger->debug();
		mErrorReporter->showErrors(mUi->errorListWidget, mUi->errorDock);
		mErrorReporter->clearErrors();
	}
}

void MainWindow::debugSingleStep()
{
	EditorView * const editor = dynamic_cast<EditorView *>(mUi->tabs->widget(mUi->tabs->currentIndex()));
	mVisualDebugger->setEditor(editor);
	if (mVisualDebugger->canDebug(VisualDebugger::singleStepDebug)) {
		mVisualDebugger->debugSingleStep();
		mErrorReporter->showErrors(mUi->errorListWidget, mUi->errorDock);
		mErrorReporter->clearErrors();
	}
}

void MainWindow::generateAndBuild()
{
	EditorView * const editor = dynamic_cast<EditorView *>(mUi->tabs->widget(mUi->tabs->currentIndex()));
	mVisualDebugger->setEditor(editor);

	if (mVisualDebugger->canDebug(VisualDebugger::debugWithDebugger)) {
		mVisualDebugger->generateCode();

		if (mVisualDebugger->canBuild()) {
			mDebuggerConnector->run();

			mDebuggerConnector->build(SettingsManager::value("debugWorkingDirectory", "").toString() + "/" +
					SettingsManager::value("codeFileName", "code.c").toString());

			if (!mDebuggerConnector->hasBuildError()) {
				mErrorReporter->addInformation("Code generated and builded successfully");
				mErrorReporter->showErrors(mUi->errorListWidget, mUi->errorDock);
			}
		} else {
			mErrorReporter->showErrors(mUi->errorListWidget, mUi->errorDock);
		}
		mErrorReporter->clearErrors();
	}
}

void MainWindow::startDebugger()
{
	if (mVisualDebugger->canDebug(VisualDebugger::debugWithDebugger)
		&& !mDebuggerConnector->isDebuggerRunning())
	{
		mVisualDebugger->setDebugType(VisualDebugger::debugWithDebugger);
		mDebuggerConnector->run();
		mDebuggerConnector->startDebugger();
	}
}

void MainWindow::configureDebugger()
{
	if (mDebuggerConnector->isDebuggerRunning()) {
		mDebuggerConnector->configure(SettingsManager::value("debugWorkingDirectory", "").toString() + "/" +
				SettingsManager::value("buildedFileName", "builded.exe").toString());
	}
}

void MainWindow::setBreakpointAtStart()
{
	if (mDebuggerConnector->isDebuggerRunning()) {
		EditorView *editor = dynamic_cast<EditorView *>(mUi->tabs->widget(mUi->tabs->currentIndex()));
		mVisualDebugger->setEditor(editor);

		mVisualDebugger->createIdByLineCorrelation();

		mDebuggerConnector->sendCommand("break main\n");
	}
}

void MainWindow::runProgramWithDebugger()
{
	if (mDebuggerConnector->isDebuggerRunning()) {
		mDebuggerConnector->sendCommand("run\n");
	}
}

void MainWindow::killProgramWithDebugger()
{
	if (mDebuggerConnector->isDebuggerRunning()) {
		mDebuggerConnector->sendCommand("kill\n");
	}
}

void MainWindow::placeBreakpointsInDebugger()
{
	if (mDebuggerConnector->isDebuggerRunning()) {
		EditorView * const editor = dynamic_cast<EditorView *>(mUi->tabs->widget(mUi->tabs->currentIndex()));
		mVisualDebugger->setEditor(editor);

		mVisualDebugger->createIdByLineCorrelation();
		if (mVisualDebugger->canComputeBreakpoints()) {
			QList<int>* breakpoints = mVisualDebugger->computeBreakpoints();

			for (int i = 0; i < breakpoints->size(); i++) {
				mDebuggerConnector->sendCommand("break " + QString::number(breakpoints->at(i)) + "\n");
			}

			delete breakpoints;
		} else {
			mErrorReporter->showErrors(mUi->errorListWidget, mUi->errorDock);
			mErrorReporter->clearErrors();
		}
	}
}

void MainWindow::goToNextBreakpoint()
{
	if (mDebuggerConnector->isDebuggerRunning()) {
		mDebuggerConnector->sendCommand("cont\n");
	}
}

void MainWindow::goToNextInstruction()
{
	if (mDebuggerConnector->isDebuggerRunning()) {
		mDebuggerConnector->sendCommand("next\n");
	}
}

void MainWindow::closeDebuggerProcessAndThread()
{
	if (mDebuggerConnector->isDebuggerRunning()) {
		mVisualDebugger->dehighlight();
		mVisualDebugger->setDebugType(VisualDebugger::noDebug);
		mDebuggerConnector->finishProcess();
	}
}

void MainWindow::startDebugging()
{
	if (mVisualDebugger->canDebug(VisualDebugger::debugWithDebugger)) {
		generateAndBuild();
		startDebugger();
		configureDebugger();
		setBreakpointAtStart();
		runProgramWithDebugger();
	}
}

void MainWindow::drawDebuggerStdOutput(QString output)
{
	mErrorReporter->addInformation(output);
	mErrorReporter->showErrors(mUi->errorListWidget, mUi->errorDock);

	if ('1' <= output.at(0) && output.at(0) <= '9') {
		int index = output.indexOf("\t");
		Id idToLigth = mVisualDebugger->getIdByLine(output.mid(0,index).toInt());
		mVisualDebugger->highlight(idToLigth);
	} else {
		QString const fileName = SettingsManager::value("codeFileName", "code.c").toString();

		int index = output.indexOf(fileName + ":");
		if (index > -1) {
			index += (fileName.length() + 1);
			int boundaryIndex = index;
			while ('0' <= output.at(boundaryIndex) && output.at(boundaryIndex) <= '9') {
				boundaryIndex++;
			}
			Id const idToLight = mVisualDebugger->getIdByLine(output.mid(index, boundaryIndex - index).toInt());
			mVisualDebugger->highlight(idToLight);
		}
	}
}

void MainWindow::drawDebuggerErrOutput(QString output)
{
	mVisualDebugger->dehighlight();
	mVisualDebugger->setDebugType(VisualDebugger::noDebug);
	mErrorReporter->addCritical(output);
	mErrorReporter->showErrors(mUi->errorListWidget, mUi->errorDock);
	mErrorReporter->clearErrors();
}

void MainWindow::checkEditorForDebug(int index)
{
	Q_UNUSED(index)

	//	bool enabled = mUi->tabs->count() > 0 &&
	//			mUi->tabs->tabText(mUi->tabs->currentIndex()).compare("(Block Diagram)") == 0;
	// mUi->debuggerToolBar->setVisible(enabled);
	//	mUi->actionDebug->setEnabled(enabled);
	//	mUi->actionDebug_Single_step->setEnabled(enabled);
	//	mUi->actionGenerate_and_build->setEnabled(enabled);
	//	mUi->actionStart_debugger->setEnabled(enabled);
	//	mUi->actionRun->setEnabled(enabled);
	//	mUi->actionKill->setEnabled(enabled);
	//	mUi->actionClose_all->setEnabled(enabled);
	//	mUi->actionNext->setEnabled(enabled);
	//	mUi->actionCont->setEnabled(enabled);
	//	mUi->actionSet_Breakpoints->setEnabled(enabled);
	//	mUi->actionConfigure->setEnabled(enabled);
	//	mUi->actionBreak_main->setEnabled(enabled);
	//	mUi->actionStart_debugging->setEnabled(enabled);
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

	Id const currentId = mUi->paletteToolbox->currentTab();
	mUi->paletteToolbox->recreateTabs();

	loadPlugins();

	mUi->paletteToolbox->setActiveEditor(currentId);
	mUi->paletteToolbox->setComboBox(currentId);
}

void MainWindow::applySettings()
{
	getCurrentTab()->invalidateScene();
	mErrorReporter->updateVisibility(SettingsManager::value("warningWindow", true).toBool());
}

void MainWindow::hideDockWidget(QDockWidget *dockWidget, QString name)
{
	mDocksVisibility[name] = !dockWidget->isHidden();
	if (mDocksVisibility[name])
		dockWidget->hide();
}

void MainWindow::showDockWidget(QDockWidget *dockWidget, QString name)
{
	if (mDocksVisibility[name])
		dockWidget->show();
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
	return getCurrentTab()->mvIface()->rootId();
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
		if (action.toolbarName() == "file")
			mUi->fileToolbar->addAction(action.action());
		else if (action.toolbarName() == "interpreters")
			mUi->interpreterToolBar->addAction(action.action());
	}

	foreach (ActionInfo const action, actions) {
		if (action.menuName() == "tools")

			mUi->menuTools->addAction(action.action());
	}

	if (mUi->parsersToolbar->actions().isEmpty())
		mUi->parsersToolbar->hide();

	if (mUi->generatorsToolbar->actions().isEmpty())
		mUi->generatorsToolbar->hide();

	if (mUi->interpreterToolBar->actions().isEmpty())
		mUi->interpreterToolBar->hide();

	QList<QPair<QString, PreferencesPage *> > const preferencesPages = mToolManager.preferencesPages();
	typedef QPair<QString, PreferencesPage *> PageDescriptor;
	foreach (PageDescriptor const page, preferencesPages)
		mPreferencesDialog.registerPage(page.first, page.second);
}

void MainWindow::generateRobotSourceCode()
{
	saveAll();

	qReal::generators::NxtOSEKRobotGenerator gen(mModels->repoControlApi());
	gui::ErrorReporter &errors = gen.generate();
	if (errors.showErrors(mUi->errorListWidget, mUi->errorDock)){
		mErrorReporter->showErrors(mUi->errorListWidget, mUi->errorDock);

		CodeArea * const area = new CodeArea();
		QFile file("nxt-tools/example0/example0.c");
		QTextStream * inStream = NULL;
		if (!file.isOpen() && file.open(QIODevice::ReadOnly | QIODevice::Text))
			inStream = new QTextStream(&file);

		if (inStream)
			area->document()->setPlainText(inStream->readAll());

		area->show();

		mUi->tabs->addTab(area, "example0");
		mUi->tabs->setCurrentWidget(area);

		mUi->actionUpload_Program->setVisible(mNxtToolsPresent);
		mUi->actionFlash_Robot->setVisible(mNxtToolsPresent);
	}
}

void MainWindow::uploadProgram()
{
	if (!mNxtToolsPresent) {
		mErrorReporter->addError("upload.sh not found. Make sure it is present in QReal installation directory");
		mErrorReporter->showErrors(mUi->errorListWidget, mUi->errorDock);
		return;
	}
	generateRobotSourceCode();
	mFlashTool->uploadProgram();
}

void MainWindow::flashRobot()
{
	if (!mNxtToolsPresent) {
		mErrorReporter->addError("flash.sh not found. Make sure it is present in QReal installation directory");
		mErrorReporter->showErrors(mUi->errorListWidget, mUi->errorDock);
		return;
	}
	mFlashTool->flashRobot();
}

void MainWindow::showErrors(gui::ErrorReporter const * const errorReporter)
{
	errorReporter->showErrors(mUi->errorListWidget, mUi->errorDock);
}

bool MainWindow::showConnectionRelatedMenus() const
{
	return mToolManager.customizer()->showConnectionRelatedMenus();
}

void MainWindow::checkNxtTools()
{
	QDir dir(qApp->applicationDirPath());
	if (!QDir().exists(dir.absolutePath() + "/nxt-tools")) {
		mNxtToolsPresent = false;
		return;
	}
	dir.cd(dir.absolutePath() + "/nxt-tools");

	QDir gnuarm(dir.absolutePath() + "/gnuarm");
	QDir nexttool(dir.absolutePath() + "/nexttool");
	QDir nxtOSEK(dir.absolutePath() + "/nxtOSEK");

#ifdef Q_OS_WIN
	QFile flash(dir.absolutePath() + "/flash.bat");
	QFile upload1(dir.absolutePath() + "/upload.bat");
	QFile upload2(dir.absolutePath() + "/upload.sh");

	mNxtToolsPresent = gnuarm.exists() && nexttool.exists() && nxtOSEK.exists() && flash.exists() && upload1.exists() && upload2.exists();
#else
	QDir libnxt(dir.absolutePath() + "/libnxt");
	QFile flash(dir.absolutePath() + "/flash.sh");
	QFile upload(dir.absolutePath() + "/upload.sh");

	mNxtToolsPresent = gnuarm.exists() && libnxt.exists() && nexttool.exists() && nxtOSEK.exists() && flash.exists() && upload.exists();
#endif
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
	connect(mUi->tabs, SIGNAL(currentChanged(int)), this, SLOT(changeMiniMapSource(int)));
	connect(mUi->tabs, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
}

void MainWindow::initDocks()
{
	mUi->paletteDock->setWidget(mUi->paletteToolbox);
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

void MainWindow::initDebugger()
{
	mVisualDebugger = new VisualDebugger(mModels->logicalModelAssistApi(), mModels->graphicalModelAssistApi(), *this);
	mDebuggerConnector = new DebuggerConnector();

	connect(mDebuggerConnector, SIGNAL(readyReadStdOutput(QString)), this, SLOT(drawDebuggerStdOutput(QString)));
	connect(mDebuggerConnector, SIGNAL(readyReadErrOutput(QString)), this, SLOT(drawDebuggerErrOutput(QString)));
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
	mRecentProjectsMenu = new QMenu("Recent projects", mUi->menu_File);
	mUi->menu_File->insertMenu(mUi->menu_File->actions().at(1), mRecentProjectsMenu);
	connect(mRecentProjectsMenu, SIGNAL(aboutToShow()), this, SLOT(openRecentProjectsMenu()));
}

void MainWindow::saveDiagramAsAPicture()
{
	QString const fileName = QFileDialog::getSaveFileName(this,  tr("Save File"), "", tr("Images (*.png *.jpg)"));
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

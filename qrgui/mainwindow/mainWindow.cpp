﻿#include "mainWindow.h"
#include "ui_mainWindow.h"

#include <QtCore/QProcess>
#include <QtWidgets/QDialog>
#include <QtPrintSupport/QPrinter>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtPrintSupport/QPrintDialog>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QListWidgetItem>
#include <QtCore/QPluginLoader>
#include <QtCore/QMetaType>
#include <QtSvg/QSvgGenerator>
#include <QtCore/QDebug>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QAction>
#include <QtGui/QKeySequence>

#include <qrkernel/settingsManager.h>

#include "mainwindow/errorReporter.h"

#include "mainwindow/shapeEdit/shapeEdit.h"
#include "mainwindow/propertyEditorProxyModel.h"
#include "dialogs/gesturesShow/gesturesWidget.h"

#include "models/models.h"
#include "view/editorView.h"

#include "controller/commands/removeElementCommand.h"
#include "controller/commands/doNothingCommand.h"
#include "controller/commands/arrangeLinksCommand.h"
#include "controller/commands/updateElementCommand.h"

#include "umllib/element.h"
#include "pluginManager/listenerManager.h"

#include "mainwindow/referenceList.h"

#include "mainwindow/splashScreen.h"
#include "dialogs/startDialog/startDialog.h"
#include "dialogs/suggestToCreateProjectDialog.h"
#include "dialogs/progressDialog/progressDialog.h"

#include "mainwindow/qscintillaTextEdit.h"

#include "mainwindow/dotRunner.h"
#include "view/sceneCustomizer.h"

#include "hotKeyManager/hotKeyManager.h"

using namespace qReal;
using namespace qReal::commands;

QString const unsavedDir = "unsaved";

MainWindow::MainWindow(QString const &fileToOpen)
		: mUi(new Ui::MainWindowUi)
		, mCodeTabManager(new QMap<EditorView*, CodeArea*>())
		, mModels(NULL)
		, mController(new Controller)
		, mEditorManagerProxy(new EditorManager())
		, mListenerManager(NULL)
		, mPropertyModel(mEditorManagerProxy)
		, mGesturesWidget(NULL)
		, mRootIndex(QModelIndex())
		, mErrorReporter(NULL)
		, mIsFullscreen(false)
		, mTempDir(qApp->applicationDirPath() + "/" + unsavedDir)
		, mPreferencesDialog(this)
		, mRecentProjectsLimit(SettingsManager::value("recentProjectsLimit").toInt())
		, mRecentProjectsMapper(new QSignalMapper())
		, mProjectManager(new ProjectManager(this))
		, mStartDialog(new StartDialog(*this, *mProjectManager))
		, mSceneCustomizer(new SceneCustomizer(this))
		, mInitialFileToOpen(fileToOpen)
{
	mUi->setupUi(this);
	mUi->paletteTree->initMainWindow(this);
	setWindowTitle("QReal");
	initSettingsManager();
	registerMetaTypes();
	SplashScreen splashScreen(SettingsManager::value("Splashscreen").toBool());
	splashScreen.setVisible(false);
	splashScreen.setProgress(5);

	initRecentProjectsMenu();
	initToolManager();
	initTabs();

	splashScreen.setProgress(20);

	initMiniMap();
	initGridProperties();

	splashScreen.setProgress(40);

	initDocks();
	mModels = new models::Models(mProjectManager->saveFilePath(), mEditorManagerProxy, mConstraintsManager);

	mErrorReporter = new gui::ErrorReporter(mUi->errorListWidget, mUi->errorDock);
	mErrorReporter->updateVisibility(SettingsManager::value("warningWindow").toBool());

	mPreferencesDialog.init(mUi->actionShow_grid, mUi->actionShow_alignment
			, mUi->actionSwitch_on_grid, mUi->actionSwitch_on_alignment);

	splashScreen.setProgress(60);

	loadPlugins();

	splashScreen.setProgress(70);

	mDocksVisibility.clear();

	splashScreen.setProgress(80);

	initActionsFromSettings();

	splashScreen.setProgress(100);
	if (!SettingsManager::value("maximized").toBool()) {
		showNormal();
		resize(SettingsManager::value("size").toSize());
		move(SettingsManager::value("pos").toPoint());
	} else {
		showMaximized();
	}
	splashScreen.close();

	mFindReplaceDialog = new FindReplaceDialog(mModels->logicalRepoApi(), this);
	mFindHelper = new FindManager(mModels->repoControlApi(), mModels->mutableLogicalRepoApi(), this, mFindReplaceDialog);
	connectActions();
	initExplorers();

	// So now we are going to load plugins. The problem is that if we will do it
	// here then we have some problems with correct main window initialization
	// beacuse of total event loop blocking by plugins. So waiting for main
	// window initialization complete and then loading plugins.
	QTimer::singleShot(50, this, SLOT(initPluginsAndStartDialog()));

	connect(&mModels->logicalModelAssistApi(), SIGNAL(propertyChanged(Id)), this, SLOT(checkConstraints(Id)));
	connect(&mPropertyModel, SIGNAL(propertyChangedFromPropertyEditor(QModelIndex)), this, SLOT(checkConstraints(QModelIndex)));
	connect(&mModels->logicalModelAssistApi(), SIGNAL(parentChanged(IdList)), this, SLOT(checkConstraints(IdList)));
	connect(&mModels->logicalModelAssistApi(), SIGNAL(nameChanged(Id)), this, SLOT(checkConstraints(Id)));
	connect(&mModels->graphicalModelAssistApi(), SIGNAL(nameChanged(Id)), this, SLOT(checkConstraints(Id)));
	connect(&mModels->logicalModelAssistApi(), SIGNAL(addedElementToModel(Id)), this, SLOT(checkConstraints(Id)));
}

void MainWindow::connectActions()
{
	mUi->actionShow_grid->setChecked(SettingsManager::value("ShowGrid").toBool());
	mUi->actionShow_alignment->setChecked(SettingsManager::value("ShowAlignment").toBool());
	mUi->actionSwitch_on_grid->setChecked(SettingsManager::value("ActivateGrid").toBool());
	mUi->actionSwitch_on_alignment->setChecked(SettingsManager::value("ActivateAlignment").toBool());
	connect(mUi->actionQuit, SIGNAL(triggered()), this, SLOT(close()));

	connect(mUi->actionShowSplash, SIGNAL(toggled(bool)), this, SLOT (toggleShowSplash(bool)));
	connect(mUi->actionOpen, SIGNAL(triggered()), mProjectManager, SLOT(suggestToOpenExisting()));
	connect(mUi->actionSave, SIGNAL(triggered()), mProjectManager, SLOT(saveOrSuggestToSaveAs()));
	connect(mUi->actionSave_as, SIGNAL(triggered()), mProjectManager, SLOT(suggestToSaveAs()));
	connect(mUi->actionSave_diagram_as_a_picture, SIGNAL(triggered()), this, SLOT(saveDiagramAsAPicture()));
	connect(mUi->actionPrint, SIGNAL(triggered()), this, SLOT(print()));
	connect(mUi->actionMakeSvg, SIGNAL(triggered()), this, SLOT(makeSvg()));

	connect(mUi->actionNew_Diagram, SIGNAL(triggered()), mProjectManager, SLOT(suggestToCreateDiagram()));
	connect(mUi->actionNewProject, SIGNAL(triggered()), this, SLOT(createProject()));

	connect(mUi->actionDeleteFromDiagram, SIGNAL(triggered()), this, SLOT(deleteFromDiagram()));
	connect(mUi->actionCopyElementsOnDiagram, SIGNAL(triggered()), this, SLOT(copyElementsOnDiagram()));
	connect(mUi->actionPasteOnDiagram, SIGNAL(triggered()), this, SLOT(pasteOnDiagram()));
	connect(mUi->actionPasteReference, SIGNAL(triggered()), this, SLOT(pasteCopyOfLogical()));

	connect(mUi->actionUndo, SIGNAL(triggered()), mController, SLOT(undo()));
	connect(mUi->actionRedo, SIGNAL(triggered()), mController, SLOT(redo()));

	connect(mUi->actionPreferences, SIGNAL(triggered()), this, SLOT(showPreferencesDialog()));

	connect(mUi->actionShow_grid, SIGNAL(toggled(bool)), this, SLOT(showGrid(bool)));
	connect(mUi->actionShow_alignment, SIGNAL(toggled(bool)), this, SLOT(showAlignment(bool)));
	connect(mUi->actionSwitch_on_grid, SIGNAL(toggled(bool)), this, SLOT(switchGrid(bool)));
	connect(mUi->actionSwitch_on_alignment, SIGNAL(toggled(bool)), this, SLOT(switchAlignment(bool)));

	connect(mUi->actionHelp, SIGNAL(triggered()), this, SLOT(showHelp()));
	connect(mUi->actionAbout, SIGNAL(triggered()), this, SLOT(showAbout()));
	connect(mUi->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

	connect(mUi->actionShow, SIGNAL(triggered()), this, SLOT(showGestures()));
	connect(mUi->actionFullscreen, SIGNAL(triggered()), this, SLOT(fullscreen()));

	connect(mUi->actionFind, SIGNAL(triggered()), this, SLOT(showFindDialog()));

	connect(mFindReplaceDialog, SIGNAL(replaceClicked(QStringList&))
			, mFindHelper, SLOT(handleReplaceDialog(QStringList&)));
	connect(mFindReplaceDialog, SIGNAL(findModelByName(QStringList))
			, mFindHelper, SLOT(handleFindDialog(QStringList)));
	connect(mFindReplaceDialog, SIGNAL(chosenElement(qReal::Id)), mFindHelper, SLOT(handleRefsDialog(qReal::Id)));

	connect(&mPreferencesDialog, SIGNAL(paletteRepresentationChanged()), this, SLOT(changePaletteRepresentation()));
	connect(mUi->paletteTree, SIGNAL(paletteParametersChanged()), &mPreferencesDialog, SLOT(changePaletteParameters()));

	connect(mController, SIGNAL(canUndoChanged(bool)), mUi->actionUndo, SLOT(setEnabled(bool)));
	connect(mController, SIGNAL(canRedoChanged(bool)), mUi->actionRedo, SLOT(setEnabled(bool)));
	connect(mController, SIGNAL(modifiedChanged(bool)), mProjectManager, SLOT(setUnsavedIndicator(bool)));

	connect(mProjectManager, SIGNAL(afterOpen(QString))
			, &mModels->logicalModelAssistApi().exploser(), SLOT(refreshAllPalettes()));
	connect(mProjectManager, SIGNAL(closed()), &mModels->logicalModelAssistApi().exploser(), SLOT(refreshAllPalettes()));
	connect(mProjectManager, SIGNAL(closed()), mController, SLOT(projectClosed()));

	connect(&mModels->logicalModelAssistApi().exploser(), SIGNAL(explosionTargetRemoved())
			, this, SLOT(closeTabsWithRemovedRootElements()));

	setDefaultShortcuts();
}

void MainWindow::initActionsFromSettings()
{
	mUi->actionShowSplash->setChecked(SettingsManager::value("Splashscreen").toBool());
}

void MainWindow::registerMetaTypes()
{
	qRegisterMetaType<Id>();
	qRegisterMetaTypeStreamOperators<Id>();
	qRegisterMetaType<IdList>();
	qRegisterMetaTypeStreamOperators<IdList>();
}

void MainWindow::showFindDialog()
{
	mFindReplaceDialog->stateClear();
	mFindReplaceDialog->show();
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
		CodeArea *area = dynamic_cast<CodeArea*>(mUi->tabs->currentWidget());
		if (area != NULL) {
			mProjectManager->saveGenCode(area->toPlainText());
		}
		mProjectManager->saveOrSuggestToSaveAs();
	} else if (keyEvent->modifiers() == Qt::ControlModifier && keyEvent->key() == Qt::Key_W) {
		closeTab(mUi->tabs->currentIndex());
	} else if (keyEvent->key() == Qt::Key_F1) {
		showHelp();
	} else if (keyEvent->modifiers() == Qt::ControlModifier && keyEvent->key() == Qt::Key_F) {
		mFindReplaceDialog->stateClear();
		mFindReplaceDialog->show();
	}
}

MainWindow::~MainWindow()
{
	QDir().rmdir(mTempDir);
	delete mListenerManager;
	delete mErrorReporter;
	mUi->paletteTree->saveConfiguration();
	SettingsManager::instance()->saveData();
	delete mRecentProjectsMenu;
	delete mRecentProjectsMapper;
	delete mModels;
	delete mController;
	delete mCodeTabManager;
	delete mFindReplaceDialog;
	delete mFindHelper;
	delete mProjectManager;
	delete mStartDialog;
	delete mSceneCustomizer;
}

EditorManagerInterface& MainWindow::editorManager()
{
	return mEditorManagerProxy;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	QList<ToolPluginInterface *> toolPlugins = mToolManager.getPlugins();
	foreach (ToolPluginInterface *toolPlugin, toolPlugins) {
		toolPlugin->closeNeededWidget();
	}

	if (!mProjectManager->suggestToSaveChangesOrCancel()) {
		event->ignore();
		return;
	}
	mProjectManager->close();

	SettingsManager::setValue("maximized", isMaximized());
	SettingsManager::setValue("size", size());
	SettingsManager::setValue("pos", pos());
}

void MainWindow::loadPlugins()
{
	mUi->paletteTree->loadPalette(SettingsManager::value("PaletteRepresentation").toBool()
			, SettingsManager::value("PaletteIconsInARowCount").toInt()
			, &mEditorManagerProxy);
}

void MainWindow::closeDiagramTab(Id const &id)
{
	IdList const graphicalIds = mModels->graphicalRepoApi().graphicalElements(id.type());
	if (!graphicalIds.isEmpty()) {
		// TODO: Why only for first graphical element?
		QModelIndex const index = mModels->graphicalModelAssistApi().indexById(graphicalIds[0]);
		for (int i = 0; i < mUi->tabs->count(); i++) {
			EditorView const * const tab = dynamic_cast<EditorView const *>(mUi->tabs->widget(i));
			if (tab != NULL && tab->mvIface()->rootIndex() == index) {
				mUi->tabs->removeTab(i);
			}
		}
	}
}

void MainWindow::clearSelectionOnTabs()
{
	for (int i = 0; i < mUi->tabs->count(); i++) {
		EditorView const * const tab = dynamic_cast<EditorView const *>(mUi->tabs->widget(i));
		if (tab != NULL) {
			tab->scene()->clearSelection();
		}
	}
}

void MainWindow::addEditorElementsToPalette(Id const &editor, Id const &diagram)
{
	mUi->paletteTree->addEditorElements(mEditorManagerProxy, editor, diagram);
}

void MainWindow::adjustMinimapZoom(int zoom)
{
	mUi->minimapView->resetMatrix();
	mUi->minimapView->scale(0.01 * zoom, 0.01 * zoom);
}

void MainWindow::selectItemWithError(Id const &id)
{
	if (id == Id::rootId()) {
		return;
	}

	setIndexesOfPropertyEditor(id);
	centerOn(id);
}

void MainWindow::selectItem(Id const &id)
{
	if (id == Id::rootId()) {
		return;
	}

	setIndexesOfPropertyEditor(id);
	centerOn(id);
}

void MainWindow::selectItemOrDiagram(Id const &graphicalId)
{
	activateItemOrDiagram(graphicalId, true);
}

void MainWindow::activateItemOrDiagram(QModelIndex const &idx, bool setSelected)
{
	QModelIndex const parent = idx.parent();
	int const numTab = getTabIndex(idx);

	if (numTab != -1) {
		mUi->tabs->setCurrentIndex(numTab);
		Id const currentTabId = getCurrentTab()->mvIface()->rootId();
		mToolManager.activeTabChanged(currentTabId);
	} else {
		openNewTab(idx);
	}

	if (mUi->tabs->isEnabled()) {
		if (parent == mModels->graphicalModelAssistApi().rootIndex()) {
			getCurrentTab()->mvIface()->setRootIndex(idx);
		} else {
			getCurrentTab()->mvIface()->setRootIndex(parent);
			// select this item on diagram
			getCurrentTab()->scene()->clearSelection();
			EditorViewScene const *scene = static_cast<EditorViewScene const *>(getCurrentTab()->scene());
			Element * const e = scene->getElem(idx.data(roles::idRole).value<Id>());
			if (e) {
				if (setSelected) {
					e->setSelected(true);
				}
			} else {
				Q_ASSERT(false);
			}
		}
	}
}

void MainWindow::activateItemOrDiagram(Id const &id, bool setSelected)
{
	if (mModels->graphicalModelAssistApi().isGraphicalId(id)) {
		activateItemOrDiagram(mModels->graphicalModelAssistApi().indexById(id), setSelected);
		return;
	}

	// id is logical ID
	IdList const graphicalIds = mModels->graphicalModelAssistApi().graphicalIdsByLogicalId(id);
	if (graphicalIds.count()) {
		activateItemOrDiagram(mModels->graphicalModelAssistApi().indexById(graphicalIds[0]), setSelected);
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

void MainWindow::refreshRecentProjectsList(QString const &fileName)
{
	QString previousString = SettingsManager::value("recentProjects").toString();
	QStringList previousList = previousString.split(";", QString::SkipEmptyParts);
	previousList.removeOne(fileName);
	if (!previousList.isEmpty() && (previousList.size() == mRecentProjectsLimit)) {
		previousList.removeLast();
	}
	previousList.push_front(fileName);
	previousString = "";
	QStringListIterator iterator(previousList);
	while (iterator.hasNext()) {
		previousString = previousString + iterator.next() + ";";
	}
	SettingsManager::setValue("recentProjects", previousString);
}

void MainWindow::openRecentProjectsMenu()
{
	delete mRecentProjectsMapper;
	mRecentProjectsMapper = new QSignalMapper;

	mRecentProjectsMenu->clear();
	QString const stringList = SettingsManager::value("recentProjects").toString();
	QStringList const recentProjects = stringList.split(";", QString::SkipEmptyParts);
	foreach (QString projectPath, recentProjects) {
		mRecentProjectsMenu->addAction(projectPath);
		QObject::connect(mRecentProjectsMenu->actions().last(), SIGNAL(triggered())
				, mRecentProjectsMapper, SLOT(map()));
		mRecentProjectsMapper->setMapping(mRecentProjectsMenu->actions().last(), projectPath);
	}

	QObject::connect(mRecentProjectsMapper, SIGNAL(mapped(QString const &))
			, mProjectManager, SLOT(openExisting(QString const &)));
}

void MainWindow::closeAllTabs()
{
	int const tabCount = mUi->tabs->count();

	for (int i = 0; i < tabCount; i++) {
		closeTab(0);
	}
}

void MainWindow::setReference(QStringList const &data, QPersistentModelIndex const &index, int const &role)
{
	removeOldBackReference(index, role);
	setData(data.join(','), index, role);
	foreach (QString const &target, data) {
		if (!target.isEmpty()) {
			setBackReference(index, target);
		}
	}
}

void MainWindow::setData(QString const &data, QPersistentModelIndex const &index, int const &role)
{
	// const_cast here is ok, since we need to set data in a correct model, and
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
	if (fileName.isEmpty()) {
		return;
	}

	newSvg.setFileName(fileName);
	newSvg.setSize(QSize(800, 600));

	QPainter painter(&newSvg);
	getCurrentTab()->scene()->render(&painter);
}

void MainWindow::deleteFromHighlightedElements(Element *element)
{
	EditorView const * const view = getCurrentTab();
	EditorViewScene* scene = NULL;
	if (view) {
		scene = dynamic_cast<EditorViewScene*>(view->scene());
	}
	if (scene) {
		scene->deleteFromHighlightedElements(element);
	}
}

void MainWindow::deleteElementFromDiagram(Id const &id)
{
	bool isLogical = mModels->logicalModelAssistApi().isLogicalId(id);
	if (isLogical) {
		mUi->logicalModelExplorer->setCurrentIndex(mModels->logicalModelAssistApi().indexById(id));
	} else {
		mUi->graphicalModelExplorer->setCurrentIndex(mModels->graphicalModelAssistApi().indexById(id));
	}
	deleteFromExplorer(isLogical);
}

void MainWindow::reportOperation(invocation::LongOperation *operation)
{
	ProgressDialog *progressDialog = new ProgressDialog(this);
	progressDialog->connectOperation(operation);
}

void MainWindow::deleteFromExplorer(bool isLogicalModel)
{
	if (isLogicalModel) {
		QModelIndex const index = mUi->logicalModelExplorer->currentIndex();
		if (index.isValid()) {
			mController->executeGlobal(logicalDeleteCommand(index));
		}
		return;
	}

	Id const id = mModels->graphicalModelAssistApi().idByIndex(
			mUi->graphicalModelExplorer->currentIndex());
	if (id != Id()) {
		deleteItems(IdList() << id, true);
	}
/* BUT : see my version */ //qwerty_old
}

void MainWindow::deleteItems(IdList &itemsToDelete, bool global)
{
	IdList itemsToUpdate;
	DoNothingCommand *multipleRemoveCommand = new DoNothingCommand;

	addEdgesToBeDeleted(itemsToDelete);
	// QGraphicsScene::selectedItems() returns items in no particular order,
	// so we should handle parent-child relationships manually
	while (!itemsToDelete.isEmpty()) {
		Id const currentItem = itemsToDelete.at(0);
		IdList const children = mModels->graphicalModelAssistApi().children(currentItem);
		foreach (Id const &child, children) {
			itemsToDelete.removeAll(child);
			// Child remove commands will be added in currentItem delete command
		}

		bool const isEdge = !mEditorManagerProxy.isGraphicalElementNode(currentItem);
		if (isEdge) {
			Id const src = mModels->graphicalModelAssistApi().from(currentItem);
			if (src != Id() && !itemsToUpdate.contains(src)) {
				itemsToUpdate.append(src);
			}
			Id const dst = mModels->graphicalModelAssistApi().to(currentItem);
			if (dst != Id() && !itemsToUpdate.contains(dst)) {
				itemsToUpdate.append(dst);
			}
			multipleRemoveCommand->insertPreAction(graphicalDeleteCommand(currentItem), 0);
		} else {
			multipleRemoveCommand->addPreAction(graphicalDeleteCommand(currentItem));
		}

		itemsToDelete.removeAll(currentItem);
	}

	multipleRemoveCommand->removeDuplicates();
	if (global) {
		mController->executeGlobal(multipleRemoveCommand);
	} else {
		mController->execute(multipleRemoveCommand);
	}

	/*foreach (NodeElement *item, itemsToArrangeLinks) { // qwerty_old
		if (item) {
			item->arrangeLinks();
			checkConstraints(item->logicalId());//проверяем на ограничения связанные элементы удаляемого линка в логической модели
		}
	}
	if (parentIndex != mModels->logicalModelAssistApi().indexById(Id::rootId())) {
		checkConstraints(parentIndex);//проверяем на ограничения родителя удаляемого элемента в логической модели
	}*/
}

void MainWindow::addEdgesToBeDeleted(IdList &itemsToDelete)
{
	IdList elementsToDelete = itemsToDelete;
	int i = 0;
	while (i < elementsToDelete.count()) {
		Id const currentElement = elementsToDelete.at(i);
		IdList const children = mModels->graphicalModelAssistApi().children(currentElement);
		elementsToDelete.append(children);
		i++;
	}
	foreach (Id const &currentElement, elementsToDelete) {
		IdList const linksOfCurrentElement = mModels->mutableLogicalRepoApi().links(currentElement);
		foreach (Id const &link, linksOfCurrentElement) {
			Id const otherEntityOfCurrentLink
					= mModels->mutableLogicalRepoApi().otherEntityFromLink(link, currentElement);
			if (otherEntityOfCurrentLink == Id::rootId() || elementsToDelete.contains(otherEntityOfCurrentLink)) {
				itemsToDelete.append(link);
			}
		}
	}
}

void MainWindow::removeReferences(Id const &id)
{
	mModels->logicalModelAssistApi().removeReferencesTo(id);
	mModels->logicalModelAssistApi().removeReferencesFrom(id);
}

void MainWindow::deleteFromScene()
{
	QList<QGraphicsItem *> itemsToDelete = getCurrentTab()->scene()->selectedItems();
	IdList idsToDelete;
	foreach (QGraphicsItem const *item, itemsToDelete) {
		Element const *element = dynamic_cast<Element const *>(item);
		if (element) {
			idsToDelete << element->id();
		}
	}

	deleteItems(idsToDelete);
}

AbstractCommand *MainWindow::logicalDeleteCommand(QGraphicsItem *target)
{
	Element *elem = dynamic_cast<Element *>(target);
	if (!elem || elem->id().isNull()) {
		return NULL;
	}
	return logicalDeleteCommand(elem->id());
}

AbstractCommand *MainWindow::graphicalDeleteCommand(QGraphicsItem *target)
{
	Element *elem = dynamic_cast<Element *>(target);
	if (!elem || elem->id().isNull()) {
		return NULL;
	}
	return graphicalDeleteCommand(elem->id());
/* BUT : see my version */ //qwerty_old
}

AbstractCommand *MainWindow::logicalDeleteCommand(QModelIndex const &index)
{
	Id const id = mModels->logicalModelAssistApi().idByIndex(index);
	return logicalDeleteCommand(id);
}

AbstractCommand *MainWindow::graphicalDeleteCommand(QModelIndex const &index)
{
	Id const id = mModels->graphicalModelAssistApi().idByIndex(index);
	return graphicalDeleteCommand(id);
}

commands::AbstractCommand *MainWindow::logicalDeleteCommand(Id const &id)
{
	// Logical deletion is equal to all its graphical parts deletion
	IdList const graphicalIds = mModels->graphicalModelAssistApi().graphicalIdsByLogicalId(id);

	if (graphicalIds.isEmpty()) {
		return new RemoveElementCommand(
				mModels->logicalModelAssistApi()
				, mModels->graphicalModelAssistApi()
				, mModels->logicalRepoApi().parent(id)
				, Id()
				, id
				, true
				, mModels->graphicalModelAssistApi().name(id)
				, mModels->graphicalModelAssistApi().position(id)
				);
	}

	DoNothingCommand *result = new DoNothingCommand;
	foreach (Id const &graphicalId, graphicalIds) {
		result->addPreAction(graphicalDeleteCommand(graphicalId));
	}
	if (graphicalIds.size() != 1) { // else it was done in graphicalDeleteCommand()
		appendExplosionsCommands(result, id);
	}
	result->removeDuplicates();
	return result;
}

commands::AbstractCommand *MainWindow::graphicalDeleteCommand(Id const &id)
{
	Id const logicalId = mModels->graphicalModelAssistApi().logicalId(id);
	AbstractCommand *result = new RemoveElementCommand(
				mModels->logicalModelAssistApi()
				, mModels->graphicalModelAssistApi()
				, mModels->logicalRepoApi().parent(logicalId)
				, mModels->graphicalRepoApi().parent(id)
				, id
				, false
				, mModels->graphicalModelAssistApi().name(id)
				, mModels->graphicalModelAssistApi().position(id)
				);

	connect(result, SIGNAL(redoComplete(bool)), this, SLOT(closeTabsWithRemovedRootElements()));
	IdList const children = mModels->graphicalModelAssistApi().children(id);
	foreach (Id const &child, children) {
		if (mEditorManagerProxy.isGraphicalElementNode(child)) {
			result->addPreAction(graphicalDeleteCommand(child));
		} else {
			// Edges are deletted first
			result->insertPreAction(graphicalDeleteCommand(child), 0);
		}
	}

	// correcting unremoved edges
	ArrangeLinksCommand *arrangeCommand = new ArrangeLinksCommand(getCurrentTab(), id, true);
	arrangeCommand->setRedoEnabled(false);
	result->addPreAction(arrangeCommand);

	UpdateElementCommand *updateCommand = new UpdateElementCommand(getCurrentTab(), id);
	updateCommand->setRedoEnabled(false);
	result->addPreAction(updateCommand);

	IdList const links = mModels->graphicalRepoApi().links(id);
	foreach (Id const &link, links) {
		UpdateElementCommand *updateLinkCommand = new UpdateElementCommand(getCurrentTab(), link);
		updateLinkCommand->setRedoEnabled(false);
		result->addPreAction(updateLinkCommand);
	}

	if (mModels->graphicalModelAssistApi().graphicalIdsByLogicalId(logicalId).size() == 1) {
		appendExplosionsCommands(result, logicalId);
	}

	return result;
}

void MainWindow::appendExplosionsCommands(AbstractCommand *parentCommand, Id const &logicalId)
{
	IdList const toDelete = mModels->logicalModelAssistApi().exploser().elementsWithHardDependencyFrom(logicalId);
	foreach (Id const &logicalChild, toDelete) {
		parentCommand->addPreAction(logicalDeleteCommand(logicalChild));
	}

	mModels->logicalModelAssistApi().exploser().handleRemoveCommand(logicalId, parentCommand);
}

void MainWindow::deleteFromDiagram()
{
	if (mModels->graphicalModel()) {
		if (mUi->graphicalModelExplorer->hasFocus()) {
			deleteFromExplorer(false);
		} else if (getCurrentTab() && getCurrentTab()->hasFocus()) {
			deleteFromScene();
		}
	}
	if (mModels->logicalModel()) {
		if (mUi->logicalModelExplorer->hasFocus()) {
			deleteFromExplorer(true);
		}
	}

	if (getCurrentTab() && getCurrentTab()->scene()) {
		getCurrentTab()->scene()->invalidate();
	}
}

void MainWindow::copyElementsOnDiagram()
{
	EditorViewScene* scene = dynamic_cast<EditorViewScene*>(getCurrentTab()->scene());
	if (scene) {
		scene->copy();
	}
}

void MainWindow::pasteOnDiagram()
{
	EditorViewScene* scene = dynamic_cast<EditorViewScene*>(getCurrentTab()->scene());
	if (scene) {
		scene->paste(false);
	}
}

void MainWindow::pasteCopyOfLogical()
{
	EditorViewScene* scene = dynamic_cast<EditorViewScene*>(getCurrentTab()->scene());
	if (scene) {
		scene->paste(true);
	}
}

void MainWindow::showAbout()
{
	QMessageBox::about(this, tr("About QReal"), mToolManager.customizer()->aboutText());
}

void MainWindow::showHelp()
{
	QString const url = QString("file:///%1/help/index.html").arg(QApplication::applicationDirPath());
	QDesktopServices::openUrl(QUrl(url));
}

void MainWindow::toggleShowSplash(bool show)
{
	SettingsManager::setValue("Splashscreen", show);
}

bool MainWindow::unloadPlugin(QString const &pluginName)
{
	if (mEditorManagerProxy.editors().contains(Id(pluginName))) {
		IdList const diagrams = mEditorManagerProxy.diagrams(Id(pluginName));

		if (!mEditorManagerProxy.unloadPlugin(pluginName)) {
			return false;
		}
		foreach (Id const &diagram, diagrams) {
			mUi->paletteTree->deleteEditor(diagram);
		}
	}
	return true;
}

bool MainWindow::unloadConstraintsPlugin(QString const &pluginName, QString const &pluginId)
{
	if (mConstraintsManager.pluginsIds().contains(Id(pluginId)) && mConstraintsManager.pluginsNames().contains(pluginName)) {
		if (!mConstraintsManager.unloadPlugin(pluginId)) {
			return false;
		}
	}
	return true;
}

bool MainWindow::loadPlugin(QString const &fileName, QString const &pluginName)
{
	if (!mEditorManagerProxy.loadPlugin(fileName)) {
		return false;
	}

	foreach (Id const &diagram, mEditorManagerProxy.diagrams(Id(pluginName))) {
		mUi->paletteTree->addEditorElements(mEditorManagerProxy, Id(pluginName), diagram);
	}

	mUi->paletteTree->initDone();
	return true;
}

bool MainWindow::loadConstraintsPlugin(QString const &fileName)
{
	if (!mConstraintsManager.loadPlugin(fileName)) {
		return false;
	}
	return true;
}

bool MainWindow::pluginLoaded(QString const &pluginName)
{
	return mEditorManagerProxy.editors().contains(Id(pluginName));
}

EditorView * MainWindow::getCurrentTab() const
{
	return dynamic_cast<EditorView *>(mUi->tabs->currentWidget());
}

bool MainWindow::isCurrentTabShapeEdit() const
{
	return dynamic_cast<ShapeEdit *>(mUi->tabs->currentWidget()) != NULL;
}

void MainWindow::closeCurrentTab()
{
	if (mUi->tabs->currentIndex() >= 0) {
		closeTab(mUi->tabs->currentIndex());
	}
}

void MainWindow::closeTab(int index)
{
	QWidget *widget = mUi->tabs->widget(index);
	CodeArea *possibleCodeTab = static_cast<CodeArea *>(widget);
	EditorView * deletingCodeTab = NULL;
	foreach (EditorView *diagram, mCodeTabManager->keys()) {
		if (mCodeTabManager->value(diagram) == possibleCodeTab) {
			deletingCodeTab = diagram;
		}
	}
	if (deletingCodeTab) {
		mCodeTabManager->remove(deletingCodeTab);
	}

	EditorView *editorView = dynamic_cast<EditorView *>(mUi->tabs->widget(index));
	if (editorView) {
		Id const diagramId = mModels->graphicalModelAssistApi().idByIndex(mRootIndex);
		mController->diagramClosed(diagramId);
	}

	mUi->tabs->removeTab(index);
	delete widget;
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
	mProjectManager->reinitAutosaver();
}

void MainWindow::initSettingsManager()
{
	SettingsManager::setValue("temp", mTempDir);
	QDir dir(qApp->applicationDirPath());
	if (!dir.cd(mTempDir)) {
		QDir().mkdir(mTempDir);
	}
}

void MainWindow::openSettingsDialog(QString const &tab)
{
	mPreferencesDialog.switchCurrentTab(tab);
	showPreferencesDialog();
}

void MainWindow::setSceneFont()
{
	if (SettingsManager::value("CustomFont").toBool()) {
		QFont font;
		font.fromString(SettingsManager::value("CurrentFont").toString());
		getCurrentTab()->scene()->setFont(font);
		getCurrentTab()->scene()->update();
	} else {
		getCurrentTab()->scene()->setFont(QFont(QFontDatabase::applicationFontFamilies(
				QFontDatabase::addApplicationFont(QDir::currentPath() + "/times.ttf")).at(0), 9));
		getCurrentTab()->scene()->update();
	}
}

// TODO: Unify overloads.
void MainWindow::openShapeEditor(QPersistentModelIndex const &index, int role, QString const &propertyValue)
{
	ShapeEdit *shapeEdit = new ShapeEdit(dynamic_cast<models::details::LogicalModel *>(mModels->logicalModel())
			, index, role);
	if (!propertyValue.isEmpty()) {
		shapeEdit->load(propertyValue);
	}

	// Here we are going to actually modify model to set a value of a shape.
	QAbstractItemModel *model = const_cast<QAbstractItemModel *>(index.model());
	model->setData(index, propertyValue, role);
	connect(shapeEdit, SIGNAL(shapeSaved(QString, QPersistentModelIndex const &, int const &))
			, this, SLOT(setData(QString, QPersistentModelIndex const &, int const &)));

	QString elementName = model->data(index, Qt::DisplayRole).toString();
	mUi->tabs->addTab(shapeEdit, tr("Shape Editor : %1").arg(elementName));
	mUi->tabs->setCurrentWidget(shapeEdit);
	setConnectActionZoomTo(shapeEdit);
}

// This method is for Interpreter.
void MainWindow::openShapeEditor(Id const &id, QString const &propertyValue, EditorManagerInterface *editorManagerProxy)
{
	ShapeEdit *shapeEdit = new ShapeEdit(id, editorManagerProxy, mModels->graphicalRepoApi(), this, getCurrentTab());
	if (!propertyValue.isEmpty()) {
		shapeEdit->load(propertyValue);
	}

	mUi->tabs->addTab(shapeEdit, tr("Shape Editor"));
	mUi->tabs->setCurrentWidget(shapeEdit);
	setConnectActionZoomTo(shapeEdit);
}

void MainWindow::openQscintillaTextEditor(QPersistentModelIndex const &index, int const role
		, QString const &propertyValue)
{
	gui::QScintillaTextEdit *textEdit = new gui::QScintillaTextEdit(index, role);
	if (!propertyValue.isEmpty()) {
		textEdit->setText(propertyValue.toUtf8());
	}

	textEdit->setPythonLexer();
	textEdit->setPythonEditorProperties();

	connect(textEdit, SIGNAL(textSaved(QString const &, QPersistentModelIndex const &, int const &))
			, this, SLOT(setData(QString const &, QPersistentModelIndex const &, int const &)));

	mUi->tabs->addTab(textEdit, tr("Text Editor"));
	mUi->tabs->setCurrentWidget(textEdit);
	setConnectActionZoomTo(textEdit);
}

void MainWindow::openShapeEditor()
{
	ShapeEdit * const shapeEdit = new ShapeEdit;
	mUi->tabs->addTab(shapeEdit, tr("Shape Editor"));
	mUi->tabs->setCurrentWidget(shapeEdit);
	setConnectActionZoomTo(shapeEdit);
}

void MainWindow::openReferenceList(QPersistentModelIndex const &index
		, QString const &referenceType,	QString const &propertyValue, int role)
{
	ReferenceList referenceList(this, index, referenceType, propertyValue.split(',', QString::SkipEmptyParts), role);
	connect(&referenceList, SIGNAL(referenceSet(QStringList, QPersistentModelIndex, int))
			, this, SLOT(setReference(QStringList, QPersistentModelIndex, int)));
	referenceList.exec();
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
	if (view != NULL) {
		disconnectZoom(view);
	} else {
		ShapeEdit *const shapeWidget = dynamic_cast<ShapeEdit *>(widget);
		if (shapeWidget != NULL) {
			disconnectZoom(shapeWidget->getView());
		}
	}
}

void MainWindow::connectActionZoomTo(QWidget* widget)
{
	EditorView * const view = (dynamic_cast<EditorView *>(widget));
	if (view != NULL) {
		connectZoom(view);
	} else {
		ShapeEdit * const shapeWidget = (dynamic_cast<ShapeEdit *>(widget));
		if (shapeWidget != NULL) {
			connectZoom(shapeWidget->getView());
		}
	}
}

void MainWindow::setConnectActionZoomTo(QWidget* widget)
{
	for (int i = 0; i < mUi->tabs->count(); i++) {
		disconnectActionZoomTo(mUi->tabs->widget(i));
	}

	connectActionZoomTo(widget);
}

void MainWindow::centerOn(Id const &id)
{
	if (id.isNull() || mEditorManagerProxy.isDiagramNode(id)) {
		return;
	}

	EditorView* const view = getCurrentTab();
	if (!view) {
		return;
	}

	EditorViewScene* const scene = dynamic_cast<EditorViewScene*>(view->scene());
	Element* const element = scene->getElem(id);

	scene->clearSelection();
	if (element) {
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
		if (view) {
			Id const diagramId = mModels->graphicalModelAssistApi().idByIndex(index);
			mController->diagramOpened(diagramId);
		}
		mSceneCustomizer->customizeView(view);
		initCurrentTab(view, index);
		mUi->tabs->addTab(view, index.data().toString());
		mUi->tabs->setCurrentWidget(view);

		// Focusing on scene top left corner
		view->centerOn(view->scene()->sceneRect().topLeft());
	}

	// changing of palette active editor
	if (SettingsManager::value("PaletteTabSwitching").toBool()) {
		int i = 0;
		foreach (QString const &name, mUi->paletteTree->editorsNames()) {
			Id const id = mModels->graphicalModelAssistApi().idByIndex(index);
			Id const diagramId = Id(id.editor(), id.diagram());
			QString const diagramName = mEditorManagerProxy.friendlyName(diagramId);
			if (diagramName == name) {
				mUi->paletteTree->setComboBoxIndex(i);
				break;
			}
			i++;
		}
	}
}

void MainWindow::openFirstDiagram()
{
	Id const rootId = mModels->graphicalModelAssistApi().rootId();
	IdList const rootIds = mModels->graphicalModelAssistApi().children(rootId);
	if (rootIds.count() == 0) {
		return;
	}

	Id firstDiagram = Id::rootId();
	foreach (Id currentChild, rootIds) { // qwerty_asd_temp
		if (mModels->graphicalModelAssistApi().isGraphicalId(currentChild)) {
			firstDiagram = currentChild;
			break;
		}
	}
	openNewTab(mModels->graphicalModelAssistApi().indexById(firstDiagram));
}

void MainWindow::closeTabsWithRemovedRootElements()
{
	Id const rootId = mModels->graphicalModelAssistApi().rootId();
	IdList const rootIds = mModels->graphicalModelAssistApi().children(rootId);
	int i = 0;
	while (i < mUi->tabs->count()) {
		EditorView *tab = (dynamic_cast<EditorView *>(mUi->tabs->widget(i)));
		if (tab && !rootIds.contains(tab->mvIface()->rootId())) {
			closeTab(i);
			break;
		} else {
			++i;
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

	EditorViewScene *scene = dynamic_cast<EditorViewScene *>(tab->scene());
	if (scene) {
		scene->initNodes();
	}
}

void MainWindow::setShortcuts(EditorView * const tab)
{
	EditorViewScene *scene = dynamic_cast<EditorViewScene *>(tab->scene());
	if (!scene) {
		return;
	}

	// Add shortcut - select all
	QAction *selectAction = new QAction(tab);
	selectAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_A));
	connect(selectAction, SIGNAL(triggered()), scene, SLOT(selectAll()));
	tab->addAction(selectAction);
}

void MainWindow::setDefaultShortcuts()
{
	QAction *closeCurrentTabAction = new QAction(this);
	QAction *closeAllTabsAction = new QAction(this);
	connect(closeCurrentTabAction, SIGNAL(triggered()), this, SLOT(closeCurrentTab()));
	connect(closeAllTabsAction, SIGNAL(triggered()), this, SLOT(closeAllTabs()));
	addAction(closeCurrentTabAction);
	addAction(closeAllTabsAction);

	closeCurrentTabAction->setShortcuts(QList<QKeySequence>()
			<< QKeySequence(Qt::CTRL + Qt::Key_W)
			<< QKeySequence(Qt::CTRL + Qt::Key_F4));
	closeAllTabsAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_W));

	mUi->actionUndo->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Z));
	mUi->actionRedo->setShortcuts(QList<QKeySequence>()
			<< QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_Z)
			<< QKeySequence(Qt::CTRL + Qt::Key_Y));

	mUi->actionZoom_In->setShortcuts(QList<QKeySequence>()
			<< QKeySequence(Qt::CTRL + Qt::Key_Equal)
			<< QKeySequence(Qt::CTRL + Qt::Key_Plus));
	mUi->actionZoom_Out->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Minus));

	mUi->actionNewProject->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
	mUi->actionNew_Diagram->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_T));

	// TODO: bind Ctrl+P to print when it will be repaired
	// TODO: bind Ctrl+F to find dialog when it will be repaired

	HotKeyManager::setCommand("File.Open", tr("Open project"), mUi->actionOpen);
	HotKeyManager::setCommand("File.Save", tr("Save project"), mUi->actionSave);
	HotKeyManager::setCommand("File.SaveAs", tr("Save project as"), mUi->actionSave_as);
	HotKeyManager::setCommand("File.NewProject", tr("New project"), mUi->actionNewProject);
	HotKeyManager::setCommand("File.NewDiagram", tr("New diagram"), mUi->actionNew_Diagram);
	HotKeyManager::setCommand("Editor.Undo", tr("Undo"), mUi->actionUndo);
	HotKeyManager::setCommand("Editor.Redo", tr("Redo"), mUi->actionRedo);
	HotKeyManager::setCommand("Editor.ZoomIn", tr("Zoom In"), mUi->actionZoom_In);
	HotKeyManager::setCommand("Editor.ZoomOut", tr("Zoom Out"), mUi->actionZoom_Out);
	HotKeyManager::setCommand("Editor.CloseCurrentTab", tr("Close current tab"), closeCurrentTabAction);
	HotKeyManager::setCommand("Editor.CloseAllTabs", tr("Close all tabs"), closeAllTabsAction);
	HotKeyManager::setCommand("Editor.Print", tr("Print"), mUi->actionPrint);
	HotKeyManager::setCommand("Editor.Find", tr("Find"), mUi->actionFind);
}

void MainWindow::currentTabChanged(int newIndex)
{
	switchToTab(newIndex);
	mUi->minimapView->changeSource(newIndex);

	bool const isEditorTab = getCurrentTab() != NULL;
	bool const isShape = isCurrentTabShapeEdit();

	mUi->actionSave_diagram_as_a_picture->setEnabled(isEditorTab);
	// TODO: implement printing for text tabs
	mUi->actionPrint->setEnabled(isEditorTab);

	mUi->actionRedo->setEnabled(mController->canRedo() && !isShape);
	mUi->actionUndo->setEnabled(mController->canUndo() && !isShape);

	mUi->actionZoom_In->setEnabled(isEditorTab || isShape);
	mUi->actionZoom_Out->setEnabled(isEditorTab || isShape);

	if (!isEditorTab) {
		mToolManager.activeTabChanged(Id());
	} else if (getCurrentTab()->mvIface()) {
		Id const currentTabId = getCurrentTab()->mvIface()->rootId();
		mToolManager.activeTabChanged(currentTabId);
	}

	emit rootDiagramChanged();
}

void MainWindow::switchToTab(int index)
{
	if (index != -1) {
		mUi->tabs->setEnabled(true);
		EditorView *editorView = getCurrentTab();
		setConnectActionZoomTo(mUi->tabs->currentWidget());

		if (editorView != NULL && (static_cast<EditorViewScene*>(editorView->scene()))->mainWindow() != NULL) {
			getCurrentTab()->mvIface()->setModel(mModels->graphicalModel());
			getCurrentTab()->mvIface()->setLogicalModel(mModels->logicalModel());
			mRootIndex = editorView->mvIface()->rootIndex();
			Id const diagramId = mModels->graphicalModelAssistApi().idByIndex(mRootIndex);
			mController->setActiveDiagram(diagramId);
		}
	} else {
		mUi->tabs->setEnabled(false);
		mController->setActiveDiagram(Id());
	}

}

void MainWindow::updateTabName(Id const &id)
{
	for (int i = 0; i < mUi->tabs->count(); i++) {
		EditorView * const tab = (dynamic_cast<EditorView *>(mUi->tabs->widget(i)));
		if (tab && (tab->mvIface()->rootIndex() == mModels->graphicalModelAssistApi().indexById(id))) {
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

void MainWindow::cropSceneToItems()
{
	EditorView *view = getCurrentTab();
	if (view == NULL) {
		return;
	}
	EditorViewScene *scene = dynamic_cast<EditorViewScene *>(view->scene());
	if (scene != NULL) {
		scene->cropToItems();
	}
}

ListenerManager *MainWindow::listenerManager() const
{
	return mListenerManager;
}

models::Models *MainWindow::models() const
{
	return mModels;
}

Controller *MainWindow::controller() const
{
	return mController;
}

PropertyEditorView *MainWindow::propertyEditor() const
{
	return mUi->propertyEditor;
}

QTreeView *MainWindow::graphicalModelExplorer() const
{
	return mUi->graphicalModelExplorer;
}

QTreeView *MainWindow::logicalModelExplorer() const
{
	return mUi->logicalModelExplorer;
}

PropertyEditorModel &MainWindow::propertyModel()
{
	return mPropertyModel;
}

ToolPluginManager &MainWindow::toolManager()
{
	return mToolManager;
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
		if (tab != NULL) {
			tab->setDrawSceneGrid(isChecked);
		}
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
				if (nodeItem != NULL) {
					nodeItem->showAlignment(isChecked);
				}
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
				if (nodeItem != NULL) {
					nodeItem->switchGrid(isChecked);
				}
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
				if (nodeItem != NULL) {
					nodeItem->switchAlignment(isChecked);
				}
			}
		}
	}
}

void MainWindow::showGestures()
{
	mGesturesWidget = new gestures::GesturesWidget();
	mUi->tabs->addTab(mGesturesWidget, tr("Gestures Show"));
	mUi->tabs->setCurrentWidget(mGesturesWidget);
	connect(mGesturesWidget, SIGNAL(currentElementChanged()), this, SIGNAL(currentIdealGestureChanged()));
	emit gesturesShowed();
}

gestures::GesturesPainterInterface * MainWindow::gesturesPainter() const
{
	return mGesturesWidget;
}

ProxyEditorManager &MainWindow::editorManagerProxy()
{
	return mEditorManagerProxy;
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

bool MainWindow::createProject(QString const &diagramIdString)
{
	if (!mProjectManager->openEmptyWithSuggestToSaveChanges()) {
		return false;
	}
	createDiagram(diagramIdString);
	return true;
}

void MainWindow::createProject()
{
	Id const theOnlyDiagram = mEditorManagerProxy.theOnlyDiagram();
	if (theOnlyDiagram.isNull()) {
		SuggestToCreateProjectDialog dialog(this);
		dialog.exec();
	} else {
		Id const editor = editorManager().editors()[0];
		QString const diagramIdString = editorManager().diagramNodeNameString(editor, theOnlyDiagram);
		createProject(diagramIdString);
	}
}

int MainWindow::getTabIndex(const QModelIndex &index)
{
	for (int i = 0; i < mUi->tabs->count(); ++i) {
		EditorView * const editor = dynamic_cast<EditorView *>(mUi->tabs->widget(i));
		if (!editor) {
			continue;
		}
		if (index.parent() == editor->mvIface()->rootIndex()) {
			return i;
		}
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

QAction *MainWindow::actionCopyElementsOnDiagram() const
{
	return mUi->actionCopyElementsOnDiagram;
}

QAction *MainWindow::actionPasteOnDiagram() const
{
	return mUi->actionPasteOnDiagram;
}

QAction *MainWindow::actionPasteCopyOfLogical() const
{
	return mUi->actionPasteReference;
}

void MainWindow::highlight(Id const &graphicalId, bool exclusive, QColor const &color)
{
	for (int i = 0; i < mUi->tabs->count(); ++i) {
		EditorView * const view = dynamic_cast<EditorView *>(mUi->tabs->widget(i));
		if (!view) {
			continue;
		}
		EditorViewScene * const scene = dynamic_cast<EditorViewScene *>(view->scene());
		Element const * const element = scene->getElem(graphicalId);
		if (element) {
			scene->highlight(graphicalId, exclusive, color);
			view->ensureElementVisible(element, 0, 0);
		}
	}
}

void MainWindow::dehighlight(Id const &graphicalId)
{
	for (int i = 0; i < mUi->tabs->count(); ++i) {
		EditorView * const view = dynamic_cast<EditorView *>(mUi->tabs->widget(i));
		if (!view) {
			continue;
		}

		EditorViewScene * const scene = dynamic_cast<EditorViewScene *>(view->scene());

		if (graphicalId.isNull()) {
			scene->dehighlight();
		} else {
			scene->dehighlight(graphicalId);
		}
	}
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
	loadPlugins();

	mUi->paletteTree->setActiveEditor(currentId);
	mUi->paletteTree->setComboBox(currentId);
}

void MainWindow::applySettings()
{
	for (int i = 0; i < mUi->tabs->count(); i++) {
		EditorView * const tab = static_cast<EditorView *>(mUi->tabs->widget(i));
		EditorViewScene *scene = dynamic_cast <EditorViewScene *> (tab->scene());
		if (scene) {
			scene->updateEdgeElements();
			scene->invalidate();
		}
	}
	mErrorReporter->updateVisibility(SettingsManager::value("warningWindow", true).toBool());
}

void MainWindow::setBackReference(QPersistentModelIndex const &index, QString const &data)
{
	Id id = Id::loadFromString(data);
	Id indexId = mModels->logicalModelAssistApi().idByIndex(index);
	mModels->logicalRepoApi().setBackReference(id, indexId);
}

void MainWindow::removeOldBackReference(QPersistentModelIndex const &index, int const role)
{
	QStringList data = index.data(role).toString().split(',', QString::SkipEmptyParts);

	foreach (QString const &reference, data) {
		Id id = Id::loadFromString(reference);
		Id indexId = mModels->logicalModelAssistApi().idByIndex(index);
		mModels->logicalRepoApi().removeBackReference(id, indexId);
	}
}

void MainWindow::hideDockWidget(QDockWidget *dockWidget, QString const &name)
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
	foreach (QDockWidget *dock, mAdditionalDocks) {
		if (mIsFullscreen) {
			hideDockWidget(dock, dock->windowTitle());
		} else {
			showDockWidget(dock, dock->windowTitle());
		}
	}
}

QString MainWindow::getNextDirName(QString const &name)
{
	QStringList parts = name.split("_");
	bool isInt = false;
	int version = parts.last().toInt(&isInt);

	if (parts.size() < 2 || !isInt) {
		return name + "_2";
	}

	parts.last() = QString::number(++version);
	return parts.join("_");
}

Id MainWindow::activeDiagram()
{
	return getCurrentTab() && getCurrentTab()->mvIface() ? getCurrentTab()->mvIface()->rootId() : Id();
}

void MainWindow::initPluginsAndStartDialog()
{
	initToolPlugins();
	if (!mProjectManager->restoreIncorrectlyTerminated() &&
			(mInitialFileToOpen.isEmpty() || !mProjectManager->open(mInitialFileToOpen)))
	{
		mStartDialog->setVisibleForInterpreterButton(mToolManager.customizer()->showInterpeterButton());
		// Centering dialog inside main window
		mStartDialog->move(geometry().center() - mStartDialog->rect().center());
		mStartDialog->exec();
	}
}

void MainWindow::addActionOrSubmenu(QMenu *target, ActionInfo const &actionOrMenu)
{
	if (actionOrMenu.isAction()) {
		target->addAction(actionOrMenu.action());
	} else {
		target->addMenu(actionOrMenu.menu());
	}
}

void MainWindow::initToolPlugins()
{
	mToolManager.init(PluginConfigurator(mModels->repoControlApi(), mModels->graphicalModelAssistApi()
			, mModels->logicalModelAssistApi(), *this, *mProjectManager, *mSceneCustomizer));

	QList<ActionInfo> const actions = mToolManager.actions();
	foreach (ActionInfo const action, actions) {
		if (action.isAction()) {
			if (action.toolbarName() == "file") {
				mUi->fileToolbar->addAction(action.action());
			} else if (action.toolbarName() == "interpreters") {
				mUi->interpreterToolbar->addAction(action.action());
			} else if (action.toolbarName() == "generators") {
				mUi->generatorsToolbar->addAction(action.action());
			}
		}
	}

	foreach (ActionInfo const action, actions) {
		if (action.menuName() == "tools") {
			addActionOrSubmenu(mUi->menuTools, action);
		} else if (action.menuName() == "settings") {
			addActionOrSubmenu(mUi->menuSettings, action);
		}
	}

	if (mUi->parsersToolbar->actions().isEmpty()) {
		mUi->parsersToolbar->hide();
	}

	if (mUi->generatorsToolbar->actions().isEmpty()) {
		mUi->generatorsToolbar->hide();
	}

	if (mUi->interpreterToolbar->actions().isEmpty()) {
		mUi->interpreterToolbar->hide();
	}

	QList<QPair<QString, PreferencesPage *> > const preferencesPages = mToolManager.preferencesPages();
	typedef QPair<QString, PreferencesPage *> PageDescriptor;
	foreach (PageDescriptor const page, preferencesPages) {
		mPreferencesDialog.registerPage(page.first, page.second);
	}

	mModels->logicalModelAssistApi().exploser().customizeExplosionTitles(
			toolManager().customizer()->userPaletteTitle()
			, toolManager().customizer()->userPaletteDescription());
}

void MainWindow::showErrors(gui::ErrorReporter const * const errorReporter)
{
	errorReporter->showErrors(mUi->errorListWidget, mUi->errorDock);
}

bool MainWindow::showConnectionRelatedMenus() const
{
	return mToolManager.customizer()->showConnectionRelatedMenus();
}

bool MainWindow::showUsagesRelatedMenus() const
{
	return mToolManager.customizer()->showUsagesRelatedMenus();
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
		} else {
			CodeArea * const area = mCodeTabManager->value(getCurrentTab());
			area->document()->setPlainText(text);
			area->show();
			mUi->tabs->setCurrentWidget(area);
		}
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

void MainWindow::initToolManager()
{
	Customizer * const customizer = mToolManager.customizer();
	if (customizer) {
		setWindowTitle(customizer->windowTitle());
		setWindowIcon(customizer->applicationIcon());
		setVersion(customizer->productVersion());
		customizer->customizeDocks(this);
	}
}

void MainWindow::initMiniMap()
{
	mUi->minimapView->init(this);
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
	mUi->actionSwitch_on_grid->setChecked(SettingsManager::value("ActivateGrid").toBool());

	mUi->actionShow_grid->blockSignals(false);
	mUi->actionShow_grid->setChecked(SettingsManager::value("ShowGrid").toBool());
}

void MainWindow::initExplorers()
{
	mUi->propertyEditor->init(this, &mModels->logicalModelAssistApi());
	mUi->propertyEditor->setModel(&mPropertyModel);

	mUi->graphicalModelExplorer->addAction(mUi->actionDeleteFromDiagram);
	mUi->graphicalModelExplorer->addAction(mUi->actionCopyElementsOnDiagram);
	mUi->graphicalModelExplorer->addAction(mUi->actionPasteOnDiagram);
	mUi->graphicalModelExplorer->addAction(mUi->actionPasteReference);
	mUi->graphicalModelExplorer->setModel(mModels->graphicalModel());
	mUi->graphicalModelExplorer->setController(mController);
	mUi->graphicalModelExplorer->setAssistApi(&mModels->graphicalModelAssistApi());
	mUi->graphicalModelExplorer->setExploser(&mModels->logicalModelAssistApi().exploser());

	mUi->logicalModelExplorer->addAction(mUi->actionDeleteFromDiagram);
	mUi->logicalModelExplorer->addAction(mUi->actionCopyElementsOnDiagram);
	mUi->logicalModelExplorer->addAction(mUi->actionPasteOnDiagram);
	mUi->logicalModelExplorer->addAction(mUi->actionPasteReference);
	mUi->logicalModelExplorer->setModel(mModels->logicalModel());
	mUi->logicalModelExplorer->setController(mController);
	mUi->logicalModelExplorer->setAssistApi(&mModels->logicalModelAssistApi());
	mUi->logicalModelExplorer->setExploser(&mModels->logicalModelAssistApi().exploser());

	mPropertyModel.setSourceModels(mModels->logicalModel(), mModels->graphicalModel());

	connect(&mModels->graphicalModelAssistApi(), SIGNAL(nameChanged(Id const &))
			, this, SLOT(updateTabName(Id const &)));
	connect(mUi->graphicalModelExplorer, SIGNAL(clicked(QModelIndex const &))
			, this, SLOT(graphicalModelExplorerClicked(QModelIndex)));
	connect(mUi->logicalModelExplorer, SIGNAL(clicked(QModelIndex const &))
			, this, SLOT(logicalModelExplorerClicked(QModelIndex)));
}

void MainWindow::initRecentProjectsMenu()
{
	mRecentProjectsMenu = new QMenu(tr("Recent projects"), mUi->menu_File);
	mUi->menu_File->insertMenu(mUi->menu_File->actions().at(1), mRecentProjectsMenu);
	connect(mRecentProjectsMenu, SIGNAL(aboutToShow()), this, SLOT(openRecentProjectsMenu()));
}

void MainWindow::saveDiagramAsAPictureToFile(QString const &fileName)
{
	if (fileName.isEmpty()) {
		return;
	}
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
	if (getCurrentTab()) {
		QString const fileName = QFileDialog::getSaveFileName(this,  tr("Save File"), "", tr("Images (*.png *.jpg)"));
		saveDiagramAsAPictureToFile(fileName);
	}
}

void MainWindow::changePaletteRepresentation()
{
	if (SettingsManager::value("PaletteRepresentation").toBool() != mUi->paletteTree->iconsView()
			|| SettingsManager::value("PaletteIconsInARowCount").toInt() != mUi->paletteTree->itemsCountInARow())
	{
		loadPlugins();
	}
}

void MainWindow::arrangeElementsByDotRunner(QString const &algorithm, QString const &absolutePathToDotFiles)
{
	Id const diagramId = activeDiagram();
	DotRunner *runner = new DotRunner(
			diagramId
			, mModels->graphicalModelAssistApi()
			, mModels->logicalModelAssistApi()
			, mEditorManagerProxy
			, absolutePathToDotFiles
			);

	if (runner->run(algorithm)) {
		updateActiveDiagram();
	}
}

IdList MainWindow::selectedElementsOnActiveDiagram()
{
	if (!getCurrentTab()) {
		return IdList();
	}
	QList<QGraphicsItem*> items = getCurrentTab()->scene()->items();

	IdList selected;
	foreach (QGraphicsItem* item, items) {
		Element* element = dynamic_cast<Element*>(item);
		if (element) {
			if (element->isSelected()) {
				selected.append(element->id());
			}
		}
	}
	return selected;
}

void MainWindow::updateActiveDiagram()
{
	Id const diagramId = activeDiagram();
	reinitModels();
	activateItemOrDiagram(diagramId);
	mUi->graphicalModelExplorer->setRootIndex(QModelIndex());
}

gui::Error::Severity MainWindow::severityByErrorType(CheckStatus::ErrorType const &errorType)
{
	if (errorType == CheckStatus::warning) {
		return gui::Error::warning;
	} else if (errorType == CheckStatus::critical) {
		return gui::Error::critical;
	} else if (errorType == CheckStatus::verification) {
		return gui::Error::error; //else gui::Error::warning
	}
	return gui::Error::warning;
}

void MainWindow::checkOwnConstraints(Id const &id)
{
	Id const logicalId = mModels->logicalId(id);
	IdList const graphicalIds = mModels->graphicalModelAssistApi().graphicalIdsByLogicalId(logicalId);

	QList<CheckStatus> checkStatusList = mConstraintsManager.check(logicalId, mModels->logicalModelAssistApi().logicalRepoApi(), mEditorManagerProxy);
	bool checkStatus = true;
	foreach (CheckStatus check, checkStatusList) {
		gui::Error::Severity errorSeverity = severityByErrorType(check.errorType());
		QString errorMessage = check.message();

		if (check.checkStatus()) {
			if (errorSeverity != gui::Error::warning) {
				mErrorReporter->delUniqueError(errorMessage, errorSeverity, id);
			}
		} else {
			checkStatus = false;
			if (errorSeverity != gui::Error::warning) {
				mErrorReporter->addUniqueError(errorMessage, errorSeverity, id);
			}
		}
	}

	if (checkStatus) {
		foreach (Id const &graphicalId, graphicalIds) {
			dehighlight(graphicalId);
		}
	} else {
		foreach (Id const &graphicalId, graphicalIds) {
			highlight(graphicalId, false);
		}
	}
}

void MainWindow::checkConstraints(Id const &id)
{
	EditorManagerInterface::MetaType metaType = mEditorManagerProxy.metaTypeOfElement(id);
	checkOwnConstraints(id);
	if (metaType == EditorManagerInterface::node) {
		QModelIndex index = mModels->logicalModelAssistApi().indexById(id);
		checkChildrensConstraints(id);
		checkParentsConstraints(index);
		checkLinksConstraints(id);
	}
}

void MainWindow::checkConstraints(QModelIndex const &index)
{
	Id const id = mModels->logicalModelAssistApi().idByIndex(index);
	checkConstraints(id);
}

void MainWindow::checkConstraints(IdList const &idList)
{
	foreach (Id const &id, idList) {
		checkConstraints(id);
	}
}

void MainWindow::checkParentsConstraints(QModelIndex const &index)
{
	QModelIndex parent = mModels->logicalModel()->parent(index);
	Id const parentId = mModels->logicalModelAssistApi().idByIndex(parent);
	if (mModels->logicalModelAssistApi().isLogicalId(parentId)) {
		checkOwnConstraints(parentId);
		checkParentsConstraints(parent);
	}
}

void MainWindow::checkChildrensConstraints(Id const &id)
{
	IdList childrenList = mModels->logicalModelAssistApi().children(id);
	foreach (Id const &childrenId, childrenList) {
		if (mModels->logicalModelAssistApi().isLogicalId(childrenId)) {
			checkOwnConstraints(childrenId);
			checkChildrensConstraints(childrenId);
		}
	}
}

void MainWindow::checkLinksConstraints(Id const &id)
{
	IdList linksList = mModels->logicalRepoApi().links(id);
	foreach (Id const &linkId, linksList) {
		if (mModels->logicalModelAssistApi().isLogicalId(linkId)) {
			checkOwnConstraints(linkId);
		}
	}
}

QDockWidget *MainWindow::logicalModelDock() const
{
	return mUi->logicalModelDock;
}

QDockWidget *MainWindow::graphicalModelDock() const
{
	return mUi->graphicalModelDock;
}

QDockWidget *MainWindow::propertyEditorDock() const
{
	return mUi->propertyDock;
}

QDockWidget *MainWindow::errorReporterDock() const
{
	return mUi->errorDock;
}

QDockWidget *MainWindow::paletteDock() const
{
	return mUi->paletteDock;
}

void MainWindow::tabifyDockWidget(QDockWidget *first, QDockWidget *second)
{
	QMainWindow::tabifyDockWidget(first, second);
}

void MainWindow::addDockWidget(Qt::DockWidgetArea area, QDockWidget *dockWidget)
{
	mAdditionalDocks << dockWidget;
	QMainWindow::addDockWidget(area, dockWidget);
}

QListIterator<EditorView *> MainWindow::openedEditorViews() const
{
	QList<EditorView *> views;
	for (int i = 0; i < mUi->tabs->count(); ++i) {
		EditorView *view = (dynamic_cast<EditorView *>(mUi->tabs->widget(i)));
		if (view) {
			views << view;
		}
	}
	return QListIterator<EditorView *>(views);
}

void MainWindow::setVersion(QString const &version)
{
	// TODO: update title
	SettingsManager::setValue("version", version);
}

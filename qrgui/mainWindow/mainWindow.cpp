/* Copyright 2007-2015 QReal Research Group, Dmitry Mordvinov
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "mainWindow.h"
#include "ui_mainWindow.h"

#include <QtCore/QProcess>
#include <QtCore/QPluginLoader>
#include <QtCore/QMetaType>
#include <QtGui/QDesktopServices>
#include <QtGui/QKeySequence>
#include <QtWidgets/QDialog>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QListWidgetItem>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QAction>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QtSvg/QSvgGenerator>

#include <qrkernel/settingsManager.h>
#include <qrkernel/settingsListener.h>
#include <qrkernel/logging.h>
#include <qrutils/outFile.h>
#include <qrutils/stringUtils.h>
#include <qrutils/qRealFileDialog.h>
#include <qrutils/graphicsUtils/animatedHighlighter.h>
#include <thirdparty/qscintilla/Qt4Qt5/Qsci/qsciprinter.h>
#include <thirdparty/qscintilla/Qt4Qt5/Qsci/qsciscintillabase.h>

#include <plugins/toolPluginInterface/systemEvents.h>

#include <dialogs/projectManagement/suggestToCreateProjectDialog.h>
#include <dialogs/progressDialog/progressDialog.h>

#include <models/models.h>
#include <models/commands/createGroupCommand.h>
#include <models/commands/multipleRemoveCommand.h>
#include <models/commands/removeElementCommand.h>

#include <editor/editorView.h>
#include <editor/sceneCustomizer.h>
#include <editor/element.h>

#include <hotKeyManager/hotKeyManager.h>
#include <hotKeyManager/hotKeyManagerPage.h>

#include <brandManager/brandManager.h>

#include <mouseGestures/gesturesWidget.h>

#include <textEditor/textManager.h>
#include <textEditor/qscintillaTextEdit.h>

#include "qrealApplication.h"
#include "errorReporter.h"
#include "shapeEdit/shapeEdit.h"
#include "startWidget/startWidget.h"
#include "referenceList.h"
#include "splashScreen.h"
#include "dotRunner.h"

using namespace qReal;
using namespace qReal::commands;
using namespace gui;

MainWindow::MainWindow(const QString &fileToOpen)
	: mUi(new Ui::MainWindowUi)
	, mController(new Controller)
	, mPropertyModel(mFacade.editorManager())
	, mTextManager(new text::TextManager(mFacade.events(), *this))
	, mRootIndex(QModelIndex())
	, mErrorReporter(nullptr)
	, mIsFullscreen(false)
	, mPreferencesDialog(this)
	, mRecentProjectsLimit(SettingsManager::value("recentProjectsLimit").toInt())
	, mRecentProjectsMapper(new QSignalMapper())
	, mProjectManager(new ProjectManagerWrapper(this, mTextManager))
	, mStartWidget(nullptr)
	, mSceneCustomizer(new SceneCustomizer)
	, mInitialFileToOpen(fileToOpen)
{
	mUi->setupUi(this);
	mUi->paletteTree->initMainWindow(this);
	setWindowTitle("QReal");
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

	mErrorReporter = new gui::ErrorReporter(mUi->errorListWidget, mUi->errorDock);
	mErrorReporter->updateVisibility(SettingsManager::value("warningWindow").toBool());
	mUi->errorDock->toggleViewAction()->setShortcut(QKeySequence(Qt::ALT + Qt::Key_1));
	addAction(mUi->errorDock->toggleViewAction());

	mPreferencesDialog.init();


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

	mFindReplaceDialog = new FindReplaceDialog(models().logicalRepoApi(), this);
	mFindHelper = new FindManager(models().repoControlApi()
			, models().mutableLogicalRepoApi(), this, mFindReplaceDialog);
	connectActions();
	connectSystemEvents();
	initExplorers();

	// So now we are going to load plugins. The problem is that if we will do it
	// here then we have some problems with correct main window initialization
	// beacuse of total event loop blocking by plugins. So waiting for main
	// window initialization complete and then loading plugins.
	QTimer::singleShot(50, this, SLOT(initPluginsAndStartWidget()));
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

	connect(mUi->actionNewProject, SIGNAL(triggered()), this, SLOT(createProject()));
	connect(mUi->actionNew_Diagram, SIGNAL(triggered()), mProjectManager, SLOT(suggestToCreateDiagram()));

	connect(mUi->logicalModelExplorer, &ModelExplorer::elementRemoved
			, this, &MainWindow::deleteFromLogicalExplorer);
	connect(mUi->graphicalModelExplorer, &ModelExplorer::elementRemoved
			, this, &MainWindow::deleteFromGraphicalExplorer);

	connect(mUi->actionUndo, SIGNAL(triggered()), mController, SLOT(undo()));
	connect(mUi->actionRedo, SIGNAL(triggered()), mController, SLOT(redo()));

	connect(mUi->actionPreferences, SIGNAL(triggered()), this, SLOT(showPreferencesDialog()));

	connect(mUi->actionShow_grid, SIGNAL(toggled(bool)), this, SLOT(showGrid(bool)));
	connect(mUi->actionShow_alignment, SIGNAL(toggled(bool)), this, SLOT(showAlignment(bool)));
	connect(mUi->actionSwitch_on_grid, SIGNAL(toggled(bool)), this, SLOT(switchGrid(bool)));
	connect(mUi->actionSwitch_on_alignment, SIGNAL(toggled(bool)), this, SLOT(switchAlignment(bool)));
	SettingsListener::listen("ShowGrid", mUi->actionShow_grid, &QAction::setChecked);
	SettingsListener::listen("ShowAlignment", mUi->actionShow_alignment, &QAction::setChecked);
	SettingsListener::listen("ActivateGrid", mUi->actionSwitch_on_grid, &QAction::setChecked);
	SettingsListener::listen("ActivateAlignment", mUi->actionSwitch_on_alignment, &QAction::setChecked);

	mUi->actionShow_all_text->setChecked(!qReal::SettingsManager::value("hideNonHardLabels").toBool());
	connect(mUi->actionShow_all_text, &QAction::triggered
			, [](bool checked) { SettingsManager::setValue("hideNonHardLabels", !checked); });

	connect(mUi->actionHelp, SIGNAL(triggered()), this, SLOT(showHelp()));
	connect(mUi->actionAbout, SIGNAL(triggered()), this, SLOT(showAbout()));
	connect(mUi->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

	connect(mUi->actionGesturesShow, SIGNAL(triggered()), this, SLOT(showGestures()));

	connect(mUi->actionFullscreen, SIGNAL(triggered()), this, SLOT(fullscreen()));

	connect(mUi->actionFind, SIGNAL(triggered()), this, SLOT(showFindDialog()));

	connect(mFindReplaceDialog, SIGNAL(replaceClicked(QStringList&))
			, mFindHelper, SLOT(handleReplaceDialog(QStringList&)));
	connect(mFindReplaceDialog, SIGNAL(findModelByName(QStringList))
			, mFindHelper, SLOT(handleFindDialog(QStringList)));
	connect(mFindReplaceDialog, SIGNAL(chosenElement(qReal::Id)), mFindHelper, SLOT(handleRefsDialog(qReal::Id)));

	SettingsListener::listen("PaletteRepresentation", this, &MainWindow::changePaletteRepresentation);
	SettingsListener::listen("PaletteIconsInARowCount", this, &MainWindow::changePaletteRepresentation);
	SettingsListener::listen("toolbarSize", this, &MainWindow::resetToolbarSize);
	SettingsListener::listen("pathToImages", this, &MainWindow::updatePaletteIcons);
	connect(&mPreferencesDialog, &PreferencesDialog::settingsApplied, this, &MainWindow::applySettings);

	connect(mController, SIGNAL(canUndoChanged(bool)), mUi->actionUndo, SLOT(setEnabled(bool)));
	connect(mController, SIGNAL(canRedoChanged(bool)), mUi->actionRedo, SLOT(setEnabled(bool)));
	connect(mController, SIGNAL(modifiedChanged(bool)), mProjectManager, SLOT(setUnsavedIndicator(bool)));

	connect(mUi->tabs, &QTabWidget::currentChanged, this, &MainWindow::changeWindowTitle);
	connect(mTextManager, SIGNAL(textChanged(bool)), this, SLOT(setTextChanged(bool)));

	connect(mProjectManager, &ProjectManager::afterOpen, mUi->paletteTree, &PaletteTree::refreshUserPalettes);
	connect(mProjectManager, &ProjectManager::closed, mUi->paletteTree, &PaletteTree::refreshUserPalettes);
	connect(mProjectManager, SIGNAL(closed()), mController, SLOT(projectClosed()));

	connect(mUi->propertyEditor, &PropertyEditorView::shapeEditorRequested, this, static_cast<void (MainWindow::*)
			(const QPersistentModelIndex &, int, const QString &, bool)>(&MainWindow::openShapeEditor));
	connect(mUi->propertyEditor, &PropertyEditorView::textEditorRequested, this, &MainWindow::openQscintillaTextEditor);
	connect(mUi->propertyEditor, &PropertyEditorView::referenceListRequested, this, &MainWindow::openReferenceList);

	connect(mUi->menuPanels, &QMenu::aboutToShow, [=]() {
		mUi->menuPanels->clear();
		mUi->menuPanels->addActions(createPopupMenu()->actions());
	});

	addAction(mUi->actionHideBottomDocks);
	connect(mUi->actionHideBottomDocks, &QAction::triggered, this, &MainWindow::hideBottomDocks);

	setDefaultShortcuts();
}

void MainWindow::connectSystemEvents()
{
	connect(mErrorReporter, &ErrorReporter::informationAdded, &mFacade.events(), &SystemEvents::informationAdded);
	connect(mErrorReporter, &ErrorReporter::warningAdded, &mFacade.events(), &SystemEvents::warningAdded);
	connect(mErrorReporter, &ErrorReporter::errorAdded, &mFacade.events(), &SystemEvents::errorAdded);
	connect(mErrorReporter, &ErrorReporter::criticalAdded, &mFacade.events(), &SystemEvents::criticalAdded);

	connect(static_cast<QRealApplication *>(qApp), &QRealApplication::lowLevelEvent
			, &mFacade.events(), &SystemEvents::lowLevelEvent);
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

MainWindow::~MainWindow()
{
	delete mErrorReporter;
	mUi->paletteTree->saveConfiguration();
	SettingsManager::instance()->saveData();
	delete mRecentProjectsMenu;
	delete mRecentProjectsMapper;
	delete mController;
	delete mFindReplaceDialog;
	delete mFindHelper;
	delete mProjectManager;
	delete mSceneCustomizer;
	delete mTextManager;
}

EditorManagerInterface &MainWindow::editorManager()
{
	return mFacade.editorManager();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	if (!mProjectManager->suggestToSaveChangesOrCancel()) {
		event->ignore();
		return;
	}
	mProjectManager->close();

	SettingsManager::setValue("maximized", isMaximized());
	SettingsManager::setValue("size", size());
	SettingsManager::setValue("pos", pos());

	QLOG_INFO() << "Closing main window...";
	emit mFacade.events().closedMainWindow();
}

void MainWindow::loadPlugins()
{
	mUi->paletteTree->loadPalette(SettingsManager::value("PaletteRepresentation").toBool()
			, SettingsManager::value("PaletteIconsInARowCount").toInt()
			, &editorManager());
	SettingsManager::setValue("EditorsLoadedCount", editorManager().editors().count());
}

void MainWindow::clearSelectionOnTabs()
{
	for (int i = 0; i < mUi->tabs->count(); i++) {
		const EditorView * const tab = dynamic_cast<const EditorView *>(mUi->tabs->widget(i));
		if (tab != nullptr) {
			tab->scene()->clearSelection();
		}
	}
}

void MainWindow::addEditorElementsToPalette(const Id &editor, const Id &diagram)
{
	mUi->paletteTree->addEditorElements(editorManager(), editor, diagram);
}

void MainWindow::adjustMinimapZoom(int zoom)
{
	mUi->minimapView->resetMatrix();
	mUi->minimapView->scale(0.01 * zoom, 0.01 * zoom);
}

void MainWindow::selectItemWithError(const Id &id)
{
	if (id == Id::rootId() || id.isNull()) {
		return;
	}

	Id graphicalId = id;
	if (!models().graphicalModelAssistApi().isGraphicalId(id)) {
		const IdList graphicalIds = models().graphicalModelAssistApi().graphicalIdsByLogicalId(id);
		graphicalId = graphicalIds.isEmpty() ? Id() : graphicalIds.at(0);
	}

	selectItemOrDiagram(graphicalId);
	setIndexesOfPropertyEditor(graphicalId);
	centerOn(graphicalId);

	Element * const element = getCurrentTab() ? getCurrentTab()->editorViewScene().getElem(graphicalId) : nullptr;
	graphicsUtils::AnimatedHighlighter::highlight(element);
}

void MainWindow::selectItem(const Id &id)
{
	if (id == Id::rootId()) {
		return;
	}

	setIndexesOfPropertyEditor(id);
	centerOn(id);
}

void MainWindow::selectItemOrDiagram(const Id &graphicalId)
{
	activateItemOrDiagram(graphicalId, true);
}

void MainWindow::activateItemOrDiagram(const QModelIndex &idx, bool setSelected)
{
	const QModelIndex parent = idx.parent();
	const int numTab = getTabIndex(idx);

	if (numTab != -1) {
		mUi->tabs->setCurrentIndex(numTab);
		const Id currentTabId = getCurrentTab()->editorViewScene().rootItemId();
		mToolManager.activeTabChanged(TabInfo(currentTabId, getCurrentTab()));
	} else {
		openNewTab(idx);
	}

	if (mUi->tabs->isEnabled()) {
		if (parent == models().graphicalModelAssistApi().rootIndex()) {
			getCurrentTab()->mutableMvIface().setRootIndex(idx);
		} else {
			getCurrentTab()->mutableMvIface().setRootIndex(parent);
			// select this item on diagram
			getCurrentTab()->scene()->clearSelection();
			const EditorViewScene *scene = static_cast<const EditorViewScene *>(getCurrentTab()->scene());
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

void MainWindow::activateItemOrDiagram(const Id &id, bool setSelected)
{
	if (id.isNull()) {
		return;
	}

	if (models().graphicalModelAssistApi().isGraphicalId(id)) {
		activateItemOrDiagram(models().graphicalModelAssistApi().indexById(id), setSelected);
		return;
	}

	// id is logical ID
	const IdList graphicalIds = models().graphicalModelAssistApi().graphicalIdsByLogicalId(id);
	if (graphicalIds.count()) {
		activateItemOrDiagram(models().graphicalModelAssistApi().indexById(graphicalIds[0]), setSelected);
	}
}

void MainWindow::sceneSelectionChanged(const QList<Element *> &elements)
{
	if (!getCurrentTab()) {
		return;
	}

	if (elements.isEmpty()) {
		mUi->graphicalModelExplorer->setCurrentIndex(QModelIndex());
		mPropertyModel.clearModelIndexes();
	} else if (elements.length() == 1) {
		Element * const singleSelected = elements.at(0);
		setIndexesOfPropertyEditor(singleSelected->id());

		const QModelIndex index = models().graphicalModelAssistApi().indexById(singleSelected->id());
		if (index.isValid()) {
			mUi->graphicalModelExplorer->setCurrentIndex(index);
		}
	}
}

void MainWindow::refreshRecentProjectsList(const QString &fileName)
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
	const QString stringList = SettingsManager::value("recentProjects").toString();
	const QStringList recentProjects = stringList.split(";", QString::SkipEmptyParts);
	foreach (QString projectPath, recentProjects) {
		mRecentProjectsMenu->addAction(projectPath);
		QObject::connect(mRecentProjectsMenu->actions().last(), SIGNAL(triggered())
				, mRecentProjectsMapper, SLOT(map()));
		mRecentProjectsMapper->setMapping(mRecentProjectsMenu->actions().last(), projectPath);
	}

	QObject::connect(mRecentProjectsMapper, SIGNAL(mapped(const QString &))
			, mProjectManager, SLOT(openExisting(const QString &)));
}

void MainWindow::closeAllTabs()
{
	const int tabCount = mUi->tabs->count();

	for (int i = 0; i < tabCount; i++) {
		closeTab(0);
	}
}

void MainWindow::setReference(const QStringList &data, const QPersistentModelIndex &index, const int &role)
{
	removeOldBackReference(index, role);
	setData(data.join(','), index, role);
	foreach (const QString &target, data) {
		if (!target.isEmpty()) {
			setBackReference(index, target);
		}
	}
}

void MainWindow::setData(const QString &data, const QPersistentModelIndex &index, const int &role)
{
	// const_cast here is ok, since we need to set data in a correct model, and
	// not going to use this index anymore.
	QAbstractItemModel * const model = const_cast<QAbstractItemModel *>(index.model());
	model->setData(index, data, role);
}

void MainWindow::print()
{
	const bool isEditorTab = getCurrentTab() != nullptr;

	if (isEditorTab) {
		QPrinter printer(QPrinter::HighResolution);
		QPrintDialog dialog(&printer, this);
		if (dialog.exec() == QDialog::Accepted) {
			QPainter painter(&printer);
			getCurrentTab()->scene()->render(&painter);
		}
	} else {
		QsciScintillaBase *textTab = static_cast<QsciScintillaBase *>(currentTab());
		QsciPrinter printer(QPrinter::HighResolution);
		QPrintDialog dialog(&printer, this);
		if (dialog.exec() == QDialog::Accepted) {
			printer.printRange(textTab);
		}
	}
}

void MainWindow::makeSvg()
{
	QSvgGenerator newSvg;

	QString fileName = utils::QRealFileDialog::getSaveFileName("SaveDiagramAsSvg", this);
	if (fileName.isEmpty()) {
		return;
	}

	newSvg.setFileName(fileName);
	newSvg.setSize(QSize(800, 600));

	QPainter painter(&newSvg);
	getCurrentTab()->scene()->render(&painter);
}

void MainWindow::deleteElementFromDiagram(const Id &id)
{
	const bool isLogical = models().logicalModelAssistApi().isLogicalId(id);
	if (isLogical) {
		mUi->logicalModelExplorer->setCurrentIndex(models().logicalModelAssistApi().indexById(id));
		deleteFromLogicalExplorer();
	} else {
		mUi->graphicalModelExplorer->setCurrentIndex(models().graphicalModelAssistApi().indexById(id));
		deleteFromGraphicalExplorer();
	}
}

void MainWindow::reportOperation(invocation::LongOperation *operation)
{
	ProgressDialog *progressDialog = new ProgressDialog(this);
	progressDialog->connectOperation(operation);
}

QWidget *MainWindow::currentTab()
{
	return mUi->tabs->currentWidget();
}

void MainWindow::openTab(QWidget *tab, const QString &title)
{
	if (mUi->tabs->indexOf(tab) == -1) {
		mUi->tabs->addTab(tab, title);
	}

	mUi->tabs->setCurrentWidget(tab);
}

void MainWindow::closeTab(QWidget *tab)
{
	mUi->tabs->removeTab(mUi->tabs->indexOf(tab));
}

QMap<QString, gui::PreferencesPage *> MainWindow::preferencesPages() const
{
	QMap<QString, PreferencesPage *> result;
	for (PreferencesPage * const page : mPreferencesDialog.pages()) {
		result[page->objectName()] = page;
	}

	return result;
}

void MainWindow::closeStartTab()
{
	for (int i = 0; i < mUi->tabs->count(); ++i) {
		const StartWidget * widget = dynamic_cast<StartWidget *>(mUi->tabs->widget(i));
		if (widget) {
			mUi->tabs->removeTab(i);
		}
	}
}

void MainWindow::closeDiagramTab(const Id &id)
{
	const IdList graphicalIds = models().graphicalRepoApi().graphicalElements(id.type());
	if (!graphicalIds.isEmpty()) {
		// TODO: Why only for first graphical element?
		const QModelIndex index = models().graphicalModelAssistApi().indexById(graphicalIds[0]);
		for (int i = 0; i < mUi->tabs->count(); i++) {
			const EditorView * const tab = dynamic_cast<const EditorView *>(mUi->tabs->widget(i));
			if (tab != nullptr && tab->mvIface().rootIndex() == index) {
				mUi->tabs->removeTab(i);
			}
		}
	}
}

void MainWindow::deleteFromLogicalExplorer()
{
	const QModelIndex index = mUi->logicalModelExplorer->currentIndex();
	if (index.isValid()) {
		/// @todo: rewrite it with just MultipleRemoveCommand.
		MultipleRemoveCommand factory(models());
		mController->executeGlobal(factory.logicalDeleteCommand(index));
	}
}

void MainWindow::deleteFromGraphicalExplorer()
{
	const Id id = models().graphicalModelAssistApi().idByIndex(mUi->graphicalModelExplorer->currentIndex());
	if (!id.isNull()) {
		MultipleRemoveCommand * const command = new MultipleRemoveCommand(models());
		command->setItemsToDelete(IdList() << id);
		mController->executeGlobal(command);
	}
}

void MainWindow::changeWindowTitle()
{
	const QString windowTitle = mToolManager.customizer()->windowTitle();

	text::QScintillaTextEdit *area = dynamic_cast<text::QScintillaTextEdit *>(currentTab());
	if (area) {
		const QString filePath = mTextManager->path(area);
		setWindowTitle(windowTitle + " " + filePath);
	} else if (getCurrentTab()) {
		mProjectManager->refreshWindowTitleAccordingToSaveFile();
	} else {
		setWindowTitle(windowTitle);
	}
}

void MainWindow::setTextChanged(bool changed)
{
	text::QScintillaTextEdit *area = static_cast<text::QScintillaTextEdit *>(currentTab());
	const QString windowTitle = mToolManager.customizer()->windowTitle();
	const QString filePath = mTextManager->path(area);
	const QString chIndicator = changed ? "*" : "";
	setWindowTitle(windowTitle + " " + chIndicator + filePath);
	const int index = mUi->tabs->currentIndex();
	mUi->tabs->setTabText(index, mUi->tabs->tabText(index).remove(QChar('*'), Qt::CaseInsensitive) + chIndicator);
}

void MainWindow::removeReferences(const Id &id)
{
	models().logicalModelAssistApi().removeReferencesTo(id);
	models().logicalModelAssistApi().removeReferencesFrom(id);
}

void MainWindow::showAbout()
{
	QMessageBox::about(this, tr("About QReal"), mToolManager.customizer()->aboutText());
}

void MainWindow::showHelp()
{
	const QString url = QString("file:///%1/help/index.html").arg(QApplication::applicationDirPath());
	QDesktopServices::openUrl(QUrl(url));
}

void MainWindow::toggleShowSplash(bool show)
{
	SettingsManager::setValue("Splashscreen", show);
}

bool MainWindow::unloadPlugin(const QString &pluginName)
{
	if (editorManager().editors().contains(Id(pluginName))) {
		const IdList diagrams = editorManager().diagrams(Id(pluginName));

		const QString error = editorManager().unloadPlugin(pluginName);
		if (!error.isEmpty()) {
			QMessageBox::warning(this, tr("Error"), tr("Plugin unloading failed: ") + error);
			return false;
		}

		foreach (const Id &diagram, diagrams) {
			mUi->paletteTree->deleteEditor(diagram);
		}
	}
	return true;
}

bool MainWindow::loadPlugin(const QString &fileName, const QString &pluginName)
{
	const QString error = editorManager().loadPlugin(fileName);
	if (!error.isEmpty()) {
		QMessageBox::warning(this, tr("Error"), tr("Plugin loading failed: ") + error);
		return false;
	}

	foreach (const Id &diagram, editorManager().diagrams(Id(pluginName))) {
		mUi->paletteTree->addEditorElements(editorManager(), Id(pluginName), diagram);
	}

	mUi->paletteTree->initDone();
	return true;
}

bool MainWindow::pluginLoaded(const QString &pluginName)
{
	return editorManager().editors().contains(Id(pluginName));
}

EditorView * MainWindow::getCurrentTab() const
{
	return dynamic_cast<EditorView *>(mUi->tabs->currentWidget());
}

bool MainWindow::isCurrentTabShapeEdit() const
{
	return dynamic_cast<ShapeEdit *>(mUi->tabs->currentWidget()) != nullptr;
}

void MainWindow::closeCurrentTab()
{
	if (mUi->tabs->currentIndex() >= 0) {
		closeTab(mUi->tabs->currentIndex());
	}
}

void MainWindow::closeTab(int index)
{
	QWidget * const widget = mUi->tabs->widget(index);
	EditorView * const diagram = dynamic_cast<EditorView *>(widget);
	text::QScintillaTextEdit * const possibleCodeTab = dynamic_cast<text::QScintillaTextEdit *>(widget);

	const QString path = mTextManager->path(possibleCodeTab);

	if (diagram) {
		const Id diagramId = diagram->editorViewScene().rootItemId();
		mController->diagramClosed(diagramId);
		emit mFacade.events().diagramClosed(diagramId);
	} else if (mTextManager->unbindCode(possibleCodeTab)) {
		emit mFacade.events().codeTabClosed(QFileInfo(path));
	} else {
		// TODO: process other tabs (for example, start tab)
	}

	mUi->tabs->removeTab(index);
	delete widget;
}

void MainWindow::showPreferencesDialog()
{
	if (mPreferencesDialog.exec() == QDialog::Accepted) {
		mToolManager.updateSettings();
	}

	mProjectManager->reinitAutosaver();
}

void MainWindow::openSettingsDialog(const QString &tab)
{
	mPreferencesDialog.switchCurrentPage(tab);
	showPreferencesDialog();
}

// TODO: Unify overloads.
void MainWindow::openShapeEditor(
		const QPersistentModelIndex &index
		, int role
		, const QString &propertyValue
		, bool useTypedPorts
		)
{
	ShapeEdit *shapeEdit = new ShapeEdit(dynamic_cast<models::details::LogicalModel *>(models().logicalModel())
			, index, role, useTypedPorts);
	if (!propertyValue.isEmpty()) {
		shapeEdit->load(propertyValue);
	}

	// Here we are going to actually modify model to set a value of a shape.
	QAbstractItemModel *model = const_cast<QAbstractItemModel *>(index.model());
	model->setData(index, propertyValue, role);
	connect(shapeEdit, SIGNAL(shapeSaved(QString, const QPersistentModelIndex &, const int &))
			, this, SLOT(setData(QString, const QPersistentModelIndex &, const int &)));

	mUi->tabs->addTab(shapeEdit, tr("Shape Editor"));
	mUi->tabs->setCurrentWidget(shapeEdit);
	setConnectActionZoomTo(shapeEdit);
}

// This method is for Interpreter.
void MainWindow::openShapeEditor(const Id &id
		, const QString &propertyValue
		, const EditorManagerInterface *editorManagerProxy
		, bool useTypedPorts)
{
	ShapeEdit *shapeEdit = new ShapeEdit(id, *editorManagerProxy, models().graphicalRepoApi(), this, getCurrentTab()
		, useTypedPorts);
	if (!propertyValue.isEmpty()) {
		shapeEdit->load(propertyValue);
	}

	mUi->tabs->addTab(shapeEdit, tr("Shape Editor"));
	mUi->tabs->setCurrentWidget(shapeEdit);
	setConnectActionZoomTo(shapeEdit);
}

void MainWindow::openQscintillaTextEditor(const QPersistentModelIndex &index, const int role
		, const QString &propertyValue)
{
	text::QScintillaTextEdit *textEdit = new text::QScintillaTextEdit(index, role);
	textEdit->setCurrentLanguage(text::Languages::python());

	if (!propertyValue.isEmpty()) {
		textEdit->setText(propertyValue.toUtf8());
	}

	connect(textEdit, SIGNAL(textSaved(const QString &, const QPersistentModelIndex &, const int &))
			, this, SLOT(setData(const QString &, const QPersistentModelIndex &, const int &)));

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

void MainWindow::openReferenceList(const QPersistentModelIndex &index
		, const QString &referenceType,	const QString &propertyValue, int role)
{
	ReferenceList referenceList(this, index, referenceType, propertyValue.split(',', QString::SkipEmptyParts), role);
	connect(&referenceList, SIGNAL(referenceSet(QStringList, QPersistentModelIndex, int))
			, this, SLOT(setReference(QStringList, QPersistentModelIndex, int)));
	referenceList.exec();
}

void MainWindow::disconnectZoom(QGraphicsView *view)
{
	disconnect(mUi->actionZoom_In, SIGNAL(triggered()), view, SLOT(zoomIn()));
	disconnect(mUi->actionZoom_Out, SIGNAL(triggered()), view, SLOT(zoomOut()));
}

void MainWindow::connectZoom(QGraphicsView *view)
{
	connect(mUi->actionZoom_In, SIGNAL(triggered()), view, SLOT(zoomIn()));
	connect(mUi->actionZoom_Out, SIGNAL(triggered()), view, SLOT(zoomOut()));
}

void MainWindow::disconnectActionZoomTo(QWidget* widget)
{
	EditorView * const view = dynamic_cast<EditorView *>(widget);
	if (view != nullptr) {
		disconnectZoom(view);
	} else {
		ShapeEdit *const shapeWidget = dynamic_cast<ShapeEdit *>(widget);
		if (shapeWidget != nullptr) {
			disconnectZoom(shapeWidget->getView());
		}
	}
}

void MainWindow::connectActionZoomTo(QWidget* widget)
{
	EditorView * const view = (dynamic_cast<EditorView *>(widget));
	if (view != nullptr) {
		connectZoom(view);
	} else {
		ShapeEdit * const shapeWidget = (dynamic_cast<ShapeEdit *>(widget));
		if (shapeWidget != nullptr) {
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

void MainWindow::centerOn(const Id &id)
{
	if (id.isNull() || editorManager().isDiagramNode(id)) {
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

void MainWindow::propertyEditorScrollTo(const QModelIndex &index)
{
	mUi->propertyEditor->scrollTo(index);
}

void MainWindow::graphicalModelExplorerClicked(const QModelIndex &index)
{
	const Id id = models().graphicalModelAssistApi().idByIndex(index);
	setIndexesOfPropertyEditor(id);
	openNewTab(index);
	centerOn(id);
}

void MainWindow::logicalModelExplorerClicked(const QModelIndex &index)
{
	const Id logicalId = models().logicalModelAssistApi().idByIndex(index);
	IdList graphicalIds = models().graphicalModelAssistApi().graphicalIdsByLogicalId(logicalId);
	if (!graphicalIds.empty()) {
		// By now we will select first graphical representation of selected element.
		// In the future it may be needed to make this more intellectual, like
		// selecting the representation in current tab.
		const Id graphicalId = graphicalIds.first();
		const QModelIndex graphicalIndex = models().graphicalModelAssistApi().indexById(graphicalId);
		graphicalModelExplorerClicked(graphicalIndex);
	} else {
		setIndexesOfPropertyEditor(logicalId);
		EditorView* const view = getCurrentTab();
		if (view != nullptr) {
			EditorViewScene* const scene = dynamic_cast<EditorViewScene*>(view->scene());
			scene->clearSelection();
		}
	}
}

void MainWindow::openNewTab(const QModelIndex &arg)
{
	QModelIndex index = arg;
	while (index.parent() != QModelIndex()) {
		index = index.parent();
	}

	int tabNumber = -1;
	for (int i = 0; i < mUi->tabs->count(); i++) {
		EditorView *tab = (dynamic_cast<EditorView *>(mUi->tabs->widget(i)));
		if (tab != nullptr && tab->mvIface().rootIndex() == index) {
			tabNumber = i;
			break;
		}
	}
	if (tabNumber != -1) {
		mUi->tabs->setCurrentIndex(tabNumber);
	} else {
		const Id diagramId = models().graphicalModelAssistApi().idByIndex(index);
		EditorView * const view = new EditorView(models(), *controller(), *mSceneCustomizer, diagramId, this);
		view->mutableScene().enableMouseGestures(qReal::SettingsManager::value("gesturesEnabled").toBool());
		SettingsListener::listen("gesturesEnabled", &(view->mutableScene()) ,&EditorViewScene::enableMouseGestures);
		SettingsListener::listen("gesturesEnabled", mUi->actionGesturesShow ,&QAction::setEnabled);
		mController->diagramOpened(diagramId);
		initCurrentTab(view, index);
		mUi->tabs->addTab(view, index.data().toString());
		mUi->tabs->setCurrentWidget(view);

		// Focusing on scene top left corner
		view->centerOn(view->scene()->sceneRect().topLeft());
	}

	// changing of palette active editor
	if (SettingsManager::value("PaletteTabSwitching").toBool()) {
		int i = 0;
		foreach (const QString &name, mUi->paletteTree->editorsNames()) {
			const Id id = models().graphicalModelAssistApi().idByIndex(index);
			const Id diagramId = Id(id.editor(), id.diagram());
			const QString diagramName = editorManager().friendlyName(diagramId);
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
	const Id rootId = models().graphicalModelAssistApi().rootId();
	const IdList rootIds = models().graphicalModelAssistApi().children(rootId);
	if (rootIds.count() == 0) {
		return;
	}

	for (const Id &id : rootIds) {
		if (models().graphicalRepoApi().isGraphicalElement(id)) {
			openNewTab(models().graphicalModelAssistApi().indexById(id));
			break;
		}
	}
}

void MainWindow::initCurrentTab(EditorView *const tab, const QModelIndex &rootIndex)
{
	if (!tab) {
		return;
	}

	const QModelIndex index = rootIndex;

	tab->mutableMvIface().configure(models().graphicalModelAssistApi()
			, models().logicalModelAssistApi(), models().exploser());

	tab->mutableMvIface().setModel(models().graphicalModel());
	if (tab->sceneRect() == QRectF(0, 0, 0, 0)) {
		tab->setSceneRect(0, 0, 1, 1);
	}

	tab->mutableMvIface().setLogicalModel(models().logicalModel());
	tab->mutableMvIface().setRootIndex(index);

	// Connect after setModel etc. because of signal selectionChanged was sent when there were old indexes
	connect(&tab->editorViewScene(), &EditorViewScene::sceneSelectionChanged, this, &MainWindow::sceneSelectionChanged);
	connect(mUi->actionAntialiasing, SIGNAL(toggled(bool)), tab, SLOT(toggleAntialiasing(bool)));
	connect(models().graphicalModel(), SIGNAL(rowsAboutToBeMoved(QModelIndex, int, int, QModelIndex, int))
			, &tab->mvIface(), SLOT(rowsAboutToBeMoved(QModelIndex, int, int, QModelIndex, int)));
	connect(models().graphicalModel(), SIGNAL(rowsMoved(QModelIndex, int, int, QModelIndex, int))
			, &tab->mvIface(), SLOT(rowsMoved(QModelIndex, int, int, QModelIndex, int)));
	connect(tab, &EditorView::rootElementRemoved, this
			, static_cast<bool (MainWindow::*)(const QModelIndex &)>(&MainWindow::closeTab));
	connect(&tab->editorViewScene(), &EditorViewScene::goTo, [=](const Id &id) { activateItemOrDiagram(id); });
	connect(&tab->editorViewScene(), &EditorViewScene::refreshPalette, this, &MainWindow::loadPlugins);
	connect(&tab->editorViewScene(), &EditorViewScene::openShapeEditor, this, static_cast<void (MainWindow::*)
			(const Id &, const QString &, const EditorManagerInterface *, bool)>(&MainWindow::openShapeEditor));

	setShortcuts(tab);

	tab->mutableScene().initNodes();
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
	HotKeyManager::setCommand("File.NewDiagram", tr("New diagram"), mUi->actionNew_Diagram);

	// TODO: bind Ctrl+P to print when it will be repaired
	// TODO: bind Ctrl+F to find dialog when it will be repaired

	HotKeyManager::setCommand("File.Open", tr("Open project"), mUi->actionOpen);
	HotKeyManager::setCommand("File.Save", tr("Save project"), mUi->actionSave);
	HotKeyManager::setCommand("File.SaveAs", tr("Save project as"), mUi->actionSave_as);
	HotKeyManager::setCommand("File.NewProject", tr("New project"), mUi->actionNewProject);
	HotKeyManager::setCommand("Editor.Undo", tr("Undo"), mUi->actionUndo);
	HotKeyManager::setCommand("Editor.Redo", tr("Redo"), mUi->actionRedo);
	HotKeyManager::setCommand("Editor.ZoomIn", tr("Zoom In"), mUi->actionZoom_In);
	HotKeyManager::setCommand("Editor.ZoomOut", tr("Zoom Out"), mUi->actionZoom_Out);
	HotKeyManager::setCommand("Editor.CloseCurrentTab", tr("Close current tab"), closeCurrentTabAction);
	HotKeyManager::setCommand("Editor.CloseAllTabs", tr("Close all tabs"), closeAllTabsAction);
	HotKeyManager::setCommand("Editor.Print", tr("Print"), mUi->actionPrint);
	HotKeyManager::setCommand("Editor.Find", tr("Find"), mUi->actionFind);
	HotKeyManager::setCommand("Editor.ToggleTitles", tr("Show all text"), mUi->actionShow_all_text);
	HotKeyManager::setCommand("View.ToggleErrorReporter", tr("Toggle errors panel")
			, mUi->errorDock->toggleViewAction());
}

void MainWindow::currentTabChanged(int newIndex)
{
	switchToTab(newIndex);
	mUi->minimapView->changeSource(newIndex);

	const bool isEditorTab = getCurrentTab();
	const bool isShape = isCurrentTabShapeEdit();
	const bool isStartTab = dynamic_cast<StartWidget *>(mUi->tabs->widget(newIndex));
	const bool isGesturesTab = dynamic_cast<gestures::GesturesWidget *>(mUi->tabs->widget(newIndex));
	const bool isDecorativeTab = isStartTab || isGesturesTab;

	mUi->actionSave->setEnabled(!isDecorativeTab);
	mUi->actionSave_as->setEnabled(!isDecorativeTab);
	mUi->actionSave_diagram_as_a_picture->setEnabled(isEditorTab);
	mUi->actionPrint->setEnabled(!isDecorativeTab);

	mUi->actionRedo->setEnabled(mController->canRedo() && !isShape && !isDecorativeTab);
	mUi->actionUndo->setEnabled(mController->canUndo() && !isShape && !isDecorativeTab);
	mUi->actionFind->setEnabled(!isDecorativeTab);

	mUi->actionZoom_In->setEnabled(isEditorTab || isShape);
	mUi->actionZoom_Out->setEnabled(isEditorTab || isShape);

	mUi->actionGesturesShow->setEnabled(qReal::SettingsManager::value("gesturesEnabled").toBool());

	if (isEditorTab) {
		const Id currentTabId = getCurrentTab()->mvIface().rootId();
		mToolManager.activeTabChanged(TabInfo(currentTabId, getCurrentTab()));
		mUi->graphicalModelExplorer->changeEditorActionsSet(getCurrentTab()->editorViewScene().editorActions());
		mUi->logicalModelExplorer->changeEditorActionsSet(getCurrentTab()->editorViewScene().editorActions());
	} else if (text::QScintillaTextEdit * const text = dynamic_cast<text::QScintillaTextEdit *>(currentTab())) {
		mToolManager.activeTabChanged(TabInfo(mTextManager->path(text), text));
	} else {
		mToolManager.activeTabChanged(TabInfo(currentTab()));
	}

	emit rootDiagramChanged();
}

void MainWindow::switchToTab(int index)
{
	if (index != -1) {
		mUi->tabs->setEnabled(true);
		EditorView *editorView = getCurrentTab();
		setConnectActionZoomTo(mUi->tabs->currentWidget());

		if (editorView) {
			getCurrentTab()->mutableMvIface().setModel(models().graphicalModel());
			getCurrentTab()->mutableMvIface().setLogicalModel(models().logicalModel());
			mRootIndex = editorView->mvIface().rootIndex();
			const Id diagramId = models().graphicalModelAssistApi().idByIndex(mRootIndex);
			mController->setActiveDiagram(diagramId);
		}
	} else {
		mUi->tabs->setEnabled(false);
		mController->setActiveDiagram(Id());
	}

}

void MainWindow::updateTabName(const Id &id)
{
	for (int i = 0; i < mUi->tabs->count(); i++) {
		EditorView * const tab = (dynamic_cast<EditorView *>(mUi->tabs->widget(i)));
		if (tab && (tab->mvIface().rootIndex() == models().graphicalModelAssistApi().indexById(id))) {
			mUi->tabs->setTabText(i, models().graphicalModelAssistApi().name(id));
			return;
		}
	}
}

bool MainWindow::closeTab(const QModelIndex &graphicsIndex)
{
	for (int i = 0; i < mUi->tabs->count(); i++) {
		EditorView * const tab = (static_cast<EditorView *>(mUi->tabs->widget(i)));
		if (tab->mvIface().rootIndex() == graphicsIndex) {
			closeTab(i);
			return true;
		}
	}
	return false;
}

models::Models &MainWindow::models()
{
	return mFacade.models();
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
		if (tab != nullptr) {
			tab->setDrawSceneGrid(isChecked);
		}
	}
}

void MainWindow::setShowAlignment(bool isChecked)
{
	for (int i = 0; i < mUi->tabs->count(); i++) {
		EditorView * const tab = (dynamic_cast<EditorView *>(mUi->tabs->widget(i)));
		if (tab != nullptr) {
			const QList<QGraphicsItem *> list = tab->scene()->items();
			foreach (QGraphicsItem * const item, list) {
				NodeElement * const nodeItem = dynamic_cast<NodeElement*>(item);
				if (nodeItem != nullptr) {
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
		if (tab != nullptr) {
			const QList<QGraphicsItem *> list = tab->scene()->items();
			foreach (QGraphicsItem *const item, list) {
				NodeElement * const nodeItem = dynamic_cast<NodeElement*>(item);
				if (nodeItem != nullptr) {
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
		if (tab != nullptr) {
			const QList<QGraphicsItem *> list = tab->scene()->items();
			foreach (QGraphicsItem * const item, list) {
				NodeElement * const nodeItem = dynamic_cast<NodeElement*>(item);
				if (nodeItem != nullptr) {
					nodeItem->switchAlignment(isChecked);
				}
			}
		}
	}
}

void MainWindow::showGestures()
{
	if (!getCurrentTab()) {
		return;
	}

	QWidget * const gesturesPainter = getCurrentTab()->mutableScene().gesturesPainterWidget();
	mUi->tabs->addTab(gesturesPainter, tr("Gestures Show"));
	mUi->tabs->setCurrentWidget(gesturesPainter);
}


ProxyEditorManager &MainWindow::editorManagerProxy()
{
	return *static_cast<ProxyEditorManager *>(&editorManager());
}

void MainWindow::createDiagram(const QString &idString)
{
	closeStartTab();
	const Id id = Id::loadFromString(idString);
	Id created;
	if (editorManager().isNodeOrEdge(id.editor(), id.element())) {
		created = models().graphicalModelAssistApi().createElement(Id::rootId(), id);
	} else {
		// It is a group
		CreateGroupCommand createGroupCommand(models().logicalModelAssistApi()
				, models().graphicalModelAssistApi(), models().exploser(), Id::rootId(), Id::rootId()
				, id, false, QPointF());
		createGroupCommand.redo();
		created = createGroupCommand.rootId();
	}

	if (created.isNull()) {
		return;
	}

	const QModelIndex index = models().graphicalModelAssistApi().indexById(created);
	mUi->graphicalModelExplorer->setCurrentIndex(index);
	const Id logicalIdCreated = models().graphicalModelAssistApi().logicalId(created);
	const QModelIndex logicalIndex = models().logicalModelAssistApi().indexById(logicalIdCreated);
	mUi->logicalModelExplorer->setCurrentIndex(logicalIndex);

	openNewTab(index);
}

bool MainWindow::createProject(const QString &diagramIdString)
{
	if (!mProjectManager->openEmptyWithSuggestToSaveChanges()) {
		return false;
	}
	createDiagram(diagramIdString);
	return true;
}

void MainWindow::createProject()
{
	const Id theOnlyDiagram = editorManager().theOnlyDiagram();
	if (theOnlyDiagram.isNull()) {
		SuggestToCreateProjectDialog dialog(editorManager(), this);
		connect(&dialog, &SuggestToCreateProjectDialog::diagramSelected, this
				, static_cast<bool (MainWindow::*)(const QString &)>(&MainWindow::createProject));
		dialog.exec();
	} else {
		const Id editor = editorManager().editors()[0];
		const QString diagramIdString = editorManager().diagramNodeNameString(editor, theOnlyDiagram);
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
		if (index.parent() == editor->mvIface().rootIndex()) {
			return i;
		}
	}
	return -1;
}

void MainWindow::setIndexesOfPropertyEditor(const Id &id)
{
	if (models().graphicalModelAssistApi().isGraphicalId(id)) {
		const Id logicalId = models().graphicalModelAssistApi().logicalId(id);
		const QModelIndex logicalIndex = models().logicalModelAssistApi().indexById(logicalId);
		const QModelIndex graphicalIndex = models().graphicalModelAssistApi().indexById(id);
		mPropertyModel.setModelIndexes(logicalIndex, graphicalIndex);
	} else if (models().logicalModelAssistApi().isLogicalId(id)) {
		const QModelIndex logicalIndex = models().logicalModelAssistApi().indexById(id);
		mPropertyModel.setModelIndexes(logicalIndex, QModelIndex());
	} else {
		mPropertyModel.clearModelIndexes();
	}
}

void MainWindow::highlight(const Id &graphicalId, bool exclusive, const QColor &color)
{
	for (int i = 0; i < mUi->tabs->count(); ++i) {
		EditorView * const view = dynamic_cast<EditorView *>(mUi->tabs->widget(i));
		if (!view) {
			continue;
		}
		EditorViewScene * const scene = dynamic_cast<EditorViewScene *>(view->scene());
		const Element * const element = scene->getElem(graphicalId);
		if (element) {
			scene->highlight(graphicalId, exclusive, color);
			view->ensureElementVisible(element, 0, 0);
		}
	}
}

void MainWindow::dehighlight(const Id &graphicalId)
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

	const Id currentId = mUi->paletteTree->currentEditor();
	loadPlugins();

	mUi->paletteTree->setActiveEditor(currentId);
	mUi->paletteTree->setComboBox(currentId);
}

void MainWindow::applySettings()
{
	for (int i = 0; i < mUi->tabs->count(); i++) {
		EditorView * const tab = dynamic_cast<EditorView *>(mUi->tabs->widget(i));
		EditorViewScene *scene = tab ? dynamic_cast <EditorViewScene *>(tab->scene()) : nullptr;
		if (scene) {
			scene->updateEdgeElements();
			scene->invalidate();
		}
	}
}

void MainWindow::resetToolbarSize(int size)
{
	for (QToolBar * const bar : toolBars()) {
		bar->setIconSize(QSize(size, size));
	}
}

void MainWindow::setBackReference(const QPersistentModelIndex &index, const QString &data)
{
	Id id = Id::loadFromString(data);
	Id indexId = models().logicalModelAssistApi().idByIndex(index);
	models().logicalRepoApi().setBackReference(id, indexId);
}

void MainWindow::removeOldBackReference(const QPersistentModelIndex &index, const int role)
{
	QStringList data = index.data(role).toString().split(',', QString::SkipEmptyParts);

	foreach (const QString &reference, data) {
		Id id = Id::loadFromString(reference);
		Id indexId = models().logicalModelAssistApi().idByIndex(index);
		models().logicalRepoApi().removeBackReference(id, indexId);
	}
}

void MainWindow::hideDockWidget(QDockWidget *dockWidget, const QString &name)
{
	mDocksVisibility[name] = !dockWidget->isHidden();
	if (mDocksVisibility[name]) {
		dockWidget->hide();
	}
}

void MainWindow::showDockWidget(QDockWidget *dockWidget, const QString &name)
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

		mUi->actionFullscreen->setIcon(QIcon(":/mainWindow/images/unFullScreen.svg"));
	} else {
		showDockWidget(mUi->minimapDock, "minimap");
		showDockWidget(mUi->graphicalModelDock, "graphicalModel");
		showDockWidget(mUi->logicalModelDock, "logicalModel");
		showDockWidget(mUi->propertyDock, "propertyEditor");
		showDockWidget(mUi->errorDock, "errorReporter");

		mUi->actionFullscreen->setIcon(QIcon(":/mainWindow/images/fullScreen.svg"));
	}
	for (QDockWidget * const dock : mAdditionalDocks) {
		if (mIsFullscreen) {
			hideDockWidget(dock, dock->windowTitle());
		} else {
			showDockWidget(dock, dock->windowTitle());
		}
	}

	if (mIsFullscreen) {
		mLastTabBarIndexes.clear();
		for (QTabBar * const bar : findChildren<QTabBar *>()) {
			if (bar->objectName().isEmpty()) {  // Else we can modify the central tab widget`s current index
				mLastTabBarIndexes[bar] = bar->currentIndex();
			}
		}
	} else {
		for (QTabBar * const bar : findChildren<QTabBar *>()) {
			if (mLastTabBarIndexes.contains(bar)) {
				bar->setCurrentIndex(mLastTabBarIndexes[bar]);
			}
		}
	}
}

void MainWindow::hideBottomDocks()
{
	for (QDockWidget *dock : findChildren<QDockWidget *>()) {
		if (dockWidgetArea(dock) == Qt::BottomDockWidgetArea) {
			dock->hide();
		}
	}
}

QString MainWindow::getNextDirName(const QString &name)
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

Id MainWindow::activeDiagram() const
{
	return getCurrentTab() ? getCurrentTab()->mvIface().rootId() : Id();
}

void MainWindow::initPluginsAndStartWidget()
{
	initToolPlugins();

	BrandManager::configure(&mToolManager);
	mPreferencesDialog.setWindowIcon(BrandManager::applicationIcon());
	PreferencesPage *hotKeyManagerPage = new PreferencesHotKeyManagerPage(this);
	mPreferencesDialog.registerPage(tr("Shortcuts"), hotKeyManagerPage);

	if (!mProjectManager->restoreIncorrectlyTerminated() &&
			(mInitialFileToOpen.isEmpty() || !mProjectManager->open(mInitialFileToOpen)))
	{
		openStartTab();
	}
}

void MainWindow::addActionOrSubmenu(QMenu *target, const ActionInfo &actionOrMenu)
{
	if (actionOrMenu.isAction()) {
		target->addAction(actionOrMenu.action());
	} else {
		target->addMenu(actionOrMenu.menu());
	}
}

void MainWindow::traverseListOfActions(QList<ActionInfo> const &actions)
{
	for (const ActionInfo &action : actions) {
		if (action.isAction()) {
			QToolBar * const toolbar = findChild<QToolBar *>(action.toolbarName() + "Toolbar");
			if (toolbar) {
				toolbar->addAction(action.action());
				connect(action.action(), &QAction::changed, [toolbar]() {
					for (QAction * const action : toolbar->actions()) {
						if (action->isVisible()) {
							toolbar->setVisible(true);
							return;
						}
					}

					toolbar->hide();
				});
			}
		}
	}

	for (const ActionInfo &action : actions) {
		const QString menuName = "menu" + utils::StringUtils::capitalizeFirstLetter(action.menuName());
		QMenu * const menu = findChild<QMenu *>(menuName);
		if (menu) {
			addActionOrSubmenu(menu, action);
		}
	}
}

QList<QAction *> MainWindow::optionalMenuActionsForInterpretedPlugins()
{
	return mListOfAdditionalActions;
}

void MainWindow::initToolPlugins()
{
	mToolManager.init(PluginConfigurator(models().repoControlApi()
		, models().graphicalModelAssistApi()
		, models().logicalModelAssistApi()
		, *this
		, *this
		, *mProjectManager
		, *mSceneCustomizer
		, mFacade.events()
		, *mTextManager));

	QList<ActionInfo> const actions = mToolManager.actions();
	traverseListOfActions(actions);

	for (const HotKeyActionInfo &actionInfo : mToolManager.hotKeyActions()) {
		HotKeyManager::setCommand(actionInfo.id(), actionInfo.label(), actionInfo.action());
	}

	if (mUi->generatorsToolbar->actions().isEmpty()) {
		mUi->generatorsToolbar->hide();
	}

	if (mUi->interpretersToolbar->actions().isEmpty()) {
		mUi->interpretersToolbar->hide();
	}

	QList<QPair<QString, PreferencesPage *> > const preferencesPages = mToolManager.preferencesPages();
	typedef QPair<QString, PreferencesPage *> PageDescriptor;
	foreach (const PageDescriptor page, preferencesPages) {
		mPreferencesDialog.registerPage(page.first, page.second);
	}

	mUi->paletteTree->customizeExplosionTitles(
			toolManager().customizer()->userPaletteTitle()
			, toolManager().customizer()->userPaletteDescription());

	customizeActionsVisibility();
}

void MainWindow::customizeActionsVisibility()
{
	QList<QAction *> actions = findChildren<QAction *>();
	for (const QPair<QString, ActionVisibility> &info : toolManager().customizer()->actionsVisibility()) {
		QAction *action = nullptr;
		for (QAction * const currentAction : actions) {
			if (currentAction->objectName() == info.first) {
				action = currentAction;
				break;
			}
		}

		if (!action) {
			qDebug() << "Cannot find" << info.first << "in main window actions";
			continue;
		}

		QList<QToolBar *> toolBars;
		for (QWidget * const associatedWidget : action->associatedWidgets()) {
			if (QToolBar * const toolBar = qobject_cast<QToolBar *>(associatedWidget)) {
				toolBars << toolBar;
			}
		}

		switch (info.second) {
		case ActionVisibility::VisibleEverywhere:
			action->setVisible(true);
			break;
		case ActionVisibility::VisibleOnlyInMenu:
			for (QToolBar * const toolBar : toolBars) {
				toolBar->removeAction(action);
			}
			break;
		case ActionVisibility::Invisible:
			action->setVisible(false);
			break;
		}
	}
}

void MainWindow::initInterpretedPlugins()
{
	mInterpretedPluginLoader.init(editorManagerProxy().proxiedEditorManager(), PluginConfigurator(
			models().repoControlApi()
			, models().graphicalModelAssistApi()
			, models().logicalModelAssistApi()
			, *this
			, *this
			, *mProjectManager
			, *mSceneCustomizer
			, mFacade.events()
			, *mTextManager));

	const QList<ActionInfo> actions = mInterpretedPluginLoader.listOfActions();
	mListOfAdditionalActions = mInterpretedPluginLoader.menuActionsList();

	traverseListOfActions(actions);
}

void MainWindow::showErrors(const gui::ErrorReporter * const errorReporter)
{
	errorReporter->showErrors(mUi->errorListWidget, mUi->errorDock);
}

void MainWindow::reinitModels()
{
	closeAllTabs();

	models().reinit();

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
	const Customizer * const customizer = mToolManager.customizer();
	if (customizer) {
		setWindowTitle(customizer->windowTitle());
		setWindowIcon(customizer->applicationIcon());
		setVersion(customizer->productVersion());
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
	resetToolbarSize(SettingsManager::value("toolbarSize").toInt());
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
	mUi->propertyEditor->init(models().logicalModelAssistApi(), *mController);
	mUi->propertyEditor->setModel(&mPropertyModel);

	mUi->graphicalModelExplorer->setModel(models().graphicalModel());
	mUi->graphicalModelExplorer->setController(mController);
	mUi->graphicalModelExplorer->setAssistApi(&models().graphicalModelAssistApi());
	mUi->graphicalModelExplorer->setExploser(models().exploser());

	mUi->logicalModelExplorer->setModel(models().logicalModel());
	mUi->logicalModelExplorer->setController(mController);
	mUi->logicalModelExplorer->setAssistApi(&models().logicalModelAssistApi());
	mUi->logicalModelExplorer->setExploser(models().exploser());

	mPropertyModel.setSourceModels(models().logicalModel(), models().graphicalModel());

	connect(&models().graphicalModelAssistApi(), SIGNAL(nameChanged(const Id &))
			, this, SLOT(updateTabName(const Id &)));
	connect(mUi->graphicalModelExplorer, SIGNAL(clicked(const QModelIndex &))
			, this, SLOT(graphicalModelExplorerClicked(QModelIndex)));
	connect(mUi->logicalModelExplorer, SIGNAL(clicked(const QModelIndex &))
			, this, SLOT(logicalModelExplorerClicked(QModelIndex)));
}

void MainWindow::initRecentProjectsMenu()
{
	mRecentProjectsMenu = new QMenu(tr("Recent projects"), mUi->menu_File);
	mUi->menu_File->insertMenu(mUi->menu_File->actions().at(1), mRecentProjectsMenu);
	connect(mRecentProjectsMenu, SIGNAL(aboutToShow()), this, SLOT(openRecentProjectsMenu()));
}

void MainWindow::saveDiagramAsAPictureToFile(const QString &fileName)
{
	if (fileName.isEmpty()) {
		return;
	}
	const QRectF sceneRect = getCurrentTab()->scene()->itemsBoundingRect();

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
		const QString fileName = utils::QRealFileDialog::getSaveFileName("SaveDiagramAsPicture", this
				, tr("Save File"), "", tr("Images (*.png *.jpg)"));
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

void MainWindow::arrangeElementsByDotRunner(const QString &algorithm, const QString &absolutePathToDotFiles)
{
	const Id diagramId = activeDiagram();
	DotRunner *runner = new DotRunner(
			diagramId
			, models().graphicalModelAssistApi()
			, models().logicalModelAssistApi()
			, editorManager()
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
	const Id diagramId = activeDiagram();
	reinitModels();
	activateItemOrDiagram(diagramId);
	mUi->graphicalModelExplorer->setRootIndex(QModelIndex());
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

QStatusBar *MainWindow::statusBar() const
{
	return mUi->statusbar;
}

QList<QToolBar *> MainWindow::toolBars() const
{
	return findChildren<QToolBar *>(QString(), Qt::FindDirectChildrenOnly);
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

QByteArray MainWindow::saveState(int version) const
{
	return QMainWindow::saveState(version);
}

bool MainWindow::restoreState(const QByteArray &state, int version)
{
	const bool result = QMainWindow::restoreState(state, version);
	if (!mUi->errorListWidget->count() > 0) {
		mUi->errorDock->hide();
	}

	return result;
}

void MainWindow::setCorner(Qt::Corner corner, Qt::DockWidgetArea area)
{
	QMainWindow::setCorner(corner, area);
}

void MainWindow::setTabText(QWidget *tab, const QString &text)
{
	const int index = mUi->tabs->indexOf(tab);

	if (index != -1) {
		mUi->tabs->setTabText(index, text);
	}
}

void MainWindow::setVersion(const QString &version)
{
	// TODO: update title
	SettingsManager::setValue("version", version);
}

void MainWindow::openStartTab()
{
	mStartWidget = new StartWidget(this, mProjectManager);
	const int index = mUi->tabs->addTab(mStartWidget, tr("Getting Started"));
	mUi->tabs->setTabUnclosable(index);
	mStartWidget->setVisibleForInterpreterButton(mToolManager.customizer()->showInterpeterButton());
}

void MainWindow::beginPaletteModification()
{
}

void MainWindow::setElementInPaletteVisible(const Id &metatype, bool visible)
{
	mUi->paletteTree->setElementVisible(metatype, visible);
	// Note that if element is visible in palette, it is not necessary that it will be enabled it editor manager. It
	// may be greyed-out and still can't be used on diagrams.
}

void MainWindow::setVisibleForAllElementsInPalette(bool visible)
{
	mUi->paletteTree->setVisibleForAllElements(visible);
	for (const Id &editor : editorManager().editors()) {
		for (const Id &diagram : editorManager().diagrams(editor)) {
			for (const Id &element : editorManager().elements(diagram)) {
				editorManager().setElementEnabled(element, visible);
			}
		}
	}
}

void MainWindow::setElementInPaletteEnabled(const Id &metatype, bool enabled)
{
	mUi->paletteTree->setElementEnabled(metatype, enabled);
	editorManager().setElementEnabled(metatype, enabled);
}

void MainWindow::setEnabledForAllElementsInPalette(bool enabled)
{
	mUi->paletteTree->setEnabledForAllElements(enabled);
	for (const Id &editor : editorManager().editors()) {
		for (const Id &diagram: editorManager().diagrams(editor)) {
			for (const Id &element : editorManager().elements(diagram)) {
				editorManager().setElementEnabled(element, enabled);
			}
		}
	}
}

void MainWindow::endPaletteModification()
{
	// Disabling elements on scene...
	EditorViewScene * const scene = getCurrentTab() ? &getCurrentTab()->mutableScene() : nullptr;
	if (scene) {
		for (QGraphicsItem * const item : scene->items()) {
			if (Element * const element = dynamic_cast<Element *>(item)) {
				element->updateEnabledState();
			}
		}

		scene->update();
	}
}

#include "mainwindow.h"

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

#include <QtSvg/QSvgGenerator>

#include <QtCore/QDebug>
#include <QtCore/QSettings>
#include <QtCore/QPluginLoader>

#include "ui_mainwindow.h"
#include "errorReporter.h"

#include "../interpreters/robots/details/robotImplementations/abstractRobotModelImplementation.h"
#include "../interpreters/robots/details/robotParts/robotModel.h"
#include "../pluginInterface/editorInterface.h"
#include "preferencesDialog.h"
#include "shapeEdit/shapeEdit.h"
#include "openShapeEditorButton.h"
#include "propertyeditorproxymodel.h"
#include "gesturesShow/gestureswidget.h"

#include "../models/models.h"
#include "../view/editorview.h"
#include "../umllib/uml_element.h"
#include "../dialogs/plugindialog.h"
#include "../dialogs/robotSettingsDialog.h"
#include "../parsers/xml/xmlParser.h"
#include "../editorManager/listenerManager.h"
#include "../generators/editorGenerator/editorGenerator.h"
#include "../interpreters/visualDebugger/visualDebugger.h"

#include "../interpreters/robots/interpreter.h"

using namespace qReal;
using namespace interpreters::robots;

MainWindow::MainWindow()
	: mUi(new Ui::MainWindowUi)
	, mCloseEvent(NULL)
	, mModels(NULL)
	, mListenerManager(NULL)
	, mPropertyModel(mEditorManager)
	, mGesturesWidget(NULL)
	, mVisualDebugger(NULL)
	, mRobotInterpreter(NULL)
	, mBluetoothCommunication(NULL)
	, mErrorReporter(NULL)
	, mIsFullscreen(false)
{
	QSettings settings("SPbSU", "QReal");
	bool showSplash = settings.value("Splashscreen", true).toBool();
	QSplashScreen* splash =
			new QSplashScreen(QPixmap(":/icons/kroki3.PNG"), Qt::SplashScreen | Qt::WindowStaysOnTopHint);

	QProgressBar *progress = new QProgressBar((QWidget*) splash);
	progress->move(20, 270);
	progress->setFixedWidth(600);
	progress->setFixedHeight(15);
	progress->setRange(0, 100);

	// Step 1: splash screen loaded, progress bar initialized.
	progress->setValue(5);
	if (showSplash)
	{
		splash->show();
		QApplication::processEvents();
	}
	mUi->setupUi(this);

	mUi->tabs->setTabsClosable(true);
	mUi->tabs->setMovable(true);
	mUi->logicalModelDock->setVisible(false);

	if (!showSplash)
		mUi->actionShowSplash->setChecked(false);

	mUi->minimapView->setRenderHint(QPainter::Antialiasing, true);

	// Step 2: Ui is ready, splash screen shown.
	progress->setValue(20);
	connectActions();

	connect(mUi->tabs, SIGNAL(currentChanged(int)), this, SLOT(changeMiniMapSource(int)));
	connect(mUi->tabs, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));

	connect(mUi->minimapZoomSlider, SIGNAL(valueChanged(int)), this, SLOT(adjustMinimapZoom(int)));

	adjustMinimapZoom(mUi->minimapZoomSlider->value());
	initGridProperties();

	// Step 3: Ui connects are done.
	progress->setValue(40);

	mUi->paletteDock->setWidget(mUi->paletteToolbox);
	mUi->errorDock->setWidget(mUi->errorListWidget);
	mUi->errorListWidget->init(this);
	mUi->errorDock->setVisible(false);
	mUi->propertyEditor->setModel(&mPropertyModel);
	mUi->propertyEditor->verticalHeader()->hide();
	mUi->propertyEditor->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
	mUi->propertyEditor->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
	mUi->propertyEditor->setItemDelegate(&mDelegate);

	connect(mUi->graphicalModelExplorer, SIGNAL(clicked(QModelIndex const &)), this, SLOT(graphicalModelExplorerClicked(QModelIndex)));
	connect(mUi->logicalModelExplorer, SIGNAL(clicked(QModelIndex const &)), this, SLOT(logicalModelExplorerClicked(QModelIndex)));

	mUi->graphicalModelExplorer->addAction(mUi->actionDeleteFromDiagram);
	mUi->logicalModelExplorer->addAction(mUi->actionDeleteFromDiagram);

	// Step 4: Property editor and model explorers are initialized.
	progress->setValue(60);
	loadPlugins();
	showMaximized();

	// Step 5: Plugins are loaded.
	progress->setValue(70);

	settings.beginGroup("MainWindow");
	if (!settings.value("maximized", true).toBool()) {
		showNormal();
		resize(settings.value("size", QSize(1024, 800)).toSize());
		move(settings.value("pos", QPoint(0, 0)).toPoint());
	}
	// for jzuken's unholy netbook screen
//	resize(QSize(1024, 600));
	settings.endGroup();

	QString workingDir = settings.value("workingDir", ".").toString();

	mRootIndex = QModelIndex();
	mModels = new models::Models(workingDir, mEditorManager);

	// Step 6: Save loaded, models initialized.
	progress->setValue(80);

	if (!checkPluginsAndReopen())
		return;

	mPropertyModel.setSourceModels(mModels->logicalModel(), mModels->graphicalModel());

	connect(&mModels->graphicalModelAssistApi(), SIGNAL(nameChanged(Id const &)), this, SLOT(updateTabName(Id const &)));

	mUi->graphicalModelExplorer->setModel(mModels->graphicalModel());
	mUi->logicalModelExplorer->setModel(mModels->logicalModel());

	mGesturesWidget = new GesturesWidget();
	mVisualDebugger = new VisualDebugger(mModels->graphicalModelAssistApi());

	mDelegate.init(this, &mModels->logicalModelAssistApi());

	mErrorReporter = new gui::ErrorReporter(mUi->errorListWidget, mUi->errorDock);
	mErrorReporter->updateVisibility(settings.value("warningWindow", true).toBool());

	QString const defaultBluetoothPortName = settings.value("bluetoothPortName", "").toString();
	mBluetoothCommunication = new interpreters::robots::BluetoothRobotCommunication(defaultBluetoothPortName);
	mD2Model = new interpreters::robots::details::d2Model::D2RobotModel();
	robotModelType::robotModelTypeEnum typeOfRobotModel = static_cast<robotModelType::robotModelTypeEnum>(settings.value("robotModel", "1").toInt());
	interpreters::robots::details::robotImplementations::AbstractRobotModelImplementation *robotImpl = interpreters::robots::details::robotImplementations::AbstractRobotModelImplementation::robotModel(typeOfRobotModel, mBluetoothCommunication, mD2Model);
	interpreters::robots::details::RobotModel *robotModel = new interpreters::robots::details::RobotModel();
	mRobotInterpreter = new interpreters::robots::Interpreter(mModels->graphicalModelAssistApi()
			, mModels->logicalModelAssistApi(), *this, robotModel);
	mRobotInterpreter->setRobotImplementation(robotImpl);
	sensorType::SensorTypeEnum port1 = static_cast<sensorType::SensorTypeEnum>(settings.value("port1SensorType", "0").toInt());
	sensorType::SensorTypeEnum port2 = static_cast<sensorType::SensorTypeEnum>(settings.value("port2SensorType", "0").toInt());
	sensorType::SensorTypeEnum port3 = static_cast<sensorType::SensorTypeEnum>(settings.value("port3SensorType", "0").toInt());
	sensorType::SensorTypeEnum port4 = static_cast<sensorType::SensorTypeEnum>(settings.value("port4SensorType", "0").toInt());
	mRobotInterpreter->configureSensors(port1, port2, port3, port4);

	// Step 7: Save consistency checked, interface is initialized with models.
	progress->setValue(100);
	if (showSplash)
		splash->close();
	delete splash;

	if (mModels->graphicalModel()->rowCount() > 0)
		openNewTab(mModels->graphicalModel()->index(0, 0, QModelIndex()));

	if (settings.value("diagramCreateSuggestion", true).toBool())
		suggestToCreateDiagram();

	mDocksVisibility.clear();
}

void MainWindow::connectActions()
{
	QSettings settings("SPbSU", "QReal");
	mUi->actionShow_grid->setChecked(settings.value("ShowGrid", true).toBool());
	mUi->actionShow_alignment->setChecked(settings.value("ShowAlignment", true).toBool());
	mUi->actionSwitch_on_grid->setChecked(settings.value("ActivateGrid", false).toBool());
	mUi->actionSwitch_on_alignment->setChecked(settings.value("ActivateAlignment", true).toBool());

	connect(mUi->actionQuit, SIGNAL(triggered()), this, SLOT(close()));

	connect(mUi->actionShowSplash, SIGNAL(toggled(bool)), this, SLOT (toggleShowSplash(bool)));

	connect(mUi->actionOpen, SIGNAL(triggered()), this, SLOT(open()));
	connect(mUi->actionSave, SIGNAL(triggered()), this, SLOT(saveAll()));
	connect(mUi->actionSave_as, SIGNAL(triggered()), this, SLOT(saveAs()));
	connect(mUi->actionPrint, SIGNAL(triggered()), this, SLOT(print()));
	connect(mUi->actionMakeSvg, SIGNAL(triggered()), this, SLOT(makeSvg()));

	connect(mUi->actionDeleteFromDiagram, SIGNAL(triggered()), this, SLOT(deleteFromDiagram()));

	connect(mUi->actionShape_Edit, SIGNAL(triggered()), this, SLOT(openShapeEditor()));
	connect(mUi->actionGenerate_Editor, SIGNAL(triggered()), this, SLOT(generateEditor()));
	connect(mUi->actionPreferences, SIGNAL(triggered()), this, SLOT(showPreferencesDialog()));

	connect(mUi->actionPlugins, SIGNAL(triggered()), this, SLOT(settingsPlugins()));
	connect(mUi->actionShow_grid, SIGNAL(toggled(bool)), this, SLOT(showGrid(bool)));
	connect(mUi->actionShow_alignment, SIGNAL(toggled(bool)), this, SLOT(showAlignment(bool)));
	connect(mUi->actionSwitch_on_grid, SIGNAL(toggled(bool)), this, SLOT(switchGrid(bool)));
	connect(mUi->actionSwitch_on_alignment, SIGNAL(toggled(bool)), this, SLOT(switchAlignment(bool)));

	connect(mUi->actionHelp, SIGNAL(triggered()), this, SLOT(showHelp()));
	connect(mUi->actionAbout, SIGNAL(triggered()), this, SLOT(showAbout()));
	connect(mUi->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

	connect(mUi->actionShow, SIGNAL(triggered()), this, SLOT(showGestures()));

	connect(mUi->actionClear, SIGNAL(triggered()), this, SLOT(exterminate()));

	connect(mUi->actionRun, SIGNAL(triggered()), this, SLOT(run()));
	connect(mUi->actionStop_Running, SIGNAL(triggered()), this, SLOT(stop()));
	connect(mUi->actionStop_Robot, SIGNAL(triggered()), this, SLOT(stopRobot()));
	connect(mUi->actionRobot_Settings, SIGNAL(triggered()), this, SLOT(showRobotSettingsDialog()));

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
	}
}

MainWindow::~MainWindow()
{
	saveAll();
	delete mListenerManager;
	delete mRobotInterpreter;
	delete mErrorReporter;
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
	mCloseEvent = event;
	QSettings settings("SPbSU", "QReal");
	settings.beginGroup("MainWindow");
	settings.setValue("maximized", isMaximized());
	settings.setValue("size", size());
	settings.setValue("pos", pos());
	settings.endGroup();
}

void MainWindow::loadPlugins()
{
	foreach (Id const editor, mEditorManager.editors()) {
		foreach (Id const diagram, mEditorManager.diagrams(editor)) {
			mUi->paletteToolbox->addDiagramType(diagram, mEditorManager.friendlyName(diagram) );

			foreach (Id const element, mEditorManager.elements(diagram)) {
				mUi->paletteToolbox->addItemType(element, mEditorManager.friendlyName(element), mEditorManager.description(element), mEditorManager.icon(element));
			}
		}
	}
	mUi->paletteToolbox->initDone();
}

void MainWindow::adjustMinimapZoom(int zoom)
{
	mUi->minimapView->resetMatrix();
	mUi->minimapView->scale(0.01*zoom,0.01*zoom);
}

void MainWindow::selectItem(Id const &id)
{
	if (id == Id::rootId())
		return;

	setIndexesOfPropertyEditor(id);
	centerOn(id);
}

void MainWindow::activateItemOrDiagram(const QModelIndex &idx, bool bl, bool isSetSel)
{
	QModelIndex parent = idx.parent();

	int numTab = getTabIndex(idx);
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
			UML::Element *e = (static_cast<EditorViewScene *>(getCurrentTab()->scene()))->getElem(idx.data(roles::idRole).value<Id>());
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
	activateItemOrDiagram(mModels->graphicalModelAssistApi().indexById(id), bl, isSetSel);
}

void MainWindow::activateSubdiagram(QModelIndex const &idx) {
	// end-to-end links: if there's a first-level diagram with the same name as
	// this element, show it
	Id id = idx.data(roles::idRole).value<Id>();
	QString targetName = mModels->graphicalModelAssistApi().name(id);
	int rows = mModels->graphicalModelAssistApi().childrenOfRootDiagram();
	for (int i = 0; i < rows; ++i) {
		Id child = mModels->graphicalModelAssistApi().rootId();
		if (mModels->graphicalModelAssistApi().name(child) == targetName)
		{
			activateItemOrDiagram(child);
			return;
		}
	}

	QModelIndex diagramToActivate = idx;
	while (diagramToActivate.isValid() && diagramToActivate.parent().isValid()
		&& diagramToActivate.parent() != getCurrentTab()->mvIface()->rootIndex())
		{
		diagramToActivate = diagramToActivate.parent();
	}

	if (diagramToActivate.model()->rowCount(diagramToActivate) > 0) {
		QModelIndex childIndex = diagramToActivate.model()->index(0, 0, diagramToActivate);
		activateItemOrDiagram(childIndex);
	}
}

void MainWindow::sceneSelectionChanged()
{
	if (!getCurrentTab())
		return;
	QList<QGraphicsItem*> graphicsItems = getCurrentTab()->scene()->selectedItems();
	int length = graphicsItems.size();
	if (length == 1) {
		QGraphicsItem *item = graphicsItems[0];
		if (UML::Element *elem = dynamic_cast<UML::Element *>(item)) {
			setIndexesOfPropertyEditor(elem->id());
			QModelIndex const index = mModels->graphicalModelAssistApi().indexById(elem->id());
			if (index.isValid()) {
				mUi->graphicalModelExplorer->setCurrentIndex(index);
			}
		} else {
			mUi->graphicalModelExplorer->setCurrentIndex(QModelIndex());
			mPropertyModel.clearModelIndexes();

			foreach(QGraphicsItem* item, graphicsItems) {
				UML::EdgeElement* edge = dynamic_cast<UML::EdgeElement*>(item);
				if (edge) {
					length--;
					graphicsItems.removeOne(edge);
				}
			}
			//TODO: remove it? length < 2
			if (length > 1) {
				foreach(QGraphicsItem* item, graphicsItems) {
					UML::NodeElement* node = dynamic_cast<UML::NodeElement*>(item);
					if (node)
						node->hideEmbeddedLinkers();
				}
			}
		}
	}
}

QString MainWindow::getWorkingDir(QString const &dialogWindowTitle)
{
	QSettings settings("SPbSU", "QReal");

	QString const dirName = QFileDialog::getExistingDirectory(this, dialogWindowTitle
		, settings.value("workingDir", ".").toString(), QFileDialog::ShowDirsOnly);

	if (dirName.isEmpty())
		return "";

	settings.setValue("workingDir", dirName);

	return dirName;
}

bool MainWindow::checkPluginsAndReopen()
{
	IdList missingPlugins = mEditorManager.checkNeededPlugins(mModels->logicalRepoApi(), mModels->graphicalRepoApi());
	bool haveMissingPlugins = !missingPlugins.isEmpty();
	bool loadingCancelled = false;

	while (haveMissingPlugins && !loadingCancelled) {
		QString text = "These plugins are not present, but needed to load the save:\n";
		foreach (Id const id, missingPlugins)
			text += id.editor() + "\n";
		text += "Do you want to create/open new project?";

		QMessageBox::StandardButton button = QMessageBox::question(this, tr("Some plugins are missing"), text, QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

		if (button == QMessageBox::Yes)
		{
			if (!open())
				loadingCancelled = true;
		}
		else
			loadingCancelled = true;
		missingPlugins = mEditorManager.checkNeededPlugins(
					mModels->logicalRepoApi(), mModels->graphicalRepoApi());
		haveMissingPlugins = !missingPlugins.isEmpty();
	}

	if (loadingCancelled){
		close();
		return false;
	}

	return true;
}

bool MainWindow::open()
{
	QString const dirName = getWorkingDir(tr("Select directory with a save to open"));

	if (dirName.isEmpty())
		return false;

	dynamic_cast<PropertyEditorModel*>(mUi->propertyEditor->model())->clearModelIndexes();
	mUi->graphicalModelExplorer->setModel(NULL);
	mUi->logicalModelExplorer->setModel(NULL);
	if (getCurrentTab())
		static_cast<EditorViewScene*>(getCurrentTab()->scene())->clearScene();

	closeAllTabs();

	mModels->repoControlApi().open(dirName);
	mModels->reinit();

	if (!checkPluginsAndReopen())
		return false;

	mPropertyModel.setSourceModels(mModels->logicalModel(), mModels->graphicalModel());
	mUi->graphicalModelExplorer->setModel(mModels->graphicalModel());
	mUi->logicalModelExplorer->setModel(mModels->logicalModel());

	return true;
}

void MainWindow::closeAllTabs(){
	int tabCount = mUi->tabs->count();
	for (int i = 0; i < tabCount; i++)
		closeTab(i);
}

void MainWindow::setShape(const QString &data, const QPersistentModelIndex &index, const int &role)
{
	// const_cast here is ok, since we need to set a shape in a correct model, and
	// not going to use this index anymore.
	QAbstractItemModel *model = const_cast<QAbstractItemModel *>(index.model());
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
	if (isLogicalModel) {
		Id logicalId = mModels->logicalModelAssistApi().idByIndex(index);
		IdList graphicalIdList = mModels->graphicalModelAssistApi().graphicalIdsByLogicalId(logicalId);
		foreach (Id graphicalId, graphicalIdList) {
			closeTab(mModels->graphicalModelAssistApi().indexById(graphicalId));
		}
	} else
		closeTab(index);
	if (index.isValid()) {
		PropertyEditorModel* pModel = dynamic_cast<PropertyEditorModel*>(mUi->propertyEditor->model());
		if (pModel->isCurrentIndex(index)) {
			pModel->clearModelIndexes();
			mUi->propertyEditor->setRootIndex(QModelIndex());
		}
		if (isLogicalModel)
			mModels->logicalModel()->removeRow(index.row(), index.parent());
		else
			mModels->graphicalModel()->removeRow(index.row(), index.parent());
	}
}

void MainWindow::deleteFromScene()
{
	foreach (QGraphicsItem *item, getCurrentTab()->scene()->selectedItems())
		deleteFromScene(item);
}

void MainWindow::deleteFromScene(QGraphicsItem *target)
{
	if (UML::Element *elem = dynamic_cast<UML::Element *>(target)) {
		QPersistentModelIndex index = mModels->graphicalModelAssistApi().indexById(elem->id());
		if (index.isValid()) {
			PropertyEditorModel* pModel = dynamic_cast<PropertyEditorModel*>(mUi->propertyEditor->model());
			if (pModel->isCurrentIndex(index))
				pModel->clearModelIndexes();
			mUi->propertyEditor->setRootIndex(QModelIndex());
			mModels->graphicalModel()->removeRow(index.row(), index.parent());
		}
		if (getCurrentTab() != NULL && getCurrentTab()->scene() != NULL)
			getCurrentTab()->scene()->invalidate();
	}
}

void MainWindow::deleteFromDiagram()
{
	bool isLogicalModel = false;
	if (mModels->graphicalModel()) {
		if (mUi->graphicalModelExplorer->hasFocus()) {
			isLogicalModel = false;
			deleteFromExplorer(isLogicalModel);
		} else if (getCurrentTab() != NULL && getCurrentTab()->hasFocus()) {
			deleteFromScene();
		}
	}
	if (mModels->logicalModel()) {
		if (mUi->logicalModelExplorer->hasFocus()) {
			isLogicalModel = true;
			deleteFromExplorer(isLogicalModel);
		}
	}
	if (getCurrentTab() != NULL && getCurrentTab()->scene() != NULL)
		getCurrentTab()->scene()->invalidate();
}

void MainWindow::showAbout()
{
	QMessageBox::about(this, tr("About QReal:Robots"),
			tr("Contacts:<br><br>"
			"se.math.spbu.ru/SE/qreal"));
}

void MainWindow::showHelp()
{
	QMessageBox::about(this, tr("Help"),
			tr("To begin:\n"
			"1. To add items to diagrams, drag & drop them from Palette to editor\n"
			"2. Get more help from author :)"));
}

void MainWindow::toggleShowSplash(bool show)
{
	QSettings settings("SPbSU", "QReal");
	settings.setValue("Splashscreen", show);
}

void MainWindow::generateEditor()
{
	generators::EditorGenerator editorGenerator(mModels->logicalRepoApi());

	QString directoryName;
	QFileInfo directoryXml;
	const QHash<Id, QString> metamodelList = editorGenerator.getMetamodelList();
	QDir dir(".");
	bool found = false;
	while (dir.cdUp() && !found) {
		QFileInfoList infoList = dir.entryInfoList(QDir::Dirs);
		foreach (QFileInfo const directory, infoList){
			if (directory.baseName() == "qrxml") {
				found = true;
				directoryXml = directory;
				directoryName = directory.absolutePath();
			}
		}
	}
	if (!found) {
		QMessageBox::warning(this, tr("error"), tr("Cannot find the directory for saving"));
		return;
	}
	foreach (Id const key, metamodelList.keys()) {
		dir.mkdir(directoryXml.absolutePath() + "/qrxml/" + metamodelList[key]);
		gui::ErrorReporter& errors = editorGenerator.generateEditor(key, directoryName + "/qrxml/" + metamodelList[key] + "/" + metamodelList[key]);

		if (errors.showErrors(mUi->errorListWidget, mUi->errorDock)) {
			if (QMessageBox::question(this, tr("loading.."), QString(tr("Do you want to load generated editor %1?")).arg(metamodelList[key]),
					QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
				return;
			QSettings settings("SPbSU", "QReal");
			loadNewEditor(directoryName + "/qrxml/", metamodelList[key], settings.value("pathToQmake", "").toString(),
					settings.value("pathToMake", "").toString(), settings.value("pluginExtension", "").toString(), settings.value("prefix", "").toString());
		}
	}
}

void MainWindow::loadNewEditor(const QString &directoryName, const QString &metamodelName,
		const QString &commandFirst, const QString &commandSecond, const QString &extension, const QString &prefix)
{
	int const progressBarWidth = 240;
	int const progressBarHeight = 20;

	if ((commandFirst == "") || (commandSecond == "") || (extension == "")) {
		QMessageBox::warning(this, tr("error"), tr("please, fill compiler settings"));
		return;
	}

	QString normalizeDirName = metamodelName.at(0).toUpper() + metamodelName.mid(1);

	QProgressBar *progress = new QProgressBar(this);
	progress->show();

	QApplication::processEvents();

	QRect screenRect = qApp->desktop()->availableGeometry();
	progress->move(screenRect.width() / 2 - progressBarWidth / 2, screenRect.height() / 2 - progressBarHeight / 2);
	progress->setFixedWidth(progressBarWidth);
	progress->setFixedHeight(progressBarHeight);
	progress->setRange(0, 100);
	progress->setValue(5);

	if (mEditorManager.editors().contains(Id(normalizeDirName))) {
		foreach (Id const diagram, mEditorManager.diagrams(Id(normalizeDirName)))
			mUi->paletteToolbox->deleteDiagramType(diagram);

		if (!mEditorManager.unloadPlugin(normalizeDirName)) {
			QMessageBox::warning(this, tr("error"), tr("cannot unload plugin"));
			progress->close();
			delete progress;
			return;
		}
	}
	progress->setValue(20);

	QProcess builder;
	builder.setWorkingDirectory(directoryName + "/" + metamodelName);
	builder.start(commandFirst);
	if ((builder.waitForFinished()) && (builder.exitCode() == 0)) {
		progress->setValue(60);
		builder.start(commandSecond);
		if (builder.waitForFinished() && (builder.exitCode() == 0)) {
			progress->setValue(80);
			if (mEditorManager.loadPlugin(prefix + metamodelName + "." + extension)) {
				foreach (Id const diagram, mEditorManager.diagrams(Id(normalizeDirName))) {
					mUi->paletteToolbox->addDiagramType(diagram, mEditorManager.friendlyName(diagram));

					foreach (Id const element, mEditorManager.elements(diagram))
						mUi->paletteToolbox->addItemType(element, mEditorManager.friendlyName(element), mEditorManager.description(element), mEditorManager.icon(element));
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
	if (!mEditorManager.editors().contains(Id("Meta_editor"))) {
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

		if (editorView != NULL && (static_cast<EditorViewScene*>(editorView->scene()))->mainWindow() != NULL)
		{
			mUi->minimapView->setScene(editorView->scene());
			getCurrentTab()->mvIface()->setModel(mModels->graphicalModel());
			getCurrentTab()->mvIface()->setLogicalModel(mModels->logicalModel());
			mRootIndex = editorView->mvIface()->rootIndex();
		}
	} else
	{
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

void MainWindow::exterminate()
{
	int tabCount = mUi->tabs->count();
	for (int i = 0; i < tabCount; i++)
		closeTab(i);
	mModels->repoControlApi().exterminate();
	mModels->reinit();

	PropertyEditorModel* pModel = dynamic_cast<PropertyEditorModel*>(mUi->propertyEditor->model());
	pModel->clearModelIndexes();
	mUi->propertyEditor->setRootIndex(QModelIndex());
}

void MainWindow::showPreferencesDialog()
{
	PreferencesDialog preferencesDialog(mUi->actionShow_grid, mUi->actionShow_alignment, mUi->actionSwitch_on_grid, mUi->actionSwitch_on_alignment);
	if (getCurrentTab() != NULL) {
		connect(&preferencesDialog, SIGNAL(gridChanged()), getCurrentTab(), SLOT(invalidateScene()));
		connect(&preferencesDialog, SIGNAL(iconsetChanged()), this, SLOT(updatePaletteIcons()));
		connect(&preferencesDialog, SIGNAL(settingsApplied()), this, SLOT(applySettings()));
	}
	preferencesDialog.exec();
}

void MainWindow::openShapeEditor()
{
	QObject const *object = sender();
	OpenShapeEditorButton const *button = dynamic_cast<OpenShapeEditorButton const *>(object);
	QString const text = tr("Shape Editor");
	ShapeEdit *shapeEdit = NULL;
	if (button != NULL) {
		QPersistentModelIndex index = button->index();
		int role = button->role();
		QString const propertyValue = button->propertyValue();
		shapeEdit = new ShapeEdit(index, role);
		if (!propertyValue.isEmpty())
			shapeEdit->load(propertyValue);
		// Here we are going to actually modify model to set a value of a shape.
		QAbstractItemModel *model = const_cast<QAbstractItemModel *>(index.model());
		model->setData(index, propertyValue, role);
		connect(shapeEdit, SIGNAL(shapeSaved(QString, QPersistentModelIndex const &, int const &)), this, SLOT(setShape(QString, QPersistentModelIndex const &, int const &)));
	}
	else {
		shapeEdit = new ShapeEdit();
	}

	mUi->tabs->addTab(shapeEdit, text);
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
	EditorView *tab = dynamic_cast<EditorView *>(widget);
	if (tab != NULL)
		disconnectZoom(tab);
	else {
		ShapeEdit *shapeTab = dynamic_cast<ShapeEdit *>(widget);
		if (shapeTab != NULL)
			disconnectZoom(shapeTab->getView());
	}
}

void MainWindow::connectActionZoomTo(QWidget* widget)
{
	EditorView *view = (dynamic_cast<EditorView *>(widget));
	if (view != NULL)
		connectZoom(view);
	else {
		ShapeEdit *shapeWidget = (dynamic_cast<ShapeEdit *>(widget));
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
	UML::Element* const element = scene->getElem(id);

	scene->clearSelection();
	if (element != NULL) {
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
	Id const id = mModels->graphicalModelAssistApi().idByIndex(index);
	setIndexesOfPropertyEditor(id);
	openNewTab(index);
	centerOn(id);
}

void MainWindow::logicalModelExplorerClicked(const QModelIndex &index)
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
		EditorView *view = new EditorView();
		mUi->tabs->addTab(view, index.data().toString());
		mUi->tabs->setCurrentWidget(view);
		initCurrentTab(index);
	}

	//changing of palette active editor:
	QSettings settings("SPbSU", "QReal");
	if (settings.value("PaletteTabSwitching", true).toBool())
	{
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

void MainWindow::initCurrentTab(const QModelIndex &rootIndex)
{
	getCurrentTab()->setMainWindow(this);
	QModelIndex index = rootIndex;

	getCurrentTab()->mvIface()->setAssistApi(mModels->graphicalModelAssistApi(), mModels->logicalModelAssistApi());

	connect(getCurrentTab()->scene(), SIGNAL(selectionChanged()), SLOT(sceneSelectionChanged()));
	connect(mUi->actionAntialiasing, SIGNAL(toggled(bool)), getCurrentTab(), SLOT(toggleAntialiasing(bool)));
	connect(mUi->actionOpenGL_Renderer, SIGNAL(toggled(bool)), getCurrentTab(), SLOT(toggleOpenGL(bool)));

	getCurrentTab()->mvIface()->setModel(mModels->graphicalModel());
	getCurrentTab()->mvIface()->setLogicalModel(mModels->logicalModel());
	getCurrentTab()->mvIface()->setRootIndex(index);
	changeMiniMapSource(mUi->tabs->currentIndex());

	connect(mModels->graphicalModel(), SIGNAL(rowsAboutToBeMoved(QModelIndex, int, int, QModelIndex, int))
			, getCurrentTab()->mvIface(), SLOT(rowsAboutToBeMoved(QModelIndex, int, int, QModelIndex, int)));
	connect(mModels->graphicalModel(), SIGNAL(rowsMoved(QModelIndex, int, int, QModelIndex, int))
			, getCurrentTab()->mvIface(), SLOT(rowsMoved(QModelIndex, int, int, QModelIndex, int)));
}

void MainWindow::updateTabName(Id const &id)
{
	for (int i = 0; i < mUi->tabs->count(); i++) {
		EditorView *tab = (static_cast<EditorView *>(mUi->tabs->widget(i)));
		if (tab->mvIface()->rootIndex() == mModels->graphicalModelAssistApi().indexById(id)) {
			mUi->tabs->setTabText(i, mModels->graphicalModelAssistApi().name(id));
			return;
		}
	}
}

void MainWindow::closeTab(QModelIndex const &graphicsIndex)
{
	for (int i = 0; i < mUi->tabs->count(); i++) {
		EditorView *tab = (static_cast<EditorView *>(mUi->tabs->widget(i)));
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
	QSettings settings("SPbSU", "QReal");
	settings.setValue("ShowGrid", isChecked);
	setShowGrid(isChecked);
}

void MainWindow::showAlignment(bool isChecked)
{
	QSettings settings("SPbSU", "QReal");
	settings.setValue("ShowAlignment", isChecked);
	setShowAlignment(isChecked);
}

void MainWindow::switchGrid(bool isChecked)
{
	QSettings settings("SPbSU", "QReal");
	settings.setValue("ActivateGrid", isChecked);
	setSwitchGrid(isChecked);
}

void MainWindow::switchAlignment(bool isChecked)
{
	QSettings settings("SPbSU", "QReal");
	settings.setValue("ActivateAlignment", isChecked);
	setSwitchAlignment(isChecked);
}

void MainWindow::setShowGrid(bool isChecked)
{
	for (int i = 0; i < mUi->tabs->count(); i++) {
		EditorView *tab = (dynamic_cast<EditorView *>(mUi->tabs->widget(i)));
		if (tab != NULL)
			tab->setDrawSceneGrid(isChecked);
	}
}

void MainWindow::setShowAlignment(bool isChecked)
{
	for (int i = 0; i < mUi->tabs->count(); i++) {
		EditorView *tab = (dynamic_cast<EditorView *>(mUi->tabs->widget(i)));
		if (tab != NULL) {
			QList<QGraphicsItem *> list = tab->scene()->items();
			foreach (QGraphicsItem *item, list) {
				NodeElement* nodeItem = dynamic_cast<NodeElement*>(item);
				if (nodeItem != NULL)
					nodeItem->showAlignment(isChecked);
			}
		}
	}
}

void MainWindow::setSwitchGrid(bool isChecked)
{
	for (int i = 0; i < mUi->tabs->count(); i++) {
		EditorView *tab = (dynamic_cast<EditorView *>(mUi->tabs->widget(i)));
		if (tab != NULL) {
			QList<QGraphicsItem *> list = tab->scene()->items();
			foreach (QGraphicsItem *item, list) {
				NodeElement* nodeItem = dynamic_cast<NodeElement*>(item);
				if (nodeItem != NULL)
					nodeItem->switchGrid(isChecked);
			}
		}
	}
}

void MainWindow::setSwitchAlignment(bool isChecked)
{
	for (int i = 0; i < mUi->tabs->count(); i++) {
		EditorView *tab = (dynamic_cast<EditorView *>(mUi->tabs->widget(i)));
		if (tab != NULL) {
			QList<QGraphicsItem *> list = tab->scene()->items();
			foreach (QGraphicsItem *item, list) {
				NodeElement* nodeItem = dynamic_cast<NodeElement*>(item);
				if (nodeItem != NULL)
					nodeItem->switchAlignment(isChecked);
			}
		}
	}
}

void MainWindow::showGestures()
{
	QString text = tr("Gestures Show");
	mGesturesWidget = new GesturesWidget();
	mUi->tabs->addTab(mGesturesWidget, text);
	mUi->tabs->setCurrentWidget(mGesturesWidget);
	connect(mGesturesWidget, SIGNAL(currentElementChanged()), this, SIGNAL(currentIdealGestureChanged()));
	emit gesturesShowed();
}

IGesturesPainter * MainWindow::gesturesPainter()
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
			const QString diagramName = mEditorManager.editorInterface(editor.editor())->diagramName(diagram.diagram());
			const QString diagramNodeName = mEditorManager.editorInterface(editor.editor())->diagramNodeName(diagram.diagram());
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
	mModels->repoControlApi().saveAll();
}

void MainWindow::saveAs()
{
	QString const dirName = getWorkingDir(tr("Select directory to save current model to"));
	if (dirName.isEmpty())
		return;
	mModels->repoControlApi().saveTo(dirName);
}

int MainWindow::getTabIndex(const QModelIndex &index)
{
	for (int i = 0; i < mUi->tabs->count(); ++i)
	{
		EditorView *editor = dynamic_cast<EditorView *>(mUi->tabs->widget(i));
		if (!editor)
			continue;
		if (index.parent() == editor->mvIface()->rootIndex())
			return i;
	}
	return -1;
}

void MainWindow::initGridProperties()
{
	QSettings settings("SPbSU", "QReal");
	mUi->actionSwitch_on_grid->blockSignals(false);
	mUi->actionSwitch_on_grid->setChecked(settings.value("ActivateGrid", true).toBool());

	mUi->actionShow_grid->blockSignals(false);
	mUi->actionShow_grid->setChecked(settings.value("ShowGrid", true).toBool());
}

void MainWindow::run()
{
	mErrorReporter->clear();
	Id const currentDiagramId = getCurrentTab()->mvIface()->rootId();
	mRobotInterpreter->interpret(currentDiagramId);
}

void MainWindow::stop()
{
	mRobotInterpreter->stop();
}

void MainWindow::stopRobot()
{
	mRobotInterpreter->stopRobot();
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

void MainWindow::highlight(Id const &graphicalId, bool exclusive)
{
	EditorView* const view = getCurrentTab();
	EditorViewScene* const scene = dynamic_cast<EditorViewScene*>(view->scene());
	UML::Element* const element = scene->getElem(graphicalId);
	scene->highlight(graphicalId, exclusive);
	view->ensureElementVisible(element);
}

void MainWindow::dehighlight(Id const &graphicalId)
{
	EditorView* const view = getCurrentTab();
	EditorViewScene* const scene = dynamic_cast<EditorViewScene*>(view->scene());
	scene->dehighlight(graphicalId);
}

void MainWindow::showRobotSettingsDialog()
{
	stopRobot();
	gui::RobotSettingsDialog robotSettingsDialog;
	int code = robotSettingsDialog.exec();
	if (code == QDialog::Accepted) {
		mBluetoothCommunication->setPortName(robotSettingsDialog.selectedPortName());
		QSettings settings("SPbSU", "QReal");
		robotModelType::robotModelTypeEnum typeOfRobotModel = static_cast<robotModelType::robotModelTypeEnum>(settings.value("robotModel", "1").toInt());
		interpreters::robots::details::robotImplementations::AbstractRobotModelImplementation *robotImpl = interpreters::robots::details::robotImplementations::AbstractRobotModelImplementation::robotModel(typeOfRobotModel, mBluetoothCommunication, mD2Model);
		mRobotInterpreter->setRobotImplementation(robotImpl);
		mRobotInterpreter->configureSensors(
				robotSettingsDialog.selectedPort1Sensor()
				, robotSettingsDialog.selectedPort2Sensor()
				, robotSettingsDialog.selectedPort3Sensor()
				, robotSettingsDialog.selectedPort4Sensor()
		);
	}
}

gui::ErrorReporter *MainWindow::errorReporter()
{
	return mErrorReporter;
}

void MainWindow::on_actionNew_Diagram_triggered()
{
	if (getCurrentTab() == NULL || getCurrentTab()->mvIface() == NULL)
		return;

	Id const diagram = getCurrentTab()->mvIface()->rootId();  // Or some other way to find current diagram.
	createDiagram(diagram.type().toString());
}

void MainWindow::updatePaletteIcons()
{
	mUi->graphicalModelExplorer->viewport()->update();
	mUi->logicalModelExplorer->viewport()->update();

	Id currentId = mUi->paletteToolbox->currentTab();
	mUi->paletteToolbox->recreateTabs();

	loadPlugins();

	mUi->paletteToolbox->setActiveEditor(currentId);
	mUi->paletteToolbox->setComboBox(currentId);
}

void MainWindow::applySettings()
{
	getCurrentTab()->invalidateScene();
	QSettings settings("SPbSU", "QReal");
	mErrorReporter->updateVisibility(settings.value("warningWindow", true).toBool());
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

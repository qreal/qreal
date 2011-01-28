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

#include "errorReporter.h"

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
#include "../parsers/xml/xmlParser.h"
#include "../dialogs/checkoutdialog.h"
#include "../generators/xmi/xmiHandler.h"
#include "../generators/java/javaHandler.h"
#include "../parsers/hascol/hascolParser.h"
#include "../editorManager/listenerManager.h"
#include "../generators/hascol/hascolGenerator.h"
#include "../generators/editorGenerator/editorGenerator.h"
#include "../visualDebugger/visualDebugger.h"

#include "metaCompiler.h"

using namespace qReal;

MainWindow::MainWindow()
	: mListenerManager(NULL), mPropertyModel(mEditorManager)
{
	QSettings settings("SPbSU", "QReal");
	bool showSplash = settings.value("Splashscreen", true).toBool();
	QSplashScreen* splash =
			new QSplashScreen(QPixmap(":/icons/kroki3.PNG"), Qt::SplashScreen | Qt::WindowStaysOnTopHint);

	QProgressBar *progress = new QProgressBar((QWidget*) splash);
	progress->move(20,270);
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
	ui.setupUi(this);

#if defined(Q_WS_WIN)
	ui.menuSvn->setEnabled(false);  // Doesn't work under Windows anyway.
#endif

	ui.tabs->setTabsClosable(true);
	ui.tabs->setMovable(true);

	if (!showSplash)
		ui.actionShowSplash->setChecked(false);

	ui.minimapView->setRenderHint(QPainter::Antialiasing, true);

	// Step 2: Ui is ready, splash screen shown.
	progress->setValue(20);
	ui.actionShow_grid->setChecked(settings.value("ShowGrid", true).toBool());
	ui.actionShow_alignment->setChecked(settings.value("ShowAlignment", true).toBool());
	ui.actionSwitch_on_grid->setChecked(settings.value("ActivateGrid", false).toBool());
	ui.actionSwitch_on_alignment->setChecked(settings.value("ActivateAlignment", true).toBool());

	connect(ui.actionQuit, SIGNAL(triggered()), this, SLOT(close()));

	connect(ui.actionShowSplash, SIGNAL(toggled(bool)), this, SLOT (toggleShowSplash(bool)));

	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(open()));
	connect(ui.actionSave, SIGNAL(triggered()), this, SLOT(saveAll()));
	connect(ui.actionSave_as, SIGNAL(triggered()), this, SLOT(saveAs()));
	connect(ui.actionPrint, SIGNAL(triggered()), this, SLOT(print()));
	connect(ui.actionMakeSvg, SIGNAL(triggered()), this, SLOT(makeSvg()));

	connect(ui.actionDeleteFromDiagram, SIGNAL(triggered()), this, SLOT(deleteFromDiagram()));

	connect(ui.tabs, SIGNAL(currentChanged(int)), this, SLOT(changeMiniMapSource(int)));
	connect(ui.tabs, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));

	connect(ui.actionCheckout, SIGNAL(triggered()), this, SLOT(doCheckout()));
	connect(ui.actionCommit, SIGNAL(triggered()), this, SLOT(doCommit()));
	connect(ui.actionExport_to_XMI, SIGNAL(triggered()), this, SLOT(exportToXmi()));
	connect(ui.actionGenerate_to_Java, SIGNAL(triggered()), this, SLOT(generateToJava()));
	connect(ui.actionGenerate_to_Hascol, SIGNAL(triggered()), this, SLOT(generateToHascol()));
	connect(ui.actionShape_Edit, SIGNAL(triggered()), this, SLOT(openShapeEditor()));
	connect(ui.actionGenerate_Editor, SIGNAL(triggered()), this, SLOT(generateEditor()));
	connect(ui.actionGenerate_Editor_qrmc, SIGNAL(triggered()), this, SLOT(generateEditorWithQRMC()));
	connect(ui.actionParse_Editor_xml, SIGNAL(triggered()), this, SLOT(parseEditorXml()));
	connect(ui.actionPreferences, SIGNAL(triggered()), this, SLOT(showPreferencesDialog()));

	connect(ui.actionParse_Hascol_sources, SIGNAL(triggered()), this, SLOT(parseHascol()));
	connect(ui.actionParse_Java_Libraries, SIGNAL(triggered()), this, SLOT(parseJavaLibraries()));

	connect(ui.actionPlugins, SIGNAL(triggered()), this, SLOT(settingsPlugins()));
	connect(ui.actionShow_grid, SIGNAL(toggled(bool)), this, SLOT(showGrid(bool)));
	connect(ui.actionShow_alignment, SIGNAL(toggled(bool)), this, SLOT(showAlignment(bool)));
	connect(ui.actionSwitch_on_grid, SIGNAL(toggled(bool)), this, SLOT(switchGrid(bool)));
	connect(ui.actionSwitch_on_alignment, SIGNAL(toggled(bool)), this, SLOT(switchAlignment(bool)));

	connect(ui.actionHelp, SIGNAL(triggered()), this, SLOT(showHelp()));
	connect(ui.actionAbout, SIGNAL(triggered()), this, SLOT(showAbout()));
	connect(ui.actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

	connect(ui.actionShow, SIGNAL(triggered()), this, SLOT(showGestures()));

	connect(ui.minimapZoomSlider, SIGNAL(valueChanged(int)), this, SLOT(adjustMinimapZoom(int)));

	connect(ui.actionDebug, SIGNAL(triggered()), this, SLOT(debug()));
	connect(ui.actionDebug_Single_step, SIGNAL(triggered()), this, SLOT(debugSingleStep()));

	connect(ui.actionClear, SIGNAL(triggered()), this, SLOT(exterminate()));

	adjustMinimapZoom(ui.minimapZoomSlider->value());
	initGridProperties();

	// Step 3: Ui connects are done.
	progress->setValue(40);

	ui.paletteDock->setWidget(ui.paletteToolbox);
	ui.errorDock->setWidget(ui.errorListWidget);
	ui.errorListWidget->init(this);
	ui.errorDock->setVisible(false);
	ui.propertyEditor->setModel(&mPropertyModel);
	ui.propertyEditor->verticalHeader()->hide();
	ui.propertyEditor->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
	ui.propertyEditor->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
	ui.propertyEditor->setItemDelegate(&mDelegate);

	connect(ui.graphicalModelExplorer, SIGNAL(clicked(QModelIndex const &)), this, SLOT(graphicalModelExplorerClicked(QModelIndex)));
	connect(ui.logicalModelExplorer, SIGNAL(clicked(QModelIndex const &)), this, SLOT(logicalModelExplorerClicked(QModelIndex)));

	ui.graphicalModelExplorer->addAction(ui.actionDeleteFromDiagram);
	ui.logicalModelExplorer->addAction(ui.actionDeleteFromDiagram);

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
	settings.endGroup();

	QString workingDir = settings.value("workingDir", ".").toString();

	mRootIndex = QModelIndex();
	mModels = new models::Models(workingDir, mEditorManager);

	// Step 6: Save loaded, models initialized.
	progress->setValue(80);

	mListenerManager = new ListenerManager(mEditorManager.listeners()
			, mModels->logicalModelAssistApi(), mModels->graphicalModelAssistApi());

	IdList missingPlugins = mEditorManager.checkNeededPlugins(mModels->logicalRepoApi(), mModels->graphicalRepoApi());
	if (!missingPlugins.isEmpty()) {
		QString text = "These plugins are not present, but needed to load the save:\n";
		foreach (Id const id, missingPlugins)
			text += id.editor() + "\n";
		QMessageBox::warning(this, tr("Some plugins are missing"), text);
		close();
		return;
	}

	mPropertyModel.setSourceModels(mModels->logicalModel(), mModels->graphicalModel());

	connect(&mModels->graphicalModelAssistApi(), SIGNAL(nameChanged(Id const &)), this, SLOT(updateTabName(Id const &)));

	ui.graphicalModelExplorer->setModel(mModels->graphicalModel());
	ui.logicalModelExplorer->setModel(mModels->logicalModel());

	mGesturesWidget = new GesturesWidget();
	mVisualDebugger = new VisualDebugger(mModels->graphicalModelAssistApi());

	mDelegate.init(this, &mModels->logicalModelAssistApi());

	// Step 7: Save consistency checked, interface is initialized with models.
	progress->setValue(100);
	if (showSplash)
		splash->close();
	delete splash;

	if (mModels->graphicalModel()->rowCount() > 0)
		openNewTab(mModels->graphicalModel()->index(0, 0, QModelIndex()));

	if (settings.value("diagramCreateSuggestion", true).toBool())
		suggestToCreateDiagram();

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
			ui.paletteToolbox->addDiagramType(diagram, mEditorManager.friendlyName(diagram) );

			foreach (Id const element, mEditorManager.elements(diagram)) {
				ui.paletteToolbox->addItemType(element, mEditorManager.friendlyName(element), mEditorManager.description(element), mEditorManager.icon(element));
			}
		}
	}
	ui.paletteToolbox->initDone();
}

void MainWindow::adjustMinimapZoom(int zoom)
{
	ui.minimapView->resetMatrix();
	ui.minimapView->scale(0.01*zoom,0.01*zoom);
}

void MainWindow::selectItemWithError(Id const &id)
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
		ui.tabs->setCurrentIndex(numTab);
	else
		openNewTab(idx);
	if (ui.tabs->isEnabled()) {
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
				ui.graphicalModelExplorer->setCurrentIndex(index);
			}
		} else {
			ui.graphicalModelExplorer->setCurrentIndex(QModelIndex());
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
	QString const dirName = QFileDialog::getExistingDirectory(this, dialogWindowTitle,".", QFileDialog::ShowDirsOnly);

	if (dirName.isEmpty())
		return "";

	QSettings settings("SPbSU", "QReal");
	settings.setValue("workingDir", dirName);

	return dirName;
}

void MainWindow::open()
{
	QString const dirName = getWorkingDir(tr("Select directory with a save to open"));

	if (dirName.isEmpty())
		return;

	mModels->repoControlApi().open(dirName);
	mModels->reinit();
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
	QModelIndex index = isLogicalModel ? (ui.logicalModelExplorer->currentIndex())
			: (ui.graphicalModelExplorer->currentIndex());
	closeTab(index);
	if (index.isValid()) {
		PropertyEditorModel* pModel = dynamic_cast<PropertyEditorModel*>(ui.propertyEditor->model());
		if (pModel->isCurrentIndex(index)) {
			pModel->clearModelIndexes();
			ui.propertyEditor->setRootIndex(QModelIndex());
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
			PropertyEditorModel* pModel = dynamic_cast<PropertyEditorModel*>(ui.propertyEditor->model());
			if (pModel->isCurrentIndex(index))
				pModel->clearModelIndexes();
			ui.propertyEditor->setRootIndex(QModelIndex());
			mModels->graphicalModel()->removeRow(index.row(), index.parent());
		}
	}
}

void MainWindow::deleteFromDiagram()
{
	bool isLogicalModel = false;
	if (mModels->graphicalModel()) {
		if (ui.graphicalModelExplorer->hasFocus()) {
			isLogicalModel = false;
			deleteFromExplorer(isLogicalModel);
		} else if (getCurrentTab() != NULL && getCurrentTab()->hasFocus()) {
			deleteFromScene();
		}
	}
	if (mModels->logicalModel()) {
		if (ui.logicalModelExplorer->hasFocus()) {
			isLogicalModel = true;
			deleteFromExplorer(isLogicalModel);
		}
	}
}

void MainWindow::showAbout()
{
	QMessageBox::about(this, tr("About QReal"),
			tr("<center>This is <b>QReal</b><br>"
			"Just another CASE tool</center>"));
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

void MainWindow::checkoutDialogOk()
{
}

void MainWindow::checkoutDialogCancel()
{
}

void MainWindow::doCheckout()
{
	QString path;
	QString url;
	CheckoutDialog *dialog = new CheckoutDialog(this);
	connect(dialog, SIGNAL(accepted()), this, SLOT(checkoutDialogOk()));
	connect(dialog, SIGNAL(rejected()), this, SLOT(checkoutDialogCancel()));
	dialog->show();
	if (dialog->Accepted)
	{
		path = dialog->getDir();
		url = dialog->getUrl();
	}
}

void MainWindow::doCommit()
{
	QString select = tr("Select directory to commit");
	QString path = QFileDialog::getExistingDirectory(this, select);

	if (path.isEmpty())
		return;
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
	errors.showErrors(ui.errorListWidget, ui.errorDock);
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
		QMessageBox::warning(this, tr("error"), "Cannot find the directory for saving");
		return;
	}
	foreach (Id const key, metamodelList.keys()) {
		dir.mkdir(directoryXml.absolutePath() + "/qrxml/" + metamodelList[key]);
		gui::ErrorReporter& errors = editorGenerator.generateEditor(key, directoryName + "/qrxml/" + metamodelList[key] + "/" + metamodelList[key]);

		if (errors.showErrors(ui.errorListWidget, ui.errorDock)) {
			if (QMessageBox::question(this, tr("loading.."), QString("Do you want to load generated editor %1?").arg(metamodelList[key]),
					QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
				return;
			QSettings settings("SPbSU", "QReal");
			loadNewEditor(directoryName + "/qrxml/", metamodelList[key], settings.value("pathToQmake", "").toString(),
					settings.value("pathToMake", "").toString(), settings.value("pluginExtension", "").toString(), settings.value("prefix", "").toString());
		}
	}
}

void MainWindow::generateEditorWithQRMC()
{
	qrmc::MetaCompiler metaCompiler("../qrmc", "./save");

	IdList const metamodels = mModels->logicalRepoApi().children(Id::rootId());

	QSettings settings("SPbSU", "QReal");

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
			if (QMessageBox::question(this, tr("loading.."), QString("Do you want to compile and load editor %1?").arg(name),
									  QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
			{
				continue;
			}

			progress->setValue(5);

			if (!metaCompiler.compile(name)) { // generating source code for all metamodels
				QMessageBox::warning(this, "error", "cannot generate source code for editor " + name);
				qDebug() << "compilation failed";
				continue;
			}
			progress->setValue(20);

			QProcess builder;
			builder.setWorkingDirectory("../qrmc/plugins");
			builder.start(settings.value("pathToQmake", "").toString());
			qDebug()  << "qmake";
			if ((builder.waitForFinished()) && (builder.exitCode() == 0)) {
				progress->setValue(40);

				builder.start(settings.value("pathToMake", "").toString());

				bool finished = builder.waitForFinished(100000);
				qDebug()  << "make";
				if (finished && (builder.exitCode() == 0)) {
					qDebug()  << "make ok";

					progress->setValue(progress->value() + forEditor/2);

					QString normalizedName = name.at(0).toUpper() + name.mid(1);
					if (!name.isEmpty()) {
						if (mEditorManager.editors().contains(Id(normalizedName))) {
							foreach (Id const diagram, mEditorManager.diagrams(Id(normalizedName)))
								ui.paletteToolbox->deleteDiagramType(diagram);

							if (!mEditorManager.unloadPlugin(normalizedName)) {
								QMessageBox::warning(this, "error", "cannot unload plugin " + normalizedName);
								progress->close();
								delete progress;
								continue;
							}
						}
						if (mEditorManager.loadPlugin(settings.value("prefix", "").toString() + name + "." + settings.value("pluginExtension", "").toString())) {
							foreach (Id const diagram, mEditorManager.diagrams(Id(normalizedName))) {
								ui.paletteToolbox->addDiagramType(diagram, mEditorManager.friendlyName(diagram));

								foreach (Id const element, mEditorManager.elements(diagram))
									ui.paletteToolbox->addItemType(element, mEditorManager.friendlyName(element), mEditorManager.description(element), mEditorManager.icon(element));
							}
						}
					}
					progress->setValue(progress->value() + forEditor/2);
				}
			}
			ui.paletteToolbox->initDone();
			progress->setValue(100);

		}
	}
	if (progress->value() != 100)
		QMessageBox::warning(this, tr("error"), "cannot load new editor");
	progress->setValue(100);
	progress->close();
	delete progress;
}

void MainWindow::loadNewEditor(const QString &directoryName, const QString &metamodelName,
		const QString &commandFirst, const QString &commandSecond, const QString &extension, const QString &prefix)
{
	int const progressBarWidth = 240;
	int const progressBarHeight = 20;

	if ((commandFirst == "") || (commandSecond == "") || (extension == "")) {
		QMessageBox::warning(this, tr("error"), "please, fill compiler settings");
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
			ui.paletteToolbox->deleteDiagramType(diagram);

		if (!mEditorManager.unloadPlugin(normalizeDirName)) {
			QMessageBox::warning(this, "error", "cannot unload plugin");
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
					ui.paletteToolbox->addDiagramType(diagram, mEditorManager.friendlyName(diagram));

					foreach (Id const element, mEditorManager.elements(diagram))
						ui.paletteToolbox->addItemType(element, mEditorManager.friendlyName(element), mEditorManager.description(element), mEditorManager.icon(element));
				}
				ui.paletteToolbox->initDone();
				progress->setValue(100);
			}
		}
	}
	if (progress->value() != 100)
		QMessageBox::warning(this, tr("error"), "cannot load new editor");
	progress->setValue(100);
	progress->close();
	delete progress;
}

void MainWindow::parseEditorXml()
{
	if (!mEditorManager.editors().contains(Id("Meta_editor"))) {
		QMessageBox::warning(this, tr("error"), "required plugin is not loaded");
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
	return dynamic_cast<EditorView *>(ui.tabs->currentWidget());
}

void MainWindow::changeMiniMapSource(int index)
{
	if (index != -1) {
		ui.tabs->setEnabled(true);
		EditorView *editorView = getCurrentTab();
		setConnectActionZoomTo(ui.tabs->currentWidget());

		if (editorView != NULL && (static_cast<EditorViewScene*>(editorView->scene()))->mainWindow() != NULL)
		{
			ui.minimapView->setScene(editorView->scene());
			getCurrentTab()->mvIface()->setModel(mModels->graphicalModel());
			getCurrentTab()->mvIface()->setLogicalModel(mModels->logicalModel());
			mRootIndex = editorView->mvIface()->rootIndex();
		}
	} else
	{
		ui.tabs->setEnabled(false);
		ui.minimapView->setScene(0);;
	}
	emit rootDiagramChanged();
}

void qReal::MainWindow::closeTab(int index)
{
	QWidget *widget = ui.tabs->widget(index);
	ui.tabs->removeTab(index);
	delete widget;
}

void MainWindow::exterminate()
{
	int tabCount = ui.tabs->count();
	for (int i = 0; i < tabCount; i++)
		closeTab(i);
	mModels->repoControlApi().exterminate();
	mModels->reinit();

	PropertyEditorModel* pModel = dynamic_cast<PropertyEditorModel*>(ui.propertyEditor->model());
	pModel->clearModelIndexes();
	ui.propertyEditor->setRootIndex(QModelIndex());
}

void MainWindow::parseHascol()
{
	QStringList const fileNames = QFileDialog::getOpenFileNames(this, tr("Select Hascol files to parse"), ".", "*.md;;*.*");
	if (fileNames.empty())
		return;

	parsers::HascolParser parser(mModels->mutableLogicalRepoApi(), mEditorManager);
	gui::ErrorReporter& errors = parser.parse(fileNames);

	errors.showErrors(ui.errorListWidget, ui.errorDock);

	mModels->reinit();
}

void MainWindow::showPreferencesDialog()
{
	PreferencesDialog preferencesDialog(ui.actionShow_grid, ui.actionShow_alignment, ui.actionSwitch_on_grid, ui.actionSwitch_on_alignment);
	preferencesDialog.exec();
}

void MainWindow::openShapeEditor()
{
	QObject const *object = sender();
	OpenShapeEditorButton const *button = dynamic_cast<OpenShapeEditorButton const *>(object);
	QString const text = "Shape Editor";
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

	ui.tabs->addTab(shapeEdit, text);
	ui.tabs->setCurrentWidget(shapeEdit);
	setConnectActionZoomTo(shapeEdit);
}

void MainWindow::disconnectZoom(QGraphicsView* view)
{
	disconnect(ui.actionZoom_In, SIGNAL(triggered()), view, SLOT(zoomIn()));
	disconnect(ui.actionZoom_Out, SIGNAL(triggered()), view, SLOT(zoomOut()));
}

void MainWindow::connectZoom(QGraphicsView* view)
{
	connect(ui.actionZoom_In, SIGNAL(triggered()), view, SLOT(zoomIn()));
	connect(ui.actionZoom_Out, SIGNAL(triggered()), view, SLOT(zoomOut()));
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
	for (int i = 0; i < ui.tabs->count(); i++)
		disconnectActionZoomTo(ui.tabs->widget(i));

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

		float const widthTab = ui.tabs->size().width();
		float const heightTab = ui.tabs->size().height();
		float const widthEl = element->boundingRect().width();
		float const heightEl = element->boundingRect().height();
		view->ensureVisible(element, (widthTab - widthEl) / 2, (heightTab - heightEl) / 2);
	}
}

void MainWindow::propertyEditorScrollTo(const QModelIndex &index)
{
	ui.propertyEditor->scrollTo(index);
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
	for (int i = 0; i < ui.tabs->count(); i++) {
		EditorView *tab = (dynamic_cast<EditorView *>(ui.tabs->widget(i)));
		if (tab != NULL && tab->mvIface()->rootIndex() == index) {
			tabNumber = i;
			break;
		}
	}
	if (tabNumber != -1) {
		ui.tabs->setCurrentIndex(tabNumber);
	} else {
		EditorView *view = new EditorView();
		ui.tabs->addTab(view, index.data().toString());
		ui.tabs->setCurrentWidget(view);
		initCurrentTab(index);
	}

	//changing of palette active editor:
	QSettings settings("SPbSU", "QReal");
	if (settings.value("PaletteTabSwitching", true).toBool())
	{
		int i = 0;
		foreach(QString name, ui.paletteToolbox->getTabNames()) {
			Id const id = mModels->graphicalModelAssistApi().idByIndex(index);
			Id const diagramId = Id(id.editor(), id.diagram());
			QString const diagramName = mEditorManager.friendlyName(diagramId);
			if (diagramName == name) {
				ui.paletteToolbox->getComboBox()->setCurrentIndex(i);
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
	connect(ui.actionAntialiasing, SIGNAL(toggled(bool)), getCurrentTab(), SLOT(toggleAntialiasing(bool)));
	connect(ui.actionOpenGL_Renderer, SIGNAL(toggled(bool)), getCurrentTab(), SLOT(toggleOpenGL(bool)));

	getCurrentTab()->mvIface()->setModel(mModels->graphicalModel());
	getCurrentTab()->mvIface()->setLogicalModel(mModels->logicalModel());
	getCurrentTab()->mvIface()->setRootIndex(index);
	changeMiniMapSource(ui.tabs->currentIndex());

	connect(mModels->graphicalModel(), SIGNAL(rowsAboutToBeMoved(QModelIndex, int, int, QModelIndex, int))
			, getCurrentTab()->mvIface(), SLOT(rowsAboutToBeMoved(QModelIndex, int, int, QModelIndex, int)));
	connect(mModels->graphicalModel(), SIGNAL(rowsMoved(QModelIndex, int, int, QModelIndex, int))
			, getCurrentTab()->mvIface(), SLOT(rowsMoved(QModelIndex, int, int, QModelIndex, int)));
}

void MainWindow::updateTabName(Id const &id)
{
	for (int i = 0; i < ui.tabs->count(); i++) {
		EditorView *tab = (static_cast<EditorView *>(ui.tabs->widget(i)));
		if (tab->mvIface()->rootIndex() == mModels->graphicalModelAssistApi().indexById(id)) {
			ui.tabs->setTabText(i, mModels->graphicalModelAssistApi().name(id));
			return;
		}
	}
}

void MainWindow::closeTab(QModelIndex const &index)
{
	for (int i = 0; i < ui.tabs->count(); i++) {
		EditorView *tab = (static_cast<EditorView *>(ui.tabs->widget(i)));
		if (tab->mvIface()->rootIndex() == index) {
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
	for (int i = 0; i < ui.tabs->count(); i++) {
		EditorView *tab = (dynamic_cast<EditorView *>(ui.tabs->widget(i)));
		if (tab != NULL)
			tab->setDrawSceneGrid(isChecked);
	}
}

void MainWindow::setShowAlignment(bool isChecked)
{
	for (int i = 0; i < ui.tabs->count(); i++) {
		EditorView *tab = (dynamic_cast<EditorView *>(ui.tabs->widget(i)));
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
	for (int i = 0; i < ui.tabs->count(); i++) {
		EditorView *tab = (dynamic_cast<EditorView *>(ui.tabs->widget(i)));
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
	for (int i = 0; i < ui.tabs->count(); i++) {
		EditorView *tab = (dynamic_cast<EditorView *>(ui.tabs->widget(i)));
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
	QString text = "Gestures Show";
	mGesturesWidget = new GesturesWidget();
	ui.tabs->addTab(mGesturesWidget, text);
	ui.tabs->setCurrentWidget(mGesturesWidget);
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
	dialog.setWindowTitle("Choose new diagram");

	QLabel label("There is no existing diagram,\n choose diagram you want work with:");
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
	cancelButton.setText("Cancel");
	QPushButton okButton;
	okButton.setText("Done");

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
		deleteFromExplorer(false);
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
	ui.graphicalModelExplorer->setCurrentIndex(index);
	Id const logicalIdCreated = mModels->graphicalModelAssistApi().logicalId(created);
	QModelIndex const logicalIndex = mModels->logicalModelAssistApi().indexById(logicalIdCreated);
	ui.logicalModelExplorer->setCurrentIndex(logicalIndex);
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
	for (int i = 0; i < ui.tabs->count(); ++i)
	{
		EditorView *editor = dynamic_cast<EditorView *>(ui.tabs->widget(i));
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
	ui.actionSwitch_on_grid->blockSignals(false);
	ui.actionSwitch_on_grid->setChecked(settings.value("ActivateGrid", false).toBool());

	ui.actionShow_grid->blockSignals(false);
	ui.actionShow_grid->setChecked(settings.value("ShowGrid", true).toBool());
}

void MainWindow::debug()
{
	EditorView *editor = dynamic_cast<EditorView *>(ui.tabs->widget(ui.tabs->currentIndex()));
	mVisualDebugger->setEditor(editor);
	if (mVisualDebugger->canDebug(VisualDebugger::fullDebug)) {
		gui::ErrorReporter &errorReporter = mVisualDebugger->debug();
		errorReporter.showErrors(ui.errorListWidget, ui.errorDock);
		mVisualDebugger->clearErrorReporter();
	}
}

void MainWindow::debugSingleStep()
{
	EditorView *editor = dynamic_cast<EditorView *>(ui.tabs->widget(ui.tabs->currentIndex()));
	mVisualDebugger->setEditor(editor);
	if (mVisualDebugger->canDebug(VisualDebugger::singleStepDebug)) {
		gui::ErrorReporter &errorReporter = mVisualDebugger->debugSingleStep();
		errorReporter.showErrors(ui.errorListWidget, ui.errorDock);
		mVisualDebugger->clearErrorReporter();
	}
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
	return ui.actionDeleteFromDiagram;
}

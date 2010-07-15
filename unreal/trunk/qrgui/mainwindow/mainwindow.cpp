#include "mainwindow.h"

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSvg/QSvgGenerator>
#include <QtCore/QPluginLoader>
#include <QtGui/QProgressBar>
#include <QtCore/QSettings>
#include <QtGui/QPrinter>
#include <QtGui/QPrintDialog>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

#include <QtCore/QDebug>

#include "../dialogs/plugindialog.h"
#include "editorInterface.h"
#include "../model/model.h"
#include "../view/editorview.h"
#include "../umllib/uml_element.h"
#include "../generators/xmi/xmiHandler.h"
#include "../generators/metaGenerator/metaGenerator.h"
#include "../generators/java/javaHandler.h"
#include "../generators/hascol/hascolGenerator.h"
#include "../dialogs/editorGeneratorDialog.h"
#include "../parsers/hascol/hascolParser.h"
#include "../parsers/xml/xmlParser.h"
#include "errorReporter.h"
#include "../editorManager/listenerManager.h"
#include "shapeEdit/shapeEdit.h"
#include "gesturesShow/gestureswidget.h"
#include "openShapeEditorButton.h"
#include "../qrrepo/svnClient.h"


using namespace qReal;

MainWindow::MainWindow()
	: mListenerManager(NULL), mPropertyModel(mEditorManager), isSave(0)
{
	mbox.setText("The document has been modified.");
	mbox.setInformativeText("Do you want to save your changes?");
	mbox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard
							| QMessageBox::Cancel);

	connect(&mbox, SIGNAL(buttonClicked(QAbstractButton*)),
			this, SLOT(slotInc(QAbstractButton*)));
	connect(&mbox, SIGNAL(buttonClicked(QAbstractButton*)),
			&mbox, SLOT(close()));
	connect(&mbox, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(close()));
	QSettings settings("SPbSU", "QReal");
	bool showSplash = settings.value("ShowSplashScreen", true).toBool();
	QSplashScreen* splash =
			new QSplashScreen(QPixmap(":/icons/kroki2.PNG"), Qt::SplashScreen | Qt::WindowStaysOnTopHint);

	QProgressBar *progress = new QProgressBar((QWidget*) splash);
	progress->move(20,270);
	progress->setFixedWidth(600);
	progress->setFixedHeight(15);
	progress->setRange(0, 100);
	progress->setValue(5);
	if (showSplash)
	{
		splash->show();
		QApplication::processEvents();
	}

	ui.setupUi(this);
	ui.tabs->setTabsClosable(true);
	ui.tabs->setMovable(true);

	if (!showSplash)
		ui.actionShowSplash->setChecked(false);

	ui.minimapView->setRenderHint(QPainter::Antialiasing, true);

	progress->setValue(20);

	connect(ui.actionQuit, SIGNAL(triggered()), this, SLOT(close()));

	connect(ui.actionShowSplash, SIGNAL(toggled(bool)), this, SLOT (toggleShowSplash(bool)));

	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(open()));
	connect(ui.actionSave, SIGNAL(triggered()), this, SLOT(save()));
	connect(ui.actionSave_as, SIGNAL(triggered()), this, SLOT(saveAs()));
	connect(ui.actionPrint, SIGNAL(triggered()), this, SLOT(print()));
	connect(ui.actionMakeSvg, SIGNAL(triggered()), this, SLOT(makeSvg()));

	connect(ui.actionDeleteFromDiagram, SIGNAL(triggered()), this, SLOT(deleteFromDiagram()));

	connect(ui.tabs, SIGNAL(currentChanged(int)), this, SLOT(changeMiniMapSource(int)));
	connect(ui.tabs, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));

	connect(ui.actionCommit, SIGNAL(triggered()), this, SLOT(doCommit()));
	connect(ui.actionExport_to_XMI, SIGNAL(triggered()), this, SLOT(exportToXmi()));
	connect(ui.actionGenerate_to_Java, SIGNAL(triggered()), this, SLOT(generateToJava()));
	connect(ui.actionGenerate_editor, SIGNAL(triggered()), this, SLOT(generateEditor()));
	connect(ui.actionGenerate_to_Hascol, SIGNAL(triggered()), this, SLOT(generateToHascol()));
	connect(ui.actionShape_Edit, SIGNAL(triggered()), this, SLOT(openNewEmptyTab()));
	connect(ui.actionGenerate_Editor, SIGNAL(triggered()), this, SLOT(newGenerateEditor()));
	connect(ui.actionParse_Editor_xml, SIGNAL(triggered()), this, SLOT(parseEditorXml()));

	connect(ui.actionParse_Hascol_sources, SIGNAL(triggered()), this, SLOT(parseHascol()));
	connect(ui.actionParse_Java_Libraries, SIGNAL(triggered()), this, SLOT(parseJavaLibraries()));

	connect(ui.actionPlugins, SIGNAL(triggered()), this, SLOT(settingsPlugins()));
	connect(ui.actionShow_grid, SIGNAL(triggered()), this, SLOT(showGrid()));
	connect(ui.actionSwitch_on_grid, SIGNAL(toggled(bool)), this, SLOT(switchGrid(bool)));

	connect(ui.actionHelp, SIGNAL(triggered()), this, SLOT(showHelp()));
	connect(ui.actionAbout, SIGNAL(triggered()), this, SLOT(showAbout()));
	connect(ui.actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

	connect(ui.actionShow, SIGNAL(triggered()), this, SLOT(showGestures()));


	connect(ui.minimapZoomSlider, SIGNAL(valueChanged(int)), this, SLOT(adjustMinimapZoom(int)));
	adjustMinimapZoom(ui.minimapZoomSlider->value());

	progress->setValue(40);

	// XXX: kludge... don't know how to do it in designer
	ui.diagramDock->setWidget(ui.diagramExplorer);

	ui.paletteDock->setWidget(ui.paletteToolbox);

	ui.propertyEditor->setModel(&mPropertyModel);
	ui.propertyEditor->verticalHeader()->hide();
	ui.propertyEditor->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
	ui.propertyEditor->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
	ui.propertyEditor->setItemDelegate(&mDelegate);
	mDelegate.setMainWindow(this);

	connect(ui.diagramExplorer, SIGNAL(clicked(QModelIndex const &)),
			&mPropertyModel, SLOT(setIndex(QModelIndex const &)));

	connect(ui.diagramExplorer, SIGNAL(clicked(QModelIndex const &)),
			this, SLOT(openNewTab(QModelIndex const &)));

	ui.diagramExplorer->addAction(ui.actionDeleteFromDiagram);
	settings.beginGroup("MainWindow");
	resize(settings.value("size", QSize(1024, 800)).toSize());
	move(settings.value("pos", QPoint(0, 0)).toPoint());
	settings.endGroup();
	progress->setValue(60);

	loadPlugins();
	showMaximized();

	progress->setValue(70);

	QString workingDir = settings.value("workingDir", "./save").toString();

	mModel = new model::Model(mEditorManager, workingDir);
	IdList missingPlugins = mEditorManager.checkNeededPlugins(mModel->api());
	if (!missingPlugins.isEmpty()) {
		QString text = "These plugins are not present, but needed to load the save:\n";
		foreach (Id const id, missingPlugins) {
			text += id.editor() + "\n";
		}
		QMessageBox::warning(this, tr("Some plugins are missing"), text);
		close();
		return;
	}

	mListenerManager = new ListenerManager(mEditorManager.listeners(), &mModel->assistApi());
		this->mGesturesWidget = new GesturesWidget();

	connect(ui.actionClear, SIGNAL(triggered()), this, SLOT(exterminate()));

	progress->setValue(80);

	mPropertyModel.setSourceModel(mModel);
	ui.diagramExplorer->setModel(mModel);
	ui.diagramExplorer->setRootIndex(mModel->rootIndex());

	connect(mModel, SIGNAL(nameChanged(QModelIndex const &)), this, SLOT(updateTab(QModelIndex const &)));

	if (mModel->rowCount() > 0)
		openNewTab(mModel->index(0, 0, QModelIndex()));

	progress->setValue(100);
	if (showSplash)
		splash->close();
	delete splash;
}

void MainWindow::slotInc(QAbstractButton* button)
{
	QString txt = button->text();
	if (txt == "Save")
		isSave = 1;
	if (txt == "Cancel")
		isSave = 3;
	if (txt == "Discard")
		isSave = 2;
}

void MainWindow::keyPressEvent(QKeyEvent *keyEvent)
{
	if (keyEvent->modifiers() == Qt::AltModifier && keyEvent->key() == Qt::Key_X){
		close();
	} else if ( keyEvent->key() == Qt::Key_F2 ){
		save();
	}
}

MainWindow::~MainWindow()
{
	if (isSave == 1)
		mModel->save();
	delete mModel;
	delete mListenerManager;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	QSettings settings("SPbSU", "QReal");
	settings.beginGroup("MainWindow");
	settings.setValue("size", size());
	settings.setValue("pos", pos());
	settings.endGroup();
	if (mModel->isChanged())
	{
		if (isSave && (isSave != 3))
			event->accept();
		else
		{
			if (isSave == 3)
			{
				event->ignore();
				isSave = 0;
			}
			else
			{
				mbox.show();
				event->ignore();
			}
		}
	}
}

void MainWindow::loadPlugins()
{
	foreach (Id const editor, mEditorManager.editors()) {
		foreach (Id const diagram, mEditorManager.diagrams(editor)) {
			ui.paletteToolbox->addDiagramType(diagram, mEditorManager.friendlyName(diagram) );

			foreach (Id const element, mEditorManager.elements(diagram)) {
				ui.paletteToolbox->addItemType(element, mEditorManager.friendlyName(element), mEditorManager.icon(element));
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

void MainWindow::activateItemOrDiagram(const QModelIndex &idx)
{
	QModelIndex parent = idx.parent();

	if (ui.tabs->isEnabled())
	{
		if (parent == mModel->rootIndex()) {
			getCurrentTab()->mvIface()->setRootIndex(idx);
		} else {
			getCurrentTab()->mvIface()->setRootIndex(parent);
			// select this item on diagram
			getCurrentTab()->scene()->clearSelection();
			UML::Element *e = (static_cast<EditorViewScene *>(getCurrentTab()->scene()))->getElemByModelIndex(idx);
			if (e)
				e->setSelected(true);
			else
				qDebug() << "shit happened!!!\n";
		}
	}
}

void MainWindow::activateItemOrDiagram(Id const &id) {
	activateItemOrDiagram(mModel->indexById(id));
}

void MainWindow::activateSubdiagram(QModelIndex const &idx) {
	// end-to-end links: if there's a first-level diagram with the same name as
	// this element, show it
	QString targetName = mModel->data(idx, Qt::DisplayRole).toString();
	int rows = mModel->rowCount(mModel->rootIndex());
	for (int i = 0; i < rows; ++i) {
		QModelIndex child = mModel->index(i, 0, mModel->rootIndex());
		if (mModel->data(child, Qt::DisplayRole).toString() == targetName)
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
			if (elem->index().isValid()) {
				ui.diagramExplorer->setCurrentIndex(elem->index());
				mPropertyModel.setIndex(elem->index());
			}
		} else {
			ui.diagramExplorer->setCurrentIndex(QModelIndex());
			mPropertyModel.setIndex(QModelIndex());

			foreach(QGraphicsItem* item, graphicsItems) {
				UML::EdgeElement* edge = dynamic_cast<UML::EdgeElement*>(item);
				if (edge) {
					length--;
					graphicsItems.removeOne(edge);
				}
			}
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

	mModel->open(dirName);
}

void MainWindow::save()
{
	mModel->api().save();
}

void MainWindow::saveAs()
{
	QString const dirName = getWorkingDir(tr("Select directory to save current model to"));

	if (dirName.isEmpty())
		return;

	mModel->saveTo(dirName);
}

void MainWindow::setShape(const QString &data, const QPersistentModelIndex &index, const int &role)
{
	mModel->setData(index, data, role);
	qDebug() << "data=" << mModel->data(index, role);
}

void MainWindow::print()
{
	QPrinter printer(QPrinter::HighResolution);
	QPrintDialog dialog(&printer, this);
	if (dialog.exec() == QDialog::Accepted) {
		QPainter painter(&printer);
		//		QRect allScene = pieChart->mapFromScene(pieChart->scene()->sceneRect()).boundingRect();
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
	newSvg.setSize(QSize(800,600));

	QPainter painter(&newSvg);
	getCurrentTab()->scene()->render(&painter);
}

void MainWindow::settingsPlugins()
{
	PluginDialog dialog(mEditorManager , this);
	dialog.exec();
}

void MainWindow::deleteFromExplorer()
{
	QModelIndex idx = ui.diagramExplorer->currentIndex();
	closeTab(idx);
	if (idx.isValid())
		mModel->removeRow(idx.row(), idx.parent());
}

void MainWindow::deleteFromScene()
{
	foreach (QGraphicsItem *item, getCurrentTab()->scene()->selectedItems())
		deleteFromScene(item);
}

void MainWindow::deleteFromScene(QGraphicsItem *target)
{
	if (UML::Element *elem = dynamic_cast<UML::Element *>(target))
	{
		qDebug() << "Deleting object, uuid: " << elem->uuid().toString();
		if (elem->index().isValid())
			mModel->removeRow(elem->index().row(), elem->index().parent());
	}
}

void MainWindow::deleteFromDiagram()
{
	if (mModel) {
		if (ui.diagramExplorer->hasFocus()) {
			deleteFromExplorer();
		} else if (getCurrentTab()->hasFocus()) {
			deleteFromScene();
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
	settings.setValue("ShowSplashScreen", show);
}

void MainWindow::doCommit()
{
//	QString const Path = getWorkingDir(tr("Select directory with working copy"));
//	QString const path = QFileDialog::getExistingDirectory(this, dialogWindowTitle,".", QFileDialog::ShowDirsOnly);
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
	generators::XmiHandler xmi(mModel->api());

	QString const fileName = QFileDialog::getSaveFileName(this);
	if (fileName.isEmpty())
		return;

	QString const errors = xmi.exportToXmi(fileName);

	if (!errors.isEmpty()) {
		QMessageBox::warning(this, tr("errors"), "Some errors occured. Export may be incorrect. Errors list: \n" + errors);
	} else {
		QMessageBox::information(this, tr("finished"), "Export is finished");
	}

	qDebug() << "Done.";
}

void MainWindow::generateToJava()
{
	generators::JavaHandler java(mModel->api());

	QString const dirName = QFileDialog::getExistingDirectory(this);
	if (dirName.isEmpty())
		return;

	QString const errors = java.generateToJava(dirName);

	if (!errors.isEmpty()) {
		QMessageBox::warning(this, tr("errors"), "Some errors occured. Export may be incorrect. Errors list: \n" + errors);
	} else {
		QMessageBox::information(this, tr("finished"), "Export is finished");
	}

	qDebug() << "Done.";
}

void MainWindow::parseJavaLibraries()
{
	generators::JavaHandler java(mModel->api());

	QString const dirName = QFileDialog::getExistingDirectory(this);
	if (dirName.isEmpty())
		return;

	QString const errors = java.parseJavaLibraries(dirName);

	if (!errors.isEmpty()) {
		QMessageBox::warning(this, tr("errors"), "Some errors occured. Export may be incorrect. Errors list: \n" + errors);
	} else {
		QMessageBox::information(this, tr("finished"), "Parsing is finished");
	}

	qDebug() << "Done.";
}

void MainWindow::generateToHascol()
{
	generators::HascolGenerator hascolGenerator(mModel->api());

	gui::ErrorReporter const errors = hascolGenerator.generate();
	errors.showErrors("Generation finished successfully");

	qDebug() << "Done.";
}

void MainWindow::newGenerateEditor()
{
	generators::MetaGenerator metaGenerator(mModel->api());

	QString directoryName;
	QFileInfo directoryXml;
	gui::ErrorReporter errors;
	const QHash<Id, QString> metamodelList = metaGenerator.getMetamodelList();
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
		errors = metaGenerator.generateEditor(key, directoryName + "/qrxml/" + metamodelList[key] + "/" + metamodelList[key]);
		errors.showErrors("Generation finished successfully");
	}
}

void MainWindow::parseEditorXml()
{
	if (!mEditorManager.editors().contains(Id("Meta_editor"))) {
		QMessageBox::warning(this, tr("error"), "required plugin is not loaded =Р");
		return;
	}
	QString const fileName = QFileDialog::getOpenFileName(this, tr("Select xml file to parse"));
	if (fileName == "")
		return;

	parsers::XmlParser parser(mModel->mutableApi(), mEditorManager);
	parser.parseFile(fileName);

	mModel->reinit();
}

EditorView * MainWindow::getCurrentTab()
{
	return dynamic_cast<EditorView *>(ui.tabs->currentWidget());
}

void MainWindow::changeMiniMapSource( int index )
{
	if (index != -1) {
		ui.tabs->setEnabled(true);
		EditorView *editorView = getCurrentTab();
		if (editorView != NULL)
			ui.minimapView->setScene(editorView->scene());
	} else
	{
		ui.tabs->setEnabled(false);
		ui.minimapView->setScene(0);;
	}
}

void qReal::MainWindow::closeTab( int index )
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
	mModel->exterminate();
}

void MainWindow::generateEditor()
{
	EditorGeneratorDialog editorGeneratorDialog(mModel->api());
	editorGeneratorDialog.exec();
}

void MainWindow::parseHascol()
{
	QStringList const fileNames = QFileDialog::getOpenFileNames(this, tr("Select Hascol files to parse"), ".", "*.md;;*.*");
	if (fileNames.empty())
		return;

	parsers::HascolParser parser(mModel->mutableApi(), mEditorManager);
	gui::ErrorReporter errors = parser.parse(fileNames);

	errors.showErrors("Parsing is finished");

	mModel->reinit();
}

void MainWindow::openNewEmptyTab()
{
	QObject *object = sender();
	OpenShapeEditorButton *button = dynamic_cast<OpenShapeEditorButton*>(object);
	QString text = "Shape Edit";
	ShapeEdit *shapeEdit;
	if (button != NULL) {
		QPersistentModelIndex index = button->getIndex();
		int role = button->getRole();
		QString propertyValue = button->getPropertyValue();
		shapeEdit = new ShapeEdit(index, role);
		if (propertyValue != "")
			shapeEdit->load(propertyValue);
		mModel->setData(index, propertyValue, role);
		connect(shapeEdit, SIGNAL(shapeSaved(QString, QPersistentModelIndex const &, int const &)), this, SLOT(setShape(QString, QPersistentModelIndex const &, int const &)));
	}
	else {
		shapeEdit = new ShapeEdit();
	}
	ui.tabs->addTab(shapeEdit, text);
	ui.tabs->setCurrentWidget(shapeEdit);
}

void MainWindow::openNewTab(const QModelIndex &index)
{
	if( index.parent() != QModelIndex() ) // only first-level diagrams are opened in new tabs
		return;

	mModel->setRootIndex(index);
	int tabNumber = -1;
	for (int i = 0; i < ui.tabs->count(); i++) {
		EditorView *tab = (static_cast<EditorView *>(ui.tabs->widget(i)));
		if (tab->mvIface()->rootIndex() == index) {
			tabNumber = i;
			break;
		}
	}
	if (tabNumber != -1) {
		ui.tabs->setCurrentIndex(tabNumber);
	} else {
		EditorView *view = new EditorView();
		ui.tabs->addTab(view, mModel->data(index, Qt::EditRole).toString());
		ui.tabs->setCurrentWidget(view);

		//		if (!index.isValid())
		//			index = mModel->rootIndex();
		initCurrentTab(index);
	}
}

void MainWindow::initCurrentTab(const QModelIndex &rootIndex)
{
	getCurrentTab()->setMainWindow(this);
	QModelIndex index = rootIndex;

	changeMiniMapSource(ui.tabs->currentIndex());

	connect(getCurrentTab()->scene(), SIGNAL(selectionChanged()), SLOT(sceneSelectionChanged()));
	connect(ui.actionZoom_In, SIGNAL(triggered()), getCurrentTab(), SLOT(zoomIn()));
	connect(ui.actionZoom_Out, SIGNAL(triggered()), getCurrentTab(), SLOT(zoomOut()));
	connect(ui.actionAntialiasing, SIGNAL(toggled(bool)), getCurrentTab(), SLOT(toggleAntialiasing(bool)));
	connect(ui.actionOpenGL_Renderer, SIGNAL(toggled(bool)), getCurrentTab(), SLOT(toggleOpenGL(bool)));

	getCurrentTab()->mvIface()->setModel(mModel);
	getCurrentTab()->mvIface()->setRootIndex(index);
	connect(mModel, SIGNAL(rowsAboutToBeMoved(QModelIndex, int, int, QModelIndex, int))
			, getCurrentTab()->mvIface(), SLOT(rowsAboutToBeMoved(QModelIndex, int, int, QModelIndex, int)));
	connect(mModel, SIGNAL(rowsMoved(QModelIndex, int, int, QModelIndex, int))
			, getCurrentTab()->mvIface(), SLOT(rowsMoved(QModelIndex, int, int, QModelIndex, int)));

		emit tabOpened();
}

void MainWindow::updateTab(QModelIndex const &index)
{
	for (int i = 0; i < ui.tabs->count(); i++) {
		EditorView *tab = (static_cast<EditorView *>(ui.tabs->widget(i)));
		if (tab->mvIface()->rootIndex() == index) {
			ui.tabs->setTabText(i, mModel->data(index, Qt::EditRole).toString());
			return;
		}
	}
}

void MainWindow::closeTab(QModelIndex const &index)
{
	for (int i = 0; i < ui.tabs->count(); i++) {
		EditorView *tab = (static_cast<EditorView *>(ui.tabs->widget(i)));
		if (tab->mvIface()->rootIndex() == index) {
			qDebug() << " closing tab" << i;
			closeTab(i);
			return;
		}
	}
}

ListenerManager *MainWindow::listenerManager()
{
	return mListenerManager;
}

void MainWindow::showGrid()
{
	EditorView *tmpView = getCurrentTab();
	if (tmpView != NULL)
		tmpView->changeSceneGrid();
}

void MainWindow::switchGrid(bool isChecked)
{
	EditorView *tmpView = getCurrentTab();
	if (tmpView != NULL) {
		QList<QGraphicsItem *> list = tmpView->scene()->items();
		foreach (QGraphicsItem *item, list) {
			NodeElement* nodeItem = dynamic_cast<NodeElement*>(item);
			if (nodeItem != NULL) {
				if (isChecked)
					nodeItem->switchOnGrid();
				else
					nodeItem->switchOffGrid();
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
	connect(this->mGesturesWidget, SIGNAL(currentElementChanged()), this, SIGNAL(currentIdealGestureChanged()));
	emit gesturesShowed();
}

IGesturesPainter * MainWindow::gesturesPainter()
{
	return this->mGesturesWidget;
}

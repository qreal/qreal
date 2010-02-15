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
#include "editorinterface.h"
#include "../model/model.h"
#include "../view/editorview.h"
#include "../umllib/uml_element.h"
#include "../generators/xmi/xmiHandler.h"
#include "../generators/java/javaHandler.h"
#include "../generators/hascol/hascolGenerator.h"
#include "../dialogs/editorGeneratorDialog.h"

using namespace qReal;

MainWindow::MainWindow()
		: mPropertyModel(mgr)
{
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

	if (!showSplash)
		ui.actionShowSplash->setChecked(false);

	ui.minimapView->setRenderHint(QPainter::Antialiasing, true);

	connect(ui.diagramExplorer, SIGNAL(clicked(const QModelIndex &)),
		this, SLOT(activateItemOrDiagram(const QModelIndex &)));

	progress->setValue(20);

	connect(ui.actionQuit, SIGNAL(triggered()), this, SLOT(close()));

	connect(ui.actionShowSplash, SIGNAL(toggled(bool)), this, SLOT (toggleShowSplash(bool)));

	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(open()));
	connect(ui.actionSave, SIGNAL(triggered()), this, SLOT(save()));
	connect(ui.actionSave_as, SIGNAL(triggered()), this, SLOT(saveAs()));
	connect(ui.actionPrint, SIGNAL(triggered()), this, SLOT(print()));
	connect(ui.actionMakeSvg, SIGNAL(triggered()), this, SLOT(makeSvg()));

	connect(ui.actionDeleteFromDiagram, SIGNAL(triggered()), this, SLOT(deleteFromDiagram()));
	connect(ui.openNewTab, SIGNAL(triggered()), this, SLOT(openNewTab()));

	connect(ui.tabs, SIGNAL(currentChanged(int)), this, SLOT(changeMiniMapSource(int)));
	connect(ui.tabs, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));

	connect(ui.actionExport_to_XMI, SIGNAL(triggered()), this, SLOT(exportToXmi()));
	connect(ui.actionGenerate_to_Java, SIGNAL(triggered()), this, SLOT(generateToJava()));
	connect(ui.actionGenerate_editor, SIGNAL(triggered()), this, SLOT(generateEditor()));
	connect(ui.actionGenerate_to_Hascol, SIGNAL(triggered()), this, SLOT(generateToHascol()));

	connect(ui.actionPlugins, SIGNAL(triggered()), this, SLOT(settingsPlugins()));

	connect(ui.actionHelp, SIGNAL(triggered()), this, SLOT(showHelp()));
	connect(ui.actionAbout, SIGNAL(triggered()), this, SLOT(showAbout()));
	connect(ui.actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

	connect(ui.minimapZoomSlider, SIGNAL(valueChanged(int)), this, SLOT(adjustMinimapZoom(int)));
	adjustMinimapZoom(ui.minimapZoomSlider->value());

	progress->setValue(40);

	// XXX: kludge... don't know how to do it in designer
	ui.diagramDock->setWidget(ui.diagramExplorer);

	ui.paletteDock->setWidget(ui.paletteToolbox);

	//ui.propertyEditor->horizontalHeader()->setStretchLastSection(true);
	//ui.propertyEditor->horizontalHeader()->hide();
	//	ui.propertyEditor->setModel(&propertyModel);

	ui.propertyEditor->setModel(&mPropertyModel);
	ui.propertyEditor->verticalHeader()->hide();
	ui.propertyEditor->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
	ui.propertyEditor->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
	ui.propertyEditor->setItemDelegate(&mDelegate);

	connect(ui.diagramExplorer, SIGNAL(clicked(QModelIndex const &)),
		&mPropertyModel, SLOT(setIndex(QModelIndex const &)));

	ui.diagramExplorer->addAction(ui.actionDeleteFromDiagram);
	ui.diagramExplorer->addAction(ui.openNewTab);

	settings.beginGroup("MainWindow");
	resize(settings.value("size", QSize(1024, 800)).toSize());
	move(settings.value("pos", QPoint(0, 0)).toPoint());
	settings.endGroup();
	progress->setValue(60);

	loadPlugins();
	showMaximized();

	progress->setValue(70);

	QString workingDir = settings.value("workingDir", "./save").toString();

	mModel = new model::Model(mgr, workingDir);
	IdList missingPlugins = mgr.checkNeededPlugins(mModel->api());
	if (!missingPlugins.isEmpty()) {
		QString text = "These plugins are not present, but needed to load the save:\n";
		foreach (Id const id, missingPlugins) {
			text += id.editor() + "\n";
		}
		QMessageBox::warning(this, tr("Some plugins are missing"), text);
		close();  // Всё, собственно.
		return;
	}

	connect(ui.actionClear, SIGNAL(triggered()), this, SLOT(exterminate()));

	progress->setValue(80);

	mPropertyModel.setSourceModel(mModel);
	ui.diagramExplorer->setModel(mModel);
	ui.diagramExplorer->setRootIndex(mModel->rootIndex());

	openNewTab();

	progress->setValue(100);
	if (showSplash)
		splash->close();
	delete splash;
}

void MainWindow::keyPressEvent(QKeyEvent *keyEvent)
{
	if (keyEvent->modifiers() == Qt::AltModifier && keyEvent->key() == Qt::Key_X){
		close();
	} else if ( keyEvent->key() == Qt::Key_F2 ){
		save();
	}
}

void MainWindow::openNewTab()
{
	QModelIndex index = QModelIndex();
	if (ui.diagramExplorer->hasFocus()) {
		index = ui.diagramExplorer->currentIndex();
	}
	int tabNumber = -1;
	for (int i = 0; i < ui.tabs->count(); i++) {
		EditorView *tab = (static_cast<EditorView *>(ui.tabs->widget(i)));
		if ((tab->mvIface()->rootIndex() == index) && (ui.tabs->currentIndex() != i)) {
			tabNumber = i;
			break;
		}
	}
	if (tabNumber != -1) {
		ui.tabs->setCurrentIndex(tabNumber);
	} else {
		EditorView *view = new EditorView();
		ui.tabs->addTab(view,"test");
		ui.tabs->setCurrentWidget(view);

		if (!index.isValid())
			index = mModel->rootIndex();
		initCurrentTab(index);
	}
}

void MainWindow::initCurrentTab(const QModelIndex &rootIndex)
{
	getCurrentTab()->setMainWindow(this);

	changeMiniMapSource(ui.tabs->currentIndex());

	connect(getCurrentTab()->scene(), SIGNAL(selectionChanged()), SLOT(sceneSelectionChanged()));
	connect(ui.actionZoom_In, SIGNAL(triggered()), getCurrentTab(), SLOT(zoomIn()));
	connect(ui.actionZoom_Out, SIGNAL(triggered()), getCurrentTab(), SLOT(zoomOut()));
	connect(ui.actionAntialiasing, SIGNAL(toggled(bool)), getCurrentTab(), SLOT(toggleAntialiasing(bool)));
	connect(ui.actionOpenGL_Renderer, SIGNAL(toggled(bool)), getCurrentTab(), SLOT(toggleOpenGL(bool)));

	getCurrentTab()->mvIface()->setModel(mModel);
	getCurrentTab()->mvIface()->setRootIndex(rootIndex);

	connect(mModel, SIGNAL(rowsAboutToBeMoved(QModelIndex, int, int, QModelIndex, int))
		, getCurrentTab()->mvIface(), SLOT(rowsAboutToBeMoved(QModelIndex, int, int, QModelIndex, int)));
	connect(mModel, SIGNAL(rowsMoved(QModelIndex, int, int, QModelIndex, int))
		, getCurrentTab()->mvIface(), SLOT(rowsMoved(QModelIndex, int, int, QModelIndex, int)));
}

MainWindow::~MainWindow()
{
	delete mModel;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	QSettings settings("SPbSU", "QReal");
	settings.beginGroup("MainWindow");
	settings.setValue("size", size());
	settings.setValue("pos", pos());
	settings.endGroup();
	event->accept();
}

void MainWindow::loadPlugins()
{
	foreach (Id const editor, mgr.editors()) {
		foreach (Id const diagram, mgr.diagrams(editor)) {
			ui.paletteToolbox->addDiagramType(diagram, mgr.friendlyName(diagram) );

			foreach (Id const element, mgr.elements(diagram)) {
				ui.paletteToolbox->addItemType(element, mgr.friendlyName(element), mgr.icon(element));
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

void MainWindow::activateSubdiagram(QModelIndex const &idx) {
	// Правило "провязки" - если есть диаграмма первого уровня с тем же именем,
	// что и у нашего элемента, показываем её.
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
	QList<QGraphicsItem*> graphicsItems = getCurrentTab()->scene()->selectedItems();
	if (graphicsItems.size() == 1) {
		QGraphicsItem *item = graphicsItems[0];
		if (UML::Element *elem = dynamic_cast<UML::Element *>(item)) {
			if (elem->index().isValid()) {
				ui.diagramExplorer->setCurrentIndex(elem->index());
				mPropertyModel.setIndex(elem->index());
			}
		}
	} else {
		ui.diagramExplorer->setCurrentIndex(QModelIndex());
		mPropertyModel.setIndex(QModelIndex());
	}
}

QString MainWindow::getWorkingDir(QString const &dialogWindowTitle)
{
	QString const dirName = QFileDialog::getExistingDirectory(this, dialogWindowTitle,
		".", QFileDialog::ShowDirsOnly);

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
	PluginDialog dialog( mgr , this);
	dialog.exec();
}

void MainWindow::deleteFromExplorer()
{
	QModelIndex idx = ui.diagramExplorer->currentIndex();
	if (idx.isValid())
		mModel->removeRow(idx.row(), idx.parent());
}

void MainWindow::deleteFromScene()
{
	foreach (QGraphicsItem *item, getCurrentTab()->scene()->selectedItems())
		if (UML::Element *elem = dynamic_cast<UML::Element *>(item))
			if (elem->index().isValid())
				mModel->removeRow(elem->index().row(), elem->index().parent());
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

void MainWindow::generateToHascol()
{
	generators::HascolGenerator hascolGenerator(mModel->api());

	QString const errors = hascolGenerator.generate();

	if (!errors.isEmpty())
		QMessageBox::warning(this, tr("errors"), "Some errors occured. Errors list: \n" + errors);
	else
		QMessageBox::information(this, tr("finished"), "Export is finished");

	qDebug() << "Done.";
}

EditorView * MainWindow::getCurrentTab()
{
	return static_cast<EditorView *>(ui.tabs->currentWidget());
}

void MainWindow::changeMiniMapSource( int index )
{
	if (index != -1) {
		ui.tabs->setEnabled(true);
		ui.minimapView->setScene(getCurrentTab()->scene());
	} else {
		ui.tabs->setEnabled(false);
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
	for (int i = 1; i < tabCount; i++)
	{
		closeTab(i);
	}
	mModel->exterminate();
}

void MainWindow::generateEditor()
{
	EditorGeneratorDialog editorGeneratorDialog(mModel->api());
	editorGeneratorDialog.exec();
}

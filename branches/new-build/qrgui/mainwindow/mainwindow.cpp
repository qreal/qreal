#include <QtGui>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSvg/QSvgGenerator>
#include <QtCore/QPluginLoader>

#include "dialogs/plugindialog.h"

#include "editorinterface.h"

#include "mainwindow.h"

MainWindow::MainWindow()
{
	ui.setupUi(this);

	ui.minimapView->setScene(ui.view->scene());
	ui.minimapView->setRenderHint(QPainter::Antialiasing, true);

//	connect(ui.diagramExplorer, SIGNAL( activated( const QModelIndex & ) ),
//			ui.view->mvIface(), SLOT( setRootIndex( const QModelIndex & ) ) );
	connect(ui.actionConnect, SIGNAL( triggered() ), this, SLOT( connectRepo() ) );
	connect(ui.actionDisconnect, SIGNAL( triggered() ), this, SLOT( closeRepo() ) );
	connect(ui.actionQuit, SIGNAL( triggered() ), this, SLOT( close() ) );
	
	connect(ui.actionZoom_In, SIGNAL( triggered() ), ui.view, SLOT( zoomIn() ) );
	connect(ui.actionZoom_Out, SIGNAL( triggered() ), ui.view, SLOT( zoomOut() ) );

	connect(ui.actionAntialiasing, SIGNAL( toggled(bool) ), ui.view, SLOT( toggleAntialiasing(bool) ) );
	connect(ui.actionOpenGL_Renderer, SIGNAL( toggled(bool) ), ui.view, SLOT( toggleOpenGL(bool) ) );

	connect(ui.actionPrint, SIGNAL( triggered() ), this, SLOT( print() ) );
	connect(ui.actionMakeSvg, SIGNAL( triggered() ), this, SLOT( makeSvg() ) );

	connect(ui.actionBeginTransaction, SIGNAL( triggered() ), this, SLOT( beginTransaction() ) );
	connect(ui.actionCommitTransaction, SIGNAL( triggered() ), this, SLOT( commitTransaction() ) );
	connect(ui.actionRollbackTransaction, SIGNAL( triggered() ), this, SLOT( rollbackTransaction() ) );

	connect(ui.actionDeleteFromDiagram, SIGNAL( triggered() ), this, SLOT( deleteFromDiagram() ) );

	connect(ui.actionPlugins, SIGNAL( triggered() ), this, SLOT( settingsPlugins() ) );

	connect(ui.actionHelp, SIGNAL( triggered() ), this, SLOT( showHelp() ) );
	connect(ui.actionAbout, SIGNAL( triggered() ), this, SLOT( showAbout() ) );
	connect(ui.actionAboutQt, SIGNAL( triggered() ), qApp, SLOT( aboutQt() ) );

	connect(ui.minimapZoomSlider, SIGNAL( valueChanged(int) ), this, SLOT( adjustMinimapZoom(int) ) );
	adjustMinimapZoom(ui.minimapZoomSlider->value());

	// XXX: kludge... don't know how to do it in designer
	ui.diagramDock->setWidget(ui.diagramExplorer);
	// ui.objectDock->setWidget(ui.objectExplorer);

	ui.paletteDock->setWidget(ui.paletteToolbox);

	ui.propertyEditor->horizontalHeader()->setStretchLastSection(true);
	ui.propertyEditor->horizontalHeader()->hide();
//	ui.propertyEditor->setModel(&propertyModel);

	ui.diagramExplorer->addAction(ui.actionDeleteFromDiagram);

//	connect(ui.diagramExplorer, SIGNAL( clicked( const QModelIndex & ) ),
//			&propertyModel, SLOT( setIndex( const QModelIndex & ) ) );

	loadPlugins();
	showMaximized();

//	connectRepo();
}

MainWindow::~MainWindow()
{
}

void MainWindow::loadPlugins()
{
//    foreach (QObject *plugin, QPluginLoader::staticInstances())
//        populateMenus(plugin);

    pluginsDir = QDir(qApp->applicationDirPath());

#if defined(Q_OS_WIN)
    if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
        pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS") {
        pluginsDir.cdUp();
        pluginsDir.cdUp();
        pluginsDir.cdUp();
    }
#endif
    pluginsDir.cd("plugins");

    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();

        if (plugin) {
	    
	    EditorInterface *iEditor = qobject_cast<EditorInterface *>(plugin);
	    if (iEditor) {
    		foreach (QString q, iEditor->diagrams())
		    ui.paletteToolbox->addDiagramType(q,iEditor->getName(q));
		
		foreach (QString q, iEditor->elements())
		    ui.paletteToolbox->addItemType(q,iEditor->getName(q),iEditor->getIcon(q));
	    }

            pluginFileNames += fileName;
        } else {
	    QMessageBox::warning(this, "QReal Plugin", loader.errorString() );
	}

    }
}

void MainWindow::adjustMinimapZoom(int zoom)
{
	ui.minimapView->resetMatrix();
	ui.minimapView->scale(0.01*zoom,0.01*zoom);
}

void MainWindow::print()
{
	QPrinter printer(QPrinter::HighResolution);
	QPrintDialog dialog(&printer, this);
	if (dialog.exec() == QDialog::Accepted) {
		QPainter painter(&printer);
//		QRect allScene = pieChart->mapFromScene(pieChart->scene()->sceneRect()).boundingRect();
		ui.view->scene()->render(&painter);
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
	ui.view->scene()->render(&painter);
}

void MainWindow::settingsPlugins()
{
    PluginDialog dialog( pluginsDir.path(), pluginFileNames , this);
    dialog.exec();
}

void MainWindow::showAbout()
{
     QMessageBox::about(this, tr("About QReal"),
             tr("<img src=\":/images/icons/slavery.jpg\"><br>"
		"<center>This is <b>QReal</b><br>"
		"Just another CASE tool</center>"));
}

void MainWindow::showHelp()
{
	QMessageBox::about(this, tr("Help"),
			tr("To begin:\n"
				"1. Click 'Connect to repository'\n"
				"2. Choose 'QSQLITE' as engine and 'temp.db' as database name\n     This should create a new unREAL database\n"
				"3. Drag 'Diagram' from 'KERNEL' tab of Palette to Diagram Explorer\n     This should create a new diagram\n"
				"4. To rename something on diagram explorer, press F2.\n"
				"5. To edit diagram visually, press Enter on diagram in Diagram Explorer\n"
				"6. To add items to diagrams, drag & drop them from Palette to editor or to Diagram Explorer\n"
				"7. Get more help from author :)"));
}

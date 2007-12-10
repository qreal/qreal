#include <QtGui>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#include <QtSvg/QSvgGenerator>

#include "mainwindow.h"

#include "qsqlconnectiondialog.h"
#include "propertyeditorproxymodel.h"
#include "realrepomodel.h"
#include "editorview.h"

MainWindow::MainWindow()
	: model(0)
{
	ui.setupUi(this);

	ui.minimapView->setScene(ui.view->scene());
	ui.minimapView->setRenderHint(QPainter::Antialiasing, true);

	connect(ui.diagramExplorer, SIGNAL( activated( const QModelIndex & ) ),
			ui.view->mvIface(), SLOT( setRootIndex( const QModelIndex & ) ) );
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
	ui.propertyEditor->setModel(&propertyModel);

	ui.diagramExplorer->addAction(ui.actionDeleteFromDiagram);

	connect(ui.diagramExplorer, SIGNAL( clicked( const QModelIndex & ) ),
			&propertyModel, SLOT( setIndex( const QModelIndex & ) ) );

	show();

	showMaximized();

	connectRepo();
}

MainWindow::~MainWindow()
{
}

void MainWindow::adjustMinimapZoom(int zoom)
{
	ui.minimapView->resetMatrix();
	ui.minimapView->scale(0.01*zoom,0.01*zoom);
}

bool realInitDatabase(QSqlDatabase db)
{
	QSqlQuery q(db);
	QFile scriptFile(":/repo/scripts.sql");
	if ( !scriptFile.open(QFile::ReadOnly) )
		return false;

	if ( q.exec("SELECT * FROM metatable ;" ) )
		return true;

	QTextStream script(&scriptFile);

	QStringList queries = script.readAll().split(";",QString::SkipEmptyParts);

	foreach (QString query, queries) {
		if ( query.simplified().isEmpty() )
			continue;

		// FIXME: small fix for sqlite driver
		if ( db.driverName() == "QSQLITE" )
			query.replace("AUTO_INCREMENT", "AUTOINCREMENT");

		if ( ! q.exec(query.simplified().append(" ;")) ) {
			qDebug() << query.simplified().append(" ;");
			return false;
		}

	}

	return true;
}

void MainWindow::connectRepo()
{
	QSqlConnectionDialog dialog(this);
	QSqlError err;

	if (dialog.exec() != QDialog::Accepted)
		return;

	closeRepo();

	db = QSqlDatabase::addDatabase(dialog.driverName());
    
	db.setDatabaseName(dialog.databaseName());
	db.setHostName(dialog.hostName());
	db.setPort(dialog.port()); 
	
	if (!db.open(dialog.userName(), dialog.password())) {
		err = db.lastError();
		db = QSqlDatabase();
	}

	if (err.type() != QSqlError::NoError) {
		QMessageBox::warning(0, QObject::tr("Unable to open database"),
				QObject::tr("An error occured while opening the connection:\n")
				+ err.driverText() + "\n" + err.databaseText());
		return;
	}

	if ( ! realInitDatabase(db) ) {
		QMessageBox::warning(0, QObject::tr("Unable to initialize database"),
				QObject::tr("An error occured while setting up data:\n"));
		return;
	}

	model = new RealRepoModel(db,this);

	ui.diagramExplorer->setModel(model);
	ui.diagramExplorer->setRootIndex(model->index(1,0,QModelIndex()));

	ui.objectExplorer->setModel(model);
//	ui.objectExplorer->setRowHidden(1,QModelIndex(),true);

	propertyModel.setSourceModel(model);

	ui.view->mvIface()->setModel(model);
	
	ui.actionBeginTransaction->setEnabled(true);
	ui.actionCommitTransaction->setEnabled(false);
	ui.actionRollbackTransaction->setEnabled(false);
}

void MainWindow::closeRepo()
{
	ui.diagramExplorer->setModel(0);
	ui.objectExplorer->setModel(0);
	propertyModel.setSourceModel(0);
	ui.view->mvIface()->setModel(0);

	ui.actionBeginTransaction->setEnabled(false);
	ui.actionCommitTransaction->setEnabled(false);
	ui.actionRollbackTransaction->setEnabled(false);

	if( model )
		delete model;

	model = 0;
}

void MainWindow::beginTransaction()
{
	if ( model ) {
		model->beginTransaction();
		ui.actionBeginTransaction->setEnabled(false);
		ui.actionCommitTransaction->setEnabled(true);
		ui.actionRollbackTransaction->setEnabled(true);
	}
}

void MainWindow::commitTransaction()
{
	if ( model ) {
		model->commitTransaction();
		ui.actionBeginTransaction->setEnabled(true);
		ui.actionCommitTransaction->setEnabled(false);
		ui.actionRollbackTransaction->setEnabled(false);
	}
}

void MainWindow::rollbackTransaction()
{
	if ( model ) {
		model->rollbackTransaction();
		ui.actionBeginTransaction->setEnabled(true);
		ui.actionCommitTransaction->setEnabled(false);
		ui.actionRollbackTransaction->setEnabled(false);

		ui.diagramExplorer->setRootIndex(model->index(1,0,QModelIndex()));
	}
}

void MainWindow::deleteFromDiagram()
{
	if ( model ) {
		QModelIndex idx = ui.diagramExplorer->currentIndex();
		model->removeRow( idx.row(), idx.parent() );
	}
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

void MainWindow::showAbout()
{
     QMessageBox::about(this, tr("About unREAL"),
             tr("This is <b>unREAL</b><br>"
				 "Just another CASE tool"));
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

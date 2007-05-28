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

	view = new EditorView;
	setCentralWidget(view);

//	delete ui.centralwidget;

	ui.minimapView->setScene(view->scene());
	ui.minimapView->setRenderHint(QPainter::Antialiasing, true);

	connect(ui.diagramExplorer, SIGNAL( activated( const QModelIndex & ) ),
			view->mvIface(), SLOT( setRootIndex( const QModelIndex & ) ) );
	connect(ui.actionConnect, SIGNAL( triggered() ), this, SLOT( connectRepo() ) );
	connect(ui.actionDisconnect, SIGNAL( triggered() ), this, SLOT( closeRepo() ) );
	connect(ui.actionQuit, SIGNAL( triggered() ), this, SLOT( close() ) );
	
	connect(ui.actionZoom_In, SIGNAL( triggered() ), view, SLOT( zoomIn() ) );
	connect(ui.actionZoom_Out, SIGNAL( triggered() ), view, SLOT( zoomOut() ) );

	connect(ui.actionAntialiasing, SIGNAL( toggled(bool) ), view, SLOT( toggleAntialiasing(bool) ) );
	connect(ui.actionOpenGL_Renderer, SIGNAL( toggled(bool) ), view, SLOT( toggleOpenGL(bool) ) );

	connect(ui.actionPrint, SIGNAL( triggered() ), this, SLOT( print() ) );
	connect(ui.actionMakeSvg, SIGNAL( triggered() ), this, SLOT( makeSvg() ) );

	connect(ui.minimapZoomSlider, SIGNAL( valueChanged(int) ), this, SLOT( adjustMinimapZoom(int) ) );
	adjustMinimapZoom(ui.minimapZoomSlider->value());

	// XXX: kludge... don't know how to do it in designer
	ui.diagramDock->setWidget(ui.diagramExplorer);
	// ui.objectDock->setWidget(ui.objectExplorer);

	ui.paletteDock->setWidget(ui.paletteToolbox);

	ui.propertyEditor->horizontalHeader()->setStretchLastSection(true);
	ui.propertyEditor->horizontalHeader()->hide();
	ui.propertyEditor->setModel(&propertyModel);

	connect(ui.diagramExplorer, SIGNAL( clicked( const QModelIndex & ) ),
			&propertyModel, SLOT( setIndex( const QModelIndex & ) ) );

	show();
    resize(1024, 800);
	connectRepo();
}

MainWindow::~MainWindow()
{
	delete view;
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

	view->mvIface()->setModel(model);
}

void MainWindow::closeRepo()
{
	ui.diagramExplorer->setModel(0);
	ui.objectExplorer->setModel(0);
	propertyModel.setSourceModel(0);
	view->mvIface()->setModel(0);

	if( model )
		delete model;

	model = 0;
}

void MainWindow::print()
{
	QPrinter printer(QPrinter::HighResolution);
	QPrintDialog dialog(&printer, this);
	if (dialog.exec() == QDialog::Accepted) {
		QPainter painter(&printer);
//		QRect allScene = pieChart->mapFromScene(pieChart->scene()->sceneRect()).boundingRect();
		view->scene()->render(&painter);
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
	view->scene()->render(&painter);
}


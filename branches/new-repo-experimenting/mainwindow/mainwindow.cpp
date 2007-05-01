#include <QtGui>
#include <QtSql>

#include "mainwindow.h"

#include "qsqlconnectiondialog.h"
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

	connect(ui.diagramExplorer, SIGNAL( activated( const QModelIndex & ) ),
			view->mvIface(), SLOT( setRootIndex( const QModelIndex & ) ) );

	connect(ui.actionConnect, SIGNAL( triggered() ), this, SLOT( connectRepo() ) );
	connect(ui.actionQuit, SIGNAL( triggered() ), this, SLOT( close() ) );
	
	connect(ui.actionZoom_In, SIGNAL( triggered() ), view, SLOT( zoomIn() ) );
	connect(ui.actionZoom_Out, SIGNAL( triggered() ), view, SLOT( zoomOut() ) );

	connect(ui.actionAntialiasing, SIGNAL( toggled(bool) ), view, SLOT( toggleAntialiasing(bool) ) );
	connect(ui.actionOpenGL_Renderer, SIGNAL( toggled(bool) ), view, SLOT( toggleOpenGL(bool) ) );

	connect(ui.minimapZoomSlider, SIGNAL( valueChanged(int) ), this, SLOT( adjustMinimapZoom(int) ) );
	adjustMinimapZoom(ui.minimapZoomSlider->value());

	// XXX: kludge... don't know how to do it in designer
	ui.diagramDock->setWidget(ui.diagramExplorer);
	ui.objectDock->setWidget(ui.objectExplorer);

	show();
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

void MainWindow::connectRepo()
{
	QSqlConnectionDialog dialog(this);
	QSqlError err;

	if (dialog.exec() != QDialog::Accepted)
		return;

	ui.diagramExplorer->setModel(0);
	ui.objectExplorer->setModel(0);
	if( model )
		delete model;

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
/*
	if ( dialog.driverName() == "QSQLITE" ) {
		this->createDatabase();
	}
*/
	model = new RealRepoModel();
	
	ui.diagramExplorer->setModel(model);
	ui.diagramExplorer->setRootIndex(model->index(0,0,QModelIndex()));

	ui.objectExplorer->setModel(model);
	ui.objectExplorer->setRowHidden(0,QModelIndex(),true);

	view->mvIface()->setModel(model);

	connect(ui.objectExplorer, SIGNAL( activated( const QModelIndex & ) ),
			model, SLOT( createSomeChild( const QModelIndex & )));
}

/** @file mainwindow.cpp
 * 	@brief Главное окно приложения 
 * */
#include <QtGui>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSvg/QSvgGenerator>
#include <QMessageBox>
#include <QTcpSocket>

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
	connect(ui.diagramExplorer, SIGNAL( clicked( const QModelIndex & ) ),
			this, SLOT( activateItemOrDiagram( const QModelIndex & ) ) );
	connect(ui.actionConnect, SIGNAL( triggered() ), this, SLOT( connectRepo() ) );
	connect(ui.actionDisconnect, SIGNAL( triggered() ), this, SLOT( closeRepo() ) );
	connect(ui.actionQuit, SIGNAL( triggered() ), this, SLOT( close() ) );
	
	connect(ui.actionZoom_In, SIGNAL( triggered() ), ui.view, SLOT( zoomIn() ) );
	connect(ui.actionZoom_Out, SIGNAL( triggered() ), ui.view, SLOT( zoomOut() ) );

	connect(ui.actionAntialiasing, SIGNAL( toggled(bool) ), ui.view, SLOT( toggleAntialiasing(bool) ) );
	connect(ui.actionOpenGL_Renderer, SIGNAL( toggled(bool) ), ui.view, SLOT( toggleOpenGL(bool) ) );

	connect(ui.actionPrint, SIGNAL( triggered() ), this, SLOT( print() ) );
	connect(ui.actionMakeSvg, SIGNAL( triggered() ), this, SLOT( makeSvg() ) );

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
	ui.propertyEditor->setItemDelegate(&delegate);

	ui.diagramExplorer->addAction(ui.actionDeleteFromDiagram);

	connect(ui.diagramExplorer, SIGNAL( clicked( const QModelIndex & ) ),
			&propertyModel, SLOT( setIndex( const QModelIndex & ) ) );

	show();

	//showMaximized();
	resize(1024,800);

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

void MainWindow::connectRepo()
{
	closeRepo();
	
	model = new RealRepoModel(this);
	if( model->getState() != QAbstractSocket::ConnectedState ){	
		qDebug() << "repo model creation failed";
		QMessageBox::critical(0, "achtung!", "cannot reach repo server at 127.0.0.1:6666.\n"
					"make sure that it is running and restart qreal");
		//qApp->exit();
		closeRepo();
		return;
	}
	ui.diagramExplorer->setModel(model);
	ui.diagramExplorer->setRootIndex(model->index(1,0,QModelIndex()));

	ui.objectExplorer->setModel(model);
//	ui.objectExplorer->setRowHidden(1,QModelIndex(),true);

	propertyModel.setSourceModel(model);

	ui.view->mvIface()->setModel(model);
	
}

void MainWindow::closeRepo()
{
	ui.diagramExplorer->setModel(0);
	ui.objectExplorer->setModel(0);
	propertyModel.setSourceModel(0);
	ui.view->mvIface()->setModel(0);

	if( model )
		delete model;

	model = 0;
}

void MainWindow::deleteFromExplorer()
{
	QModelIndex idx = ui.diagramExplorer->currentIndex();
	if (idx.isValid())
		model->removeRow(idx.row(), idx.parent());
}

void MainWindow::deleteFromScene()
{
	foreach (QGraphicsItem *item, ui.view->scene()->selectedItems()) {
		if (UML::Element * elem = dynamic_cast < UML::Element * >(item))
			if (elem->index().isValid())
				model->removeRow(elem->index().row(), elem->index().parent());
	}
}

void MainWindow::deleteFromDiagram()
{
	if ( model ) {
		if (ui.diagramExplorer->hasFocus())
		{
			deleteFromExplorer();
			/* Brutal, but right now we have no choice */
			connectRepo();
		}
		else if (ui.view->hasFocus())
		{
			deleteFromScene();
		}
	}
}

void MainWindow::activateItemOrDiagram(const QModelIndex &idx)
{
	QModelIndex parent = idx.parent();

	/* Is level-one diagram? */
	if (parent == model->index(1,0,QModelIndex()))
	{
		/* activate this diagram */
		ui.view->mvIface()->setRootIndex(idx);
	}
	else
	{
		/* activate parent diagram */
		ui.view->mvIface()->setRootIndex(parent);
		/* select this item on diagram */
		ui.view->scene()->clearSelection();
		(dynamic_cast<EditorViewScene *>(ui.view->scene()))->getElemByModelIndex(idx)->setSelected(true);
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
     QMessageBox::about(this, tr("About QReal"),
             tr("This is <b>QReal</b><br>"
				 "Just another CASE tool"));
}

void MainWindow::showHelp()
{
}

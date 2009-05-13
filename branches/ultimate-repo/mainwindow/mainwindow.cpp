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

#include "propertyeditorproxymodel.h"
#include "realrepomodel.h"
#include "editorview.h"
#include "optionsDialog.h"
#include "meta_generator.h"
#include <QProgressBar>

using namespace qReal;

MainWindow::MainWindow() : model(0)
{
	QSettings settings("Tercom", "QReal");
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

	if (!showSplash)
		ui.actionShowSplash->setChecked(false);

	ui.minimapView->setScene(ui.view->scene());
	ui.minimapView->setRenderHint(QPainter::Antialiasing, true);
	progress->setValue(10);

	connect(ui.diagramExplorer, SIGNAL( activated( const QModelIndex & ) ),
			ui.view->mvIface(), SLOT( setRootIndex( const QModelIndex & ) ) );
	connect(ui.view->scene(), SIGNAL(selectionChanged()), SLOT(sceneSelectionChanged()));
	connect(ui.diagramExplorer, SIGNAL( clicked( const QModelIndex & ) ),
			this, SLOT( activateItemOrDiagram( const QModelIndex & ) ) );
	connect(ui.actionConnect, SIGNAL( triggered() ), this, SLOT( connectRepo() ) );
	connect(ui.actionDisconnect, SIGNAL( triggered() ), this, SLOT( closeRepo() ) );
	connect(ui.actionQuit, SIGNAL( triggered() ), this, SLOT( close() ) );

	connect(ui.actionZoom_In, SIGNAL( triggered() ), ui.view, SLOT( zoomIn() ) );
	connect(ui.actionZoom_Out, SIGNAL( triggered() ), ui.view, SLOT( zoomOut() ) );

	connect(ui.actionAntialiasing, SIGNAL( toggled(bool) ), ui.view, SLOT( toggleAntialiasing(bool) ) );
	connect(ui.actionOpenGL_Renderer, SIGNAL( toggled(bool) ), ui.view, SLOT( toggleOpenGL(bool) ) );
	connect(ui.actionShowSplash, SIGNAL( toggled(bool) ), this, SLOT (toggleShowSplash(bool) ) );

	connect(ui.actionPrint, SIGNAL( triggered() ), this, SLOT( print() ) );
	connect(ui.actionMakeSvg, SIGNAL( triggered() ), this, SLOT( makeSvg() ) );
	progress->setValue(20);
	connect(ui.actionDeleteFromDiagram, SIGNAL( triggered() ), this, SLOT( deleteFromDiagram() ) );
	connect(ui.actionJumpToAvatar, SIGNAL( triggered() ), this, SLOT( jumpToAvatar() ) );

	connect(ui.actionOptions, SIGNAL( triggered() ), this, SLOT( showOptions() ) );
	connect(ui.actionRun_test_queries, SIGNAL( triggered() ), this, SLOT( runTestQueries() ));
	connect(ui.actionReconnect, SIGNAL( triggered() ), this, SLOT( reconnect() ));
	connect(ui.actionGenerate_editor, SIGNAL( triggered() ), this, SLOT( generator_editor() ));	
//
	connect(ui.actionHelp, SIGNAL( triggered() ), this, SLOT( showHelp() ) );
	connect(ui.actionAbout, SIGNAL( triggered() ), this, SLOT( showAbout() ) );
	connect(ui.actionAboutQt, SIGNAL( triggered() ), qApp, SLOT( aboutQt() ) );

	connect(ui.minimapZoomSlider, SIGNAL( valueChanged(int) ), this, SLOT( adjustMinimapZoom(int) ) );
	adjustMinimapZoom(ui.minimapZoomSlider->value());

	// XXX: kludge... don't know how to do it in designer
	ui.diagramDock->setWidget(ui.diagramExplorer);
	// ui.objectDock->setWidget(ui.objectExplorer);

	ui.paletteDock->setWidget(ui.paletteToolbox);

	ui.propertyEditor->setModel(&propertyModel);
	ui.propertyEditor->verticalHeader()->hide();
	ui.propertyEditor->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
	ui.propertyEditor->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
	ui.propertyEditor->setItemDelegate(&delegate);

	ui.diagramExplorer->addAction(ui.actionDeleteFromDiagram);

	connect(ui.diagramExplorer, SIGNAL( clicked( const QModelIndex & ) ),
			&propertyModel, SLOT( setIndex( const QModelIndex & ) ) );

	settings.beginGroup("MainWindow");
	resize(settings.value("size", QSize(1024, 800)).toSize());
	move(settings.value("pos", QPoint(0, 0)).toPoint());
	settings.endGroup();
	progress->setValue(60);
	QApplication::processEvents();

	repoAddress = settings.value("repoAddress", "127.0.0.1").toString();
	repoPort = settings.value("repoPort", 6666).toInt();

	connDialog = new ConnectionDialog(repoAddress, repoPort);
	connect(connDialog, SIGNAL(dataAccepted(const QString&, const int)), this, SLOT(reconnectRepo(const QString&, const int)));
	progress->setValue(70);
	if (showSplash)
	{
		connectRepo(splash, repoAddress, repoPort);
		progress->setValue(100);
		splash->close();
	}
	else
		connectRepo(NULL, repoAddress, repoPort);

	// progressbar will be deleted automatically
	delete splash;

	show();
}

MainWindow::~MainWindow()
{
	delete connDialog;
	closeRepo();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	QSettings settings("Tercom", "QReal");
	settings.beginGroup("MainWindow");
	settings.setValue("size", size());
	settings.setValue("pos", pos());
	settings.endGroup();
	settings.setValue("repoAddress", repoAddress);
	settings.setValue("repoPort", repoPort);
	event->accept();
}

void MainWindow::adjustMinimapZoom(int zoom)
{
	ui.minimapView->resetMatrix();
	ui.minimapView->scale(0.01*zoom,0.01*zoom);
}

void MainWindow::connectRepo(QSplashScreen *splash, const QString &addr, const int port)
{
	closeRepo();
	qDebug() << "connecting..." << addr << port;
	model = new RealRepoModel(addr, port, this);
	if( model->getState() != QAbstractSocket::ConnectedState ){
		qDebug() << "repo model creation failed";
		if (splash != NULL)
			splash->close();
		QMessageBox::critical(0, tr("achtung!"), tr("cannot reach repo server at ")
			+ repoAddress + QString(":") + QString::number(repoPort)
			+ QString("\n")
			+ tr("make sure that it is running or use \"Tools/Setup repo server path\" ")
			+ tr("to select another server"));
		//qApp->exit();
		closeRepo();
		return;
	}
	ui.diagramExplorer->setModel(model);
	ui.diagramExplorer->setRootIndex(model->getDiagramCategoryIndex());

	ui.objectExplorer->setModel(model);
//	ui.objectExplorer->setRowHidden(1,QModelIndex(),true);

	propertyModel.setSourceModel(model);

	ui.view->mvIface()->setModel(model);

	QModelIndex rootDiagram = model->createDefaultTopLevelItem();
	activateItemOrDiagram(rootDiagram);
	propertyModel.setIndex(rootDiagram);

	connect(ui.actionUndo, SIGNAL( triggered() ), model, SLOT( undo() ) );
	connect(ui.actionRedo, SIGNAL( triggered() ), model, SLOT( redo() ) );

	connect(ui.actionClear, SIGNAL( triggered() ), this, SLOT( exterminatus() ));

	connect(ui.actionCommand_list, SIGNAL( triggered() ), model,
			SLOT( showCommandList() ));

	connect(model, SIGNAL( canUndoChanged(bool) ), ui.actionUndo, SLOT( setEnabled(bool) ));
	connect(model, SIGNAL( canRedoChanged(bool) ), ui.actionRedo, SLOT( setEnabled(bool) ));
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
	{
		try
		{
			model->removeRow(idx.row(), idx.parent());
		}
		catch (bool b)
		{
			QMessageBox::warning(this, tr("Operation aborted"),
				tr("Cannot delete avatar"));
		}
		catch (QString e)
		{
			QMessageBox::warning(this, tr("Operation aborted"),
				tr("Repository can not delete this element."));
		}
	}
}

void MainWindow::deleteFromScene()
{
	foreach (QGraphicsItem *item, ui.view->scene()->selectedItems()) {
		if (UML::Element * elem = dynamic_cast < UML::Element * >(item))
			if (elem->index().isValid())
			{
				try{
					model->removeRow(elem->index().row(), elem->index().parent());
				}
				catch (qRealTypes::IdTypeList l)
				{
					QString l2 = "";
					int i;

					l2 = l[0];
					for (i = 1; i < l.size(); i++)
						l2 += ", " + l[i];
					QMessageBox::warning(this, tr("Operation aborted"),
						QString("element is referred by ") + l2);
				}
				catch (QString e)
				{
					QMessageBox::warning(this, tr("Operation aborted"),
						tr("Repository can not delete this element."));
				}
			}
	}
}

void MainWindow::deleteFromDiagram()
{
	if ( model ) {
		if (ui.diagramExplorer->hasFocus())
		{
			deleteFromExplorer();
		}
		else if (ui.view->hasFocus())
		{
			deleteFromScene();
		}
	}
}

void MainWindow::jumpToAvatarFromScene()
{
	QList<QGraphicsItem *> list = ui.view->scene()->selectedItems();

	if (list.count() != 1)
		return;

	QGraphicsItem *elem = list.first();
	UML::Element *e = dynamic_cast < UML::Element * >(elem);
	if (e->index().isValid())
	{
		QModelIndex idx = model->getAvatarIndex(e->index());
		if (idx.isValid())
		{
			qDebug() << "jumping to avatar" << idx;
			activateItemOrDiagram(idx);
		}
	}
}

void MainWindow::jumpToAvatar()
{
	if ( model ) {
		if (ui.diagramExplorer->hasFocus())
		{
			// Maybe, not needed
//			jumpToAvatarFromExplorer();
		}
		else if (ui.view->hasFocus())
		{
			jumpToAvatarFromScene();
		}
	}
}

void MainWindow::activateItemOrDiagram(const QModelIndex &idx)
{
	QModelIndex parent = idx.parent();

	/* Is level-one diagram? */
	if (parent == model->getDiagramCategoryIndex())
	{
		/* activate this diagram */
		ui.view->mvIface()->setRootIndex(idx);
		ui.diagramExplorer->setCurrentIndex(idx);
	}
	else
	{
		if (ui.view->mvIface()->rootIndex() != parent)
		{
			/* activate parent diagram */
			ui.view->mvIface()->setRootIndex(parent);
		}
		/* select this item on diagram */
		ui.view->scene()->clearSelection();
		UML::Element *e = (dynamic_cast<EditorViewScene *>(ui.view->scene()))->getElemByModelIndex(idx);
		if (e)
			e->setSelected(true);
		else
			qDebug() << "shit happened!!!\n";
	}
}

void MainWindow::sceneSelectionChanged()
{
	QList<QGraphicsItem*> graphicsItems =  ui.view->scene()->selectedItems();
	if (graphicsItems.size() == 1) {
		QGraphicsItem *item = graphicsItems[0];
		if (UML::Element *elem = dynamic_cast<UML::Element *>(item))
			if (elem->index().isValid()) {
				ui.diagramExplorer->setCurrentIndex(elem->index());
				propertyModel.setIndex(elem->index());
			}
	}
	else
	{
		ui.diagramExplorer->setCurrentIndex(QModelIndex());
		propertyModel.setIndex(QModelIndex());
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
		"Just another CASE tool<br>"
		"<i>Burning heretics since 1724</i>"));
}

void MainWindow::showHelp()
{
	QMessageBox::warning(this, tr("Not implemented"),
			 tr("This feature is not implemented yet. "));
}

void MainWindow::showOptions()
{
	QReal::OptionsDialog dialog(ui.paletteToolbox->getSelectedTabs());
	dialog.exec();
	if (dialog.result() == QDialog::Accepted)
	{
		QVector<bool> selectedEditors = dialog.getSelectedEditors();
		ui.paletteToolbox->setEditors(selectedEditors);
	}
}

void MainWindow::runTestQueries() const
{
	RealRepoClient client(repoAddress, repoPort, 0);

	// Запросы к репозиторию писать здесь.

	qDebug() << "Done.";
}

void MainWindow::reconnect()
{
	qDebug() << "OLOLO!!!";
	connDialog->exec();
}

void MainWindow::generator_editor() {
	qDebug() << "generator_editor run";
	qReal::meta_generator editorGeneratorDialog;
	editorGeneratorDialog.exec();
}



void MainWindow::reconnectRepo(const QString& addr, const int port)
{
	repoAddress = addr;
	repoPort = port;
	connectRepo(0, addr, port);
}

void MainWindow::toggleShowSplash(bool show)
{
	QSettings settings("Tercom", "QReal");
	settings.setValue("ShowSplashScreen", show);
}

void MainWindow::exterminatus(void)
{
	qDebug() << "Activating the Red Rune!!!";
	model->getRepoClient()->clearRepository();
	closeRepo();
}

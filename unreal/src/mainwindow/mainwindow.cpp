#include <QtGui>

#include "mainwindow.h"

MainWindow::MainWindow()
{
dbg;
  QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
  db.setHostName("localhost");
  db.setDatabaseName("test");
  db.setUserName("real");
  db.setPassword("domination");
  
  //db.setDatabaseName(":memory:");
  if (!db.open()) {
    QMessageBox::critical(0, qApp->tr("Cannot open database"),
      qApp->tr("Unable to establish a database connection.\n"), 
               QMessageBox::Cancel);
    return;
  }

  QSqlQuery query;    

  query.exec("create table diagram (id int primary key, name varchar(20), type varchar(20))");
  query.exec("insert into diagram values (1, 'actor', 'objects')");   
  query.exec("insert into diagram values (2, 'usecase', 'objects')");
  query.exec("insert into diagram values (3, 'super_cool_diagram', 'diagrams')");   
  query.exec("insert into diagram values (4, 'awesome_diagram', 'diagrams')");   

  query.exec("create table actor (id int primary key, name varchar(20), diagram varchar(20), svg varchar(20))");
  query.exec("insert into actor values (1, 'actor 1', 'awesome_diagram', 'path 1')");
  query.exec("insert into actor values (2, 'actor 2', 'super_cool_diagram', 'path 2')");
  query.exec("insert into actor values (3, 'actor 3', 'super_cool_diagram', 'path 1')");
 
  query.exec("create table usecase (id int primary key, name varchar(20), diagram varchar(20), svg varchar(30))");
  query.exec("insert into usecase values (1, 'usecase 1', 'awesome_diagram', 'path 3')");
 
  query.exec("create table super_cool_diagram (id int primary key, name varchar(20), type varchar(20))");
  query.exec("insert into super_cool_diagram values (1, 'actor 2', 'actor')");
  query.exec("insert into super_cool_diagram values (2, 'actor 4', 'actor')");
 
  query.exec("create table awesome_diagram (id int primary key, name varchar(20), type varchar(20))");
  query.exec("insert into awesome_diagram values (1, 'actor 1', 'actor')");
  query.exec("insert into awesome_diagram values (2, 'usecase 1', 'usecase')");
   
  //basemodel = new BaseModel(db);
    
    model1 = new ObjectExplorerModel(db);

    QDockWidget *dock = new QDockWidget(tr("object explorer"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    tree1 = new QTreeView();
    tree1->setModel(model1);
    tree1->setHeader(0);

    dock->setWidget(tree1);
    addDockWidget(Qt::LeftDockWidgetArea, dock);

    model2 = new DiagramExplorerModel(db);

    QDockWidget *dock3 = new QDockWidget(tr("diagram explorer"), this);
    dock3->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    tree2 = new QTreeView();
    tree2->setModel(model2);
    tree2->setHeader(0);

    dock3->setWidget(tree2);
    addDockWidget(Qt::LeftDockWidgetArea, dock3);

//    connect(model2, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)), model1, SLOT(reset2(const QModelIndex &, const QModelIndex &)));

    connect(model2, SIGNAL(dataAboutToBeChanged(const QModelIndex &, QVariant)), model1, SLOT(updateData(const QModelIndex &, QVariant)));
    connect(model1, SIGNAL(dataAboutToBeChanged(const QModelIndex &, QVariant)), model2, SLOT(updateData(const QModelIndex &, QVariant)));

    /*   freezed for some time
    
    q = new QSqlQueryModel();
    //q->setQuery("select * from actor where name='actor 1'");
    proxy = new Proxy();
    proxy->setSourceModel(q);
    
    
    QDockWidget  *dock2 = new QDockWidget(tr("property editor"), this);
    table = new QTableView();
    table->setModel(proxy);
    dock2->setWidget(table);
    addDockWidget(Qt::LeftDockWidgetArea, dock2);
    
    connect(tree, SIGNAL(clicked(const QModelIndex&)), q, SLOT(reset(const QModelIndex&)));
    */
    pieChart = new PieView();
    //pieChart->setModel(model2);

    //QItemSelectionModel *selectionModel = new QItemSelectionModel(model);
    //table->setSelectionModel(selectionModel);
    //pieChart->setSelectionModel(selectionModel);
 
    setCentralWidget(pieChart);

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    createDockWindows();

    readFile();

    setWindowTitle(tr("REAL"));

//    newLetter();

   
}

void MainWindow::newLetter()
{
}

void MainWindow::print()
{
}

void MainWindow::save()
{
}

void MainWindow::undo()
{
}

void MainWindow::insertCustomer(const QString &)
{
}

void MainWindow::addParagraph(const QString &)
{
}

void MainWindow::readFile()
{
}

void MainWindow::about()
{
QMessageBox::about(this, tr("about unREAL"),
            tr("this is the baseline of <b>unREAL</b> -- troo Qt-based version of <b>REAL</b>\n"
                      "<center>(approved and blessed by the Elder Gods themselves!!)</center>"));
}

void MainWindow::createActions()
{
    newLetterAct = new QAction(QIcon(":/images/new.png"), tr("&New Letter"),
                               this);
    newLetterAct->setShortcut(tr("Ctrl+N"));
    newLetterAct->setStatusTip(tr("Create a new form letter"));
    connect(newLetterAct, SIGNAL(triggered()), this, SLOT(newLetter()));

    saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save..."), this);
    saveAct->setShortcut(tr("Ctrl+S"));
    saveAct->setStatusTip(tr("Save the current form letter"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    printAct = new QAction(QIcon(":/images/print.png"), tr("&Print..."), this);
    printAct->setShortcut(tr("Ctrl+P"));
    printAct->setStatusTip(tr("Print the current form letter"));
    connect(printAct, SIGNAL(triggered()), this, SLOT(print()));

    undoAct = new QAction(QIcon(":/images/undo.png"), tr("&Undo"), this);
    undoAct->setShortcut(tr("Ctrl+Z"));
    undoAct->setStatusTip(tr("Undo the last editing action"));
    connect(undoAct, SIGNAL(triggered()), this, SLOT(undo()));

    quitAct = new QAction(tr("&Quit"), this);
    quitAct->setShortcut(tr("Ctrl+Q"));
    quitAct->setStatusTip(tr("Quit the application"));
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    addActorAct = new QAction(tr("Add Actor"), this);
    connect(addActorAct, SIGNAL(triggered()), this, SLOT(addActor()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newLetterAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAct);

    viewMenu = menuBar()->addMenu(tr("&View"));

    menuBar()->addSeparator();

    addMenu = menuBar()->addMenu(tr("&Add"));
    addMenu->addAction(addActorAct);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newLetterAct);
    fileToolBar->addAction(saveAct);
    fileToolBar->addAction(printAct);

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(undoAct);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::createDockWindows()
{
}

void MainWindow::addActor(){
  
  QSqlQuery q;
  qDebug() << "==actors:";
  q.exec("select name from actor");
  while(q.next())
    qDebug() << "  " << q.value(0).toString();

  qDebug() << "==awesome_diagram:";
  q.exec("select name from awesome_diagram");
  while(q.next())
    qDebug() << "  " << q.value(0).toString();
  
}


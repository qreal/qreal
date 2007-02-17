//=====================================================================
// Project:      unREAL
// File Name:    mainwindow.cpp
// Description:  Application's main window class
//
// Created:      hz :)
// Revision:     03-Feb-07
//
// Author:       
//===================================================================== 

#include <QtGui>
#include "mainwindow.h"
#include "dbg.h"
#include <QDebug>
#include <stdio.h>

MainWindow::MainWindow()
{
dbg;
    sqlite = false;

    if (sqlite){
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(":memory:");
    }
    else {
        db = QSqlDatabase::addDatabase("QMYSQL");
        db.setDatabaseName("unreal");
        db.setUserName("unreal");
        db.setPassword("domination");
        db.setHostName("127.0.0.1");
    }
    if (db.open())
        dbOpened = true;
    else
        dbOpened = false;

    curNum = 0;
    elemID = 0;
    curDiagram = "";
    
    if (dbOpened){
        createEditors();
    }    
    
     
    model1 = new ObjectExplorerModel(db);

    QDockWidget *dock = new QDockWidget(tr("object explorer"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    tree1 = new QTreeView();
    tree1->setModel(model1);
    tree1->setHeader(0);

    dock->setWidget(tree1);
    addDockWidget(Qt::LeftDockWidgetArea, dock);

    model2 = new DiagramExplorerModel(db);
    TreeItem *item = static_cast<TreeItem*>(model2->index(0,0,QModelIndex()).internalPointer());
    
    QDockWidget *dock3 = new QDockWidget(tr("diagram explorer"), this);
    dock3->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    tree2 = new QTreeView();
    tree2->setModel(model2);
    tree2->setHeader(0);
 
    dock3->setWidget(tree2);
    addDockWidget(Qt::LeftDockWidgetArea, dock3);


    connect(model2, SIGNAL(dataAboutToBeChanged(const QModelIndex &, QVariant)), model1, SLOT(updateData(const QModelIndex &, QVariant)));
    connect(model1, SIGNAL(dataAboutToBeChanged(const QModelIndex &, QVariant)), model2, SLOT(updateData(const QModelIndex &, QVariant)));
    connect(model2, SIGNAL(elemAdded()), model1, SLOT(doNOTuseIt()));
    
    propModel = new PropertyEditorModel();
    table = new QTableView();
    table->setModel(propModel);
    connect(tree2, SIGNAL(clicked( const QModelIndex&) ), propModel, SLOT( setFocus(const QModelIndex& )));
    connect(tree1, SIGNAL(clicked( const QModelIndex&) ), propModel, SLOT( setFocus(const QModelIndex& )));
    QDockWidget *dock4 = new QDockWidget(tr("property editor"), this);
    dock4->setWidget(table);
    addDockWidget(Qt::LeftDockWidgetArea, dock4);
    
    pieChart = new PieView();
    setCentralWidget(pieChart);
    pieChart->setRootIndex(model2->index(0, 0, QModelIndex()));
    pieChart->setModel(model2);
    
    setCurrentDiagram("req_diagram_");
	    
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    createDockWindows();

    setWindowTitle(tr("unREAL"));

    resize(800,600);  
    
    
}

void MainWindow::createEditors(){

    db.exec("create table diagram (id int primary key auto_increment, name varchar(20), type varchar(20))");
    
    reqEditor = new Editor;
    
    diagrams = new QSignalMapper(this);
    elements = new QSignalMapper(this);
    
    for(QList<QAction*>::iterator it = reqEditor->actions.begin(); it != reqEditor->actions.end(); it++){
        connect(*it, SIGNAL(triggered()), elements, SLOT(map()));
        elements->setMapping(*it, (*it)->data().toString());
    }    

    connect(elements, SIGNAL(mapped(const QString&)), this, SLOT(addElement(const QString&)));
    connect(diagrams, SIGNAL(mapped(const QString&)), this, SLOT(setCurrentDiagram(const QString&)));

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

    addReqDiagramAct = new QAction(tr("Add Requirements diagram"), this);
    connect(addReqDiagramAct, SIGNAL(triggered()), this, SLOT(addDiagram()));

    removeDiagramAct = new QAction(tr("Mutilate Diagram"), this);
    connect(removeDiagramAct, SIGNAL(triggered()), this, SLOT(removeDiagram()));

    removeElementAct = new QAction(tr("Mutilate Element"), this);
    connect(removeElementAct, SIGNAL(triggered()), this, SLOT(removeElement()));

    clearAct = new QAction(tr("clear all"), this);
    connect(clearAct, SIGNAL(triggered()), this, SLOT(clear()));

//    connect(addReqDiagramAct, SIGNAL(triggered()), model1, SLOT(doNOTuseIt()));
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
    editMenu->addAction(clearAct);

    viewMenu = menuBar()->addMenu(tr("&View"));

    menuBar()->addSeparator();

    addMenu = menuBar()->addMenu(tr("&Add"));
    addMenu->addAction(addReqDiagramAct);

    menuBar()->addSeparator();

    removeMenu = menuBar()->addMenu(tr("Mutilate"));
    removeMenu->addAction(removeDiagramAct);
    removeMenu->addAction(removeElementAct);

    menuBar()->addSeparator();

    diagramsMenu = menuBar()->addMenu(tr("Diagrams"));

    menuBar()->addSeparator();

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

    diagramsToolBar = addToolBar(tr("diagrams"));
    diagramsToolBar->addAction(addReqDiagramAct);

    reqDiagramToolBar = addToolBar(tr("Requirements Diagram"));
//    QList<QAction*>::iterator it;
//    for (it = reqEditor->actions.begin(); it != reqEditor->actions.end(); it++)    
    reqDiagramToolBar->addActions(reqEditor->actions);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::createDockWindows()
{
}

void MainWindow::addDiagram(){
  
    AddDiagramDialog dialog(this);
    if(dialog.exec()){     
        QString name = dialog.eName->text();
        QAction *tmp = new QAction(name, 0);
        tmp->setData(name);
        diagramsList << tmp;        
        diagramsMenu->addAction(diagramsList.last());
        
        connect(diagramsList.last(), SIGNAL(triggered()), diagrams, SLOT(map()));
        diagrams->setMapping(diagramsList.last(), diagramsList.last()->data().toString());

        qDebug() << "creating diagram " << name;
        
        model2->createDiagram(name);
        tree2->reset();
        curDiagram = name;

///        pieChart->setRootIndex(model2->getDiagramIndex(name));
  //      pieChart->reset();
    }   
}

void MainWindow::addElement(const QString &dname){
//dbg;    
    AddElementDialog dialog(this);
    if(dialog.exec()){
        QString name = dialog.eName->text();
        QString desc = dialog.eDescription->text();
        QString prio = dialog.ePriority->text();
        QString srce = dialog.eSource->text();
        QString stat = dialog.eStatus->text();
        
        QList<QString> list;                
        list << curDiagram << name << desc << prio << srce << stat << dname;
        
        //qDebug() << curDiagram << name << desc << prio << srce << stat << dname;
        QString fields;
        fields = "name, description, priority, source, status, diagram";
        model2->createElement(list, fields);
	
        tree2->reset(); 
        tree1->reset();
	
	pieChart->setRootIndex(model2->index(0, 0, QModelIndex()));
	pieChart->reset();
    }
}

void MainWindow::removeElement(){
//dbg;    
    RemoveElementDialog dialog(this);
    if(dialog.exec()){
        QString name    = dialog.eName->text().section('/',1,1);
        QString diagram = dialog.eName->text().section('/',0,0);
    
        qDebug() << "removing: " << name << diagram;

        //qDebug() << curDiagram << name << desc << prio << srce << stat << dname;
        model2->removeElement(name, diagram);
        tree2->reset(); 
        tree1->reset();
	
	pieChart->setRootIndex(model2->index(0, 0, QModelIndex()));
	pieChart->reset();
    }
}

void MainWindow::removeDiagram(){
//dbg;    
    RemoveDiagramDialog dialog(this);
    if(dialog.exec()){
        QString name    = dialog.eName->text().section('/',0,0);
    
        qDebug() << "removing diagram: " << name;

        //qDebug() << curDiagram << name << desc << prio << srce << stat << dname;
        model2->removeDiagram(name);
        tree2->reset(); 
        tree1->reset();
	
	pieChart->setRootIndex(model2->index(0, 0, QModelIndex()));
	pieChart->reset();
    }
}


void MainWindow::setCurrentDiagram(const QString &dname){
    curDiagram = dname;
}

void MainWindow::clear(){
// not working
    QSqlQuery q;
    db.exec("drop database unreal");
    db.exec("create database unreal");
    createEditors();
    tree1->reset();
    tree2->reset();
    table->reset();
}




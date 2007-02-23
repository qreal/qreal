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

//#define _LONG_DEBUG
#include <QtGui>
#include "mainwindow.h"
#include "dbg.h"
#include <QDebug>
#include <stdio.h>

MainWindow::MainWindow()
{
dbg;
    sqlite   = false;
    driver   = "QMYSQL";
    hostName = "127.0.0.1";
    dbName   = "unreal";
    userName = "unreal";
    passwd   = "";
    runREAL();
}
 
void MainWindow::runREAL(){ 
dbg;
    reconnect();
    if (dbOpened){
        createEditors();
    }    
     
    model1 = new ObjectExplorerModel(db);

    dock = new QDockWidget(tr("object explorer"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    tree1 = new QTreeView();
    tree1->setModel(model1);
    tree1->setHeader(0);

    dock->setWidget(tree1);
    addDockWidget(Qt::LeftDockWidgetArea, dock);

    model2 = new DiagramExplorerModel(db);
    diagramsList = model2->getDiagramsList();
    
    if( diagramsList.size() > 0 )
        setCurrentDiagram(diagramsList.at(0));
    else    
        setCurrentDiagram("");
    
    dock3 = new QDockWidget(tr("diagram explorer"), this);
    dock3->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    tree2 = new QTreeView();
    tree2->setModel(model2);
    tree2->setHeader(0);
 
    dock3->setWidget(tree2);
    addDockWidget(Qt::RightDockWidgetArea, dock3);


    connect(model2, SIGNAL(dataAboutToBeChanged(const QModelIndex &, QVariant)), model1, SLOT(updateData(const QModelIndex &, QVariant)));
    connect(model1, SIGNAL(dataAboutToBeChanged(const QModelIndex &, QVariant)), model2, SLOT(updateData(const QModelIndex &, QVariant)));
    connect(model2, SIGNAL(elemAdded( QStringList )), model1, SLOT(addElem( QStringList )));
    connect(model2, SIGNAL(elemRemoved( QStringList )), model1, SLOT(removeElem( QStringList )));
    
    propModel = new PropertyEditorModel();
    propModel->setTable("nFeatured"); // dirty hack, i don't yet now how to do it another way 
    propModel->setFilter("id=(select MAX(id) from nFeatured)");
    propModel->select();
    trans = new TransposeProxyModel();
    trans->setSourceModel(propModel);
    //propModel = new TableModel(db);
    table = new QTableView();
    table->setModel(trans);
    //table->setHeader(0);
    table->show();
    
    connect(tree2, SIGNAL(clicked( const QModelIndex&) ), this, SLOT( setFocus(const QModelIndex )));
    connect(tree1, SIGNAL(clicked( const QModelIndex&) ), this, SLOT( setFocus(const QModelIndex )));
    
    dock4 = new QDockWidget(tr("property editor"), this);
    dock4->setWidget(table);
    addDockWidget(Qt::LeftDockWidgetArea, dock4);
    
    pieChart = new ExampleEditor();
    setCentralWidget(pieChart);
    
    pieChart->setRootIndex(model2->getDiagramIndex(currentDiagram()));
    pieChart->setModel(model2);
    
    createActions();
    //if( count == 0 )
        createMenus();
    createDiagramMenu();
    createToolBars();
    createStatusBar();
    createDockWindows();

    setWindowTitle(tr("unREAL"));

    resize(1024, 768);  
    
    count++;    
}

void MainWindow::clearAll(){ 
dbg;
    removeDockWidget(dock);
    removeDockWidget(dock3);
    removeDockWidget(dock4);
    
    model1->disconnect();
    model2->disconnect();
    tree1->disconnect();
    tree2->disconnect();
    delete model1;
    delete tree1;
    delete model2;
    delete tree2;
    delete propModel;
    delete trans;
    delete table;
    delete pieChart;
   
    delete dock;
    delete dock3;
    delete dock4;
   
    deleteDiagramMenu();
    deleteActions();
    deleteEditors();
    deleteMenus();
    deleteToolBars();

    diagramsList.clear();
    diagramsActList.clear();
}

void MainWindow::setFocus( QModelIndex ind ){
dbg;
   propModel->setFocus(ind);
   table->reset();
}

void MainWindow::createEditors(){
dbg;
    db.exec("create table diagram (id int primary key auto_increment, name varchar(20), type varchar(20))");
    
    reqEditor = new Editor;
    
    diagrams = new QSignalMapper(this);
    qDebug() << "diagrams created;";
    elements = new QSignalMapper(this);
    
    for(QList<QAction*>::iterator it = reqEditor->actions.begin(); it != reqEditor->actions.end(); it++){
        connect(*it, SIGNAL(triggered()), elements, SLOT(map()));
        elements->setMapping(*it, (*it)->data().toString());
    }    

    connect(elements, SIGNAL(mapped(const QString&)), this, SLOT(addElement(const QString&)));
    connect(diagrams, SIGNAL(mapped(const QString&)), this, SLOT(setRootDiagram(const QString&)));

}

void MainWindow::deleteEditors(){
dbg;
    diagrams->disconnect();
    elements->disconnect();
    
    delete reqEditor;
    delete diagrams;
    delete elements;
}

void MainWindow::setRootDiagram( QString name ){
dbg;
    setCurrentDiagram(name);
    adjustPieChart();
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
{dbg;
QMessageBox::about(this, tr("about unREAL"),
            tr("this is the baseline of <b>unREAL</b> -- troo Qt-based version of <b>REAL</b>\n"
                      "<center>(approved and blessed by the Elder Gods themselves!!)</center>"));
}

void MainWindow::createActions()
{
dbg;
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

    optionsAct = new QAction(tr("Settings"), this);
    connect(optionsAct, SIGNAL(triggered()), this, SLOT(options()));

    addLinkAct = new QAction(tr("Add link"), this);
    connect(addLinkAct, SIGNAL(triggered()), this, SLOT(addLink()));

}

void MainWindow::deleteActions()
{dbg;
    delete newLetterAct;
    saveAct->disconnect();
    delete saveAct;
    printAct->disconnect();
    delete printAct;
    undoAct->disconnect();
    delete undoAct;
    quitAct->disconnect();
    delete quitAct;
    aboutAct->disconnect();
    delete aboutAct;
    aboutQtAct->disconnect();
    delete aboutQtAct;
    addReqDiagramAct->disconnect();
    delete addReqDiagramAct;
    removeDiagramAct->disconnect();
    delete removeDiagramAct;
    removeElementAct->disconnect();
    delete removeElementAct;
    clearAct->disconnect();
    delete clearAct;
    optionsAct->disconnect();
    delete optionsAct;
}

void MainWindow::createMenus()
{dbg;
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newLetterAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAct);
    editMenu->addAction(optionsAct);

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

void MainWindow::deleteMenus()
{dbg;
    delete fileMenu;
    delete editMenu;
    delete viewMenu;
    delete addMenu;
    delete removeMenu;
    delete diagramsMenu;
    delete helpMenu;
    delete menuWidget();
    delete menuBar();
}

void MainWindow::createToolBars()
{dbg;
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newLetterAct);
    fileToolBar->addAction(saveAct);
    fileToolBar->addAction(printAct);

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(undoAct);

    diagramsToolBar = addToolBar(tr("diagrams"));
    diagramsToolBar->addAction(addReqDiagramAct);
    diagramsToolBar->addAction(addLinkAct);
    
    if (dbOpened){
        reqDiagramToolBar = addToolBar(tr("Requirements Diagram"));
        reqDiagramToolBar->addActions(reqEditor->actions);
    }
}

void MainWindow::deleteToolBars()
{dbg;
    delete fileToolBar;
    delete editToolBar;
    delete diagramsToolBar;
    delete reqDiagramToolBar;
}

void MainWindow::createDiagramMenu(){
dbg;
    QAction* act;
    for ( int i=0; i<diagramsList.size(); i++ ){
        act = new QAction(diagramsList.at(i), 0);
        act->setData(diagramsList.at(i));
        diagramsActList << act;
    }
    for(QList<QAction*>::iterator it = diagramsActList.begin(); it != diagramsActList.end(); it++){
        connect(*it, SIGNAL(triggered()), diagrams, SLOT(map()));
        diagrams->setMapping(*it, (*it)->data().toString());
        diagramsMenu->addAction(*it);
    } 
}

void MainWindow::deleteDiagramMenu(){
dbg;
    for ( int i=0; i<diagramsActList.size(); i++ ){
        diagramsActList.at(i)->disconnect();
        delete diagramsActList.at(i);
    }    
}


void MainWindow::createStatusBar()
{dbg;
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::createDockWindows()
{
}

void MainWindow::addDiagram(){
 dbg; 
    AddDiagramDialog dialog(this);
    if(dialog.exec()){     
        QString name = dialog.eName->text();
        QAction *tmp = new QAction(name, 0);
        tmp->setData(name);
        diagramsActList << tmp;        
        diagramsMenu->clear();
        for(int i=0; i<diagramsActList.size(); i++){
            connect(diagramsActList.at(i), SIGNAL(triggered()), diagrams, SLOT(map()));
            diagrams->setMapping(diagramsActList.at(i),diagramsActList.at(i)->data().toString());
            diagramsMenu->addAction(diagramsActList.at(i));
        }    
        QStringList l;
        l << name;
        model2->insert(false, "", l);
        diagramsList << name;
        setCurrentDiagram(name);
        adjustPieChart();
    }   
}

void MainWindow::addElement(const QString &type){
dbg;    
    AddElementDialog dialog(this);
    if(dialog.exec()){
        QString name = dialog.eName->text();
        QString desc = dialog.eDescription->text();
        QString prio = dialog.ePriority->text();
        QString source = dialog.eSource->text();
        QString stat = dialog.eStatus->text();
        
        QList<QString> list;                
        QString fields;
        
        list << curDiagram << name << type << desc << prio << source << stat;
        fields = "name, description, priority, source, status, diagram";
        
        model2->insert(true, fields, list);
	
        adjustPieChart();
    }
}

void MainWindow::removeElement(){
dbg;    
    RemoveElementDialog dialog(this);
    if(dialog.exec()){
        QString name    = dialog.eName->text().section('/',1,1);
        QString diagram = dialog.eName->text().section('/',0,0);
   qDebug() << "deleting: " << diagram << name; 
        QStringList list;
        list << name << diagram; 
        model2->remove(true, list);
        adjustPieChart();
    }
}

void MainWindow::removeDiagram(){
dbg;    
    RemoveDiagramDialog dialog(this);
    if(dialog.exec()){
        QString name = dialog.eName->text();

        for(int i=0; i<diagramsActList.size(); i++){
            if (diagramsActList.at(i)->data().toString() == name)
                diagramsActList.removeAt(i);
        }
    
        QStringList l;
        l << name;
        model2->remove(false, l);

        diagramsList.removeAt(diagramsList.indexOf(name));
        if ( diagramsList.size() > 0 && currentDiagram() == name)
            setCurrentDiagram(diagramsList.at(0));
        if ( diagramsList.size() == 0 )
            setCurrentDiagram("");
        adjustPieChart();
    }
}

void MainWindow::addLink(){
dbg;    
    AddLinkDialog dialog(this);
    if(dialog.exec()){
        QString name = dialog.eName->text();
        QString from = dialog.eFrom->text();
        QString to   = dialog.eTo->text();
        QString type = "eP2N";
        
        QList<QString> list;                
        QString fields;
        
qDebug() << "from: " << from;
qDebug() << "to  : " << to;

        list << curDiagram << name << type << from << to;
        fields = "name, beginsWith, endsWith, diagram";
        
        model2->insert(true, fields, list);
	
        adjustPieChart();
    }
}

void MainWindow::setCurrentDiagram(const QString &dname){
dbg;
    curDiagram = dname;
}

QString MainWindow::currentDiagram(){
dbg;
    return curDiagram;
}

void MainWindow::adjustPieChart(){
dbg;
    if(currentDiagram() != "")
        pieChart->setRootIndex(model2->getDiagramIndex(currentDiagram()));
    else 
        pieChart->setRootIndex(QModelIndex());
	pieChart->reset();
}

void MainWindow::reconnect(){
dbg;
    if (sqlite){
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(":memory:");
    }
    else {
        db = QSqlDatabase::addDatabase(driver);
        db.setDatabaseName(dbName);
        db.setUserName(userName);
        db.setPassword(passwd);
        db.setHostName(hostName);
    }
    if (db.open())
        dbOpened = true;
    else{
        QMessageBox::information(0, tr("AAAAAAAAAAAAAAAAA"), tr("database connection error\ncheck your repo settings"));
        dbOpened = false;
    }    
    
}

void MainWindow::options(){
dbg;
    QStringList list;
    list << hostName << dbName << userName;

    RepoOptionsDialog dialog(list, this);
    if (dialog.exec()){
        hostName = dialog.eHostName->text();
        dbName   = dialog.eDBName->text();
        userName = dialog.eUserName->text();
        passwd   = dialog.ePasswd->text();
       
        db.close();
        clearAll();
        runREAL();
    }
}

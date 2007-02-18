//=====================================================================
// Project:      unREAL
// File Name:    mainwindow.h
// Description:  Application's main window class
//
// Created:      hz :)
// Revision:     03-Feb-07
//
// Author:       
//===================================================================== 

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeView>
#include <QTableView>
#include <QHeaderView>
#include <QSqlQueryModel>
#include <QSignalMapper>
#include "pieview.h"
#include "objectexplorermodel.h"
#include "diagramexplorermodel.h"
#include "propertyeditormodel.h"
#include "editor.h"
#include "dialogs.h"

class QAction;
class QListWidget;
class QMenu;
class QTextEdit;
class QTreeWidget;
class AddDiagramDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

private slots:
    void save();
    void print();
    void undo();
    void about();
    void addDiagram();
    void addElement(const QString&);
    void removeElement();
    void removeDiagram();
    void setCurrentDiagram(const QString&);
    
    
private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createDockWindows();
    void createEditors();
    void adjustPieChart();
    QString currentDiagram();

    QTreeWidget *foobar;
    QTreeWidget *treeWidget;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *addMenu;
    QMenu *removeMenu;
    QMenu *debugMenu;
    QMenu *helpMenu;
    QMenu *diagramsMenu;
    QMenu *useMenu;
    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QToolBar *diagramsToolBar;
    QToolBar *reqDiagramToolBar;
    QAction *newLetterAct;
    QAction *saveAct;
    QAction *printAct;
    QAction *undoAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *quitAct;
    QAction *addReqDiagramAct;
    QAction *removeDiagramAct;
    QAction *removeElementAct;
    QAction *debugAct;
    QAction *useSQLiteAct;
    QAction *useMySQLAct;
    QAction *clearAct;
    QList<QAction*> diagramsActList;
    QSignalMapper *elements;
    QSignalMapper *diagrams;

    //req diagram
    QAction *nFeaturedAct; //nodes
    QAction *nConcAltAct;
    QAction *nLeafAct;
    QAction *nParentAct;
    QAction *nParentMandAct;
    QAction *nParentOrAct;
    QAction *nParentOptAct;
    QAction *nParentAltAct;
    QAction *nParentOptAltAct;
    QAction *eP2NAct; // edge

    Editor *reqEditor;
    ObjectExplorerModel *model1;
    DiagramExplorerModel *model2;
    PropertyEditorModel *propModel;
    QSqlQueryModel *q;
    QTreeView *tree1;
    QTreeView *tree2;
    QTableView *table;
    PieView *pieChart;
    int curNum;
    int elemID;
    QString curDiagram;
    QStringList diagramsList;
    bool sqlite;
    bool dbOpened;

    QSqlDatabase db;
};


#endif

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

#include "editorview.h"
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
class DragWidget;

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
    void addLink();
    void removeElement();
    void removeDiagram();
    void setCurrentDiagram(const QString&);
    void setRootDiagram( QString );
    void setFocus(QModelIndex);  
    void move();
    void options();
    void refreshDiagrams();
    
private:
    void runREAL();
    void clearAll();
    void createActions();
    void createMenus();
    void createDiagramMenu();
    void createToolBars();
    void createStatusBar();
    void createDockWindows();
    void createEditors();
    void adjustPieChart();
    void reconnect();
    void deleteActions();
    void deleteEditors();
    void deleteMenus();
    void deleteDiagramMenu();
    void deleteToolBars();
 
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
    QToolBar *miscToolBar;
    QAction *newLetterAct;
    QAction *saveAct;
    QAction *printAct;
    QAction *undoAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *quitAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *toggleOpenGLAct;
    QAction *toggleAntialiasingAct;
    QAction *addReqDiagramAct;
    QAction *removeDiagramAct;
    QAction *removeElementAct;
    QAction *moveAct;
    QAction *debugAct;
    QAction *useSQLiteAct;
    QAction *useMySQLAct;
    QAction *clearAct;
    QAction *optionsAct;
    QAction *addLinkAct;
    QList<QAction*> diagramsActList;
    QSignalMapper *elements;
    QSignalMapper *diagrams;
    QDockWidget *dock;
    QDockWidget *dock3, *dock4, *dock5, *dock6;

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
    //TransposeProxyModel *trans;
    //TableModel *propModel;
    QSqlQueryModel *q;
    QTreeView *tree1;
    QTreeView *tree2;
    QTableView *table;
    EditorView *pieChart;
    QGraphicsView *miniMap;
    DragWidget *toolbox;
    QString curDiagram;
    QStringList diagramsList;
    int count;
    bool sqlite;
    bool dbOpened;
    QString driver;
    QString hostName;
    QString userName;
    QString dbName;
    QString passwd;

    QSqlDatabase db;
};


#endif

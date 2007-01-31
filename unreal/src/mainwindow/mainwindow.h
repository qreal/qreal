#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeView>
#include <QTableView>
#include <QHeaderView>
#include <QSqlQueryModel>
#include "pieview.h"
#include "objectexplorermodel.h"
#include "diagramexplorermodel.h"
#include "propertyeditormodel.h"
#include "editor.h"

class QAction;
class QListWidget;
class QMenu;
class QTextEdit;
class QTreeWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

private slots:
    void newLetter();
    void save();
    void print();
    void undo();
    void about();
    void insertCustomer(const QString &customer);
    void addParagraph(const QString &paragraph);
    void addActor();
    //void debug();
  
private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createDockWindows();
    
    void readFile();


    QTreeWidget *foobar;
    QTreeWidget *treeWidget;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *addMenu;
    QMenu *debugMenu;
    QMenu *helpMenu;
    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QToolBar *reqDiagramToolBar;
    QAction *newLetterAct;
    QAction *saveAct;
    QAction *printAct;
    QAction *undoAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *quitAct;
    QAction *addActorAct;
    QAction *debugAct;

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
};

#endif


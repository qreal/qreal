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
#include "proxymodel.h"

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
    QAction *newLetterAct;
    QAction *saveAct;
    QAction *printAct;
    QAction *undoAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *quitAct;
    QAction *addActorAct;
    QAction *debugAct;

    ObjectExplorerModel *model1;
    DiagramExplorerModel *model2;
    QSqlQueryModel *q;
    Proxy *proxy;
    QTreeView *tree1;
    QTreeView *tree2;
    QTableView *table;
    PieView *pieChart;
};

#endif

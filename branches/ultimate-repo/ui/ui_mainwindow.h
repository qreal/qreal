/********************************************************************************
** Form generated from reading ui file 'mainwindow.ui'
**
** Created: Sun Feb 3 16:28:08 2008
**      by: Qt User Interface Compiler version 4.3.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QGraphicsView>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QSlider>
#include <QtGui/QStatusBar>
#include <QtGui/QTableView>
#include <QtGui/QToolBar>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "editorview.h"
#include "palettetoolbox.h"

class Ui_MainWindowUi
{
public:
    QAction *actionQuit;
    QAction *actionConnect;
    QAction *actionZoom_In;
    QAction *actionZoom_Out;
    QAction *actionAntialiasing;
    QAction *actionOpenGL_Renderer;
    QAction *actionPrint;
    QAction *actionMakeSvg;
    QAction *actionDisconnect;
    QAction *actionBeginTransaction;
    QAction *actionCommitTransaction;
    QAction *actionRollbackTransaction;
    QAction *actionDeleteFromDiagram;
    QAction *actionHelp;
    QAction *actionAbout;
    QAction *actionAboutQt;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    EditorView *view;
    QMenuBar *menubar;
    QMenu *menu_File;
    QMenu *menu_View;
    QMenu *menuHelp;
    QStatusBar *statusbar;
    QDockWidget *objectDock;
    QWidget *dockWidgetContents_3;
    QVBoxLayout *vboxLayout;
    QTreeView *objectExplorer;
    QDockWidget *diagramDock;
    QWidget *dockWidgetContents_2;
    QHBoxLayout *hboxLayout;
    QTreeView *diagramExplorer;
    QToolBar *fileToolbar;
    QDockWidget *minimapDock;
    QWidget *dockWidgetContents_4;
    QVBoxLayout *vboxLayout1;
    QGraphicsView *minimapView;
    QSlider *minimapZoomSlider;
    QDockWidget *paletteDock;
    QWidget *dockWidgetContents_5;
    QHBoxLayout *hboxLayout1;
    PaletteToolbox *paletteToolbox;
    QToolBar *viewToolbar;
    QDockWidget *propertyDock;
    QWidget *dockWidgetContents;
    QVBoxLayout *vboxLayout2;
    QTableView *propertyEditor;

    void setupUi(QMainWindow *MainWindowUi)
    {
    if (MainWindowUi->objectName().isEmpty())
        MainWindowUi->setObjectName(QString::fromUtf8("MainWindowUi"));
    MainWindowUi->resize(579, 458);
    MainWindowUi->setWindowIcon(QIcon(QString::fromUtf8(":/icons/designer_datatable.png")));
    actionQuit = new QAction(MainWindowUi);
    actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
    actionConnect = new QAction(MainWindowUi);
    actionConnect->setObjectName(QString::fromUtf8("actionConnect"));
    actionConnect->setIcon(QIcon(QString::fromUtf8(":/icons/database_connected.png")));
    actionZoom_In = new QAction(MainWindowUi);
    actionZoom_In->setObjectName(QString::fromUtf8("actionZoom_In"));
    actionZoom_In->setIcon(QIcon(QString::fromUtf8(":/icons/zoomin.png")));
    actionZoom_Out = new QAction(MainWindowUi);
    actionZoom_Out->setObjectName(QString::fromUtf8("actionZoom_Out"));
    actionZoom_Out->setIcon(QIcon(QString::fromUtf8(":/icons/zoomout.png")));
    actionAntialiasing = new QAction(MainWindowUi);
    actionAntialiasing->setObjectName(QString::fromUtf8("actionAntialiasing"));
    actionAntialiasing->setCheckable(true);
    actionAntialiasing->setChecked(true);
    actionOpenGL_Renderer = new QAction(MainWindowUi);
    actionOpenGL_Renderer->setObjectName(QString::fromUtf8("actionOpenGL_Renderer"));
    actionOpenGL_Renderer->setCheckable(true);
    actionPrint = new QAction(MainWindowUi);
    actionPrint->setObjectName(QString::fromUtf8("actionPrint"));
    actionPrint->setIcon(QIcon(QString::fromUtf8(":/icons/print.png")));
    actionMakeSvg = new QAction(MainWindowUi);
    actionMakeSvg->setObjectName(QString::fromUtf8("actionMakeSvg"));
    actionDisconnect = new QAction(MainWindowUi);
    actionDisconnect->setObjectName(QString::fromUtf8("actionDisconnect"));
    actionDisconnect->setIcon(QIcon(QString::fromUtf8(":/icons/database_disconnect.png")));
    actionBeginTransaction = new QAction(MainWindowUi);
    actionBeginTransaction->setObjectName(QString::fromUtf8("actionBeginTransaction"));
    actionBeginTransaction->setEnabled(false);
    actionBeginTransaction->setIcon(QIcon(QString::fromUtf8(":/icons/database_connect.png")));
    actionCommitTransaction = new QAction(MainWindowUi);
    actionCommitTransaction->setObjectName(QString::fromUtf8("actionCommitTransaction"));
    actionCommitTransaction->setEnabled(false);
    actionCommitTransaction->setIcon(QIcon(QString::fromUtf8(":/icons/database_commit.png")));
    actionRollbackTransaction = new QAction(MainWindowUi);
    actionRollbackTransaction->setObjectName(QString::fromUtf8("actionRollbackTransaction"));
    actionRollbackTransaction->setEnabled(false);
    actionRollbackTransaction->setIcon(QIcon(QString::fromUtf8(":/icons/database_revert.png")));
    actionDeleteFromDiagram = new QAction(MainWindowUi);
    actionDeleteFromDiagram->setObjectName(QString::fromUtf8("actionDeleteFromDiagram"));
    actionHelp = new QAction(MainWindowUi);
    actionHelp->setObjectName(QString::fromUtf8("actionHelp"));
    actionAbout = new QAction(MainWindowUi);
    actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
    actionAboutQt = new QAction(MainWindowUi);
    actionAboutQt->setObjectName(QString::fromUtf8("actionAboutQt"));
    centralwidget = new QWidget(MainWindowUi);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    gridLayout = new QGridLayout(centralwidget);
    gridLayout->setSpacing(0);
    gridLayout->setMargin(0);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->setHorizontalSpacing(0);
    gridLayout->setVerticalSpacing(0);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    view = new EditorView(centralwidget);
    view->setObjectName(QString::fromUtf8("view"));

    gridLayout->addWidget(view, 0, 0, 1, 1);

    MainWindowUi->setCentralWidget(centralwidget);
    menubar = new QMenuBar(MainWindowUi);
    menubar->setObjectName(QString::fromUtf8("menubar"));
    menubar->setGeometry(QRect(0, 0, 579, 25));
    menu_File = new QMenu(menubar);
    menu_File->setObjectName(QString::fromUtf8("menu_File"));
    menu_View = new QMenu(menubar);
    menu_View->setObjectName(QString::fromUtf8("menu_View"));
    menuHelp = new QMenu(menubar);
    menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
    MainWindowUi->setMenuBar(menubar);
    statusbar = new QStatusBar(MainWindowUi);
    statusbar->setObjectName(QString::fromUtf8("statusbar"));
    MainWindowUi->setStatusBar(statusbar);
    objectDock = new QDockWidget(MainWindowUi);
    objectDock->setObjectName(QString::fromUtf8("objectDock"));
    objectDock->setAllowedAreas(Qt::AllDockWidgetAreas);
    dockWidgetContents_3 = new QWidget(objectDock);
    dockWidgetContents_3->setObjectName(QString::fromUtf8("dockWidgetContents_3"));
    vboxLayout = new QVBoxLayout(dockWidgetContents_3);
    vboxLayout->setSpacing(6);
    vboxLayout->setMargin(0);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    vboxLayout->setContentsMargins(0, 0, 0, 0);
    objectExplorer = new QTreeView(dockWidgetContents_3);
    objectExplorer->setObjectName(QString::fromUtf8("objectExplorer"));
    objectExplorer->setAcceptDrops(true);
    objectExplorer->setDragDropMode(QAbstractItemView::DragDrop);

    vboxLayout->addWidget(objectExplorer);

    objectDock->setWidget(dockWidgetContents_3);
    MainWindowUi->addDockWidget(static_cast<Qt::DockWidgetArea>(1), objectDock);
    diagramDock = new QDockWidget(MainWindowUi);
    diagramDock->setObjectName(QString::fromUtf8("diagramDock"));
    diagramDock->setAllowedAreas(Qt::AllDockWidgetAreas);
    dockWidgetContents_2 = new QWidget(diagramDock);
    dockWidgetContents_2->setObjectName(QString::fromUtf8("dockWidgetContents_2"));
    hboxLayout = new QHBoxLayout(dockWidgetContents_2);
    hboxLayout->setSpacing(6);
    hboxLayout->setMargin(0);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    hboxLayout->setContentsMargins(0, 0, 0, 0);
    diagramExplorer = new QTreeView(dockWidgetContents_2);
    diagramExplorer->setObjectName(QString::fromUtf8("diagramExplorer"));
    diagramExplorer->setContextMenuPolicy(Qt::ActionsContextMenu);
    diagramExplorer->setAcceptDrops(true);
    diagramExplorer->setDragDropMode(QAbstractItemView::DragDrop);

    hboxLayout->addWidget(diagramExplorer);

    diagramDock->setWidget(dockWidgetContents_2);
    MainWindowUi->addDockWidget(static_cast<Qt::DockWidgetArea>(1), diagramDock);
    fileToolbar = new QToolBar(MainWindowUi);
    fileToolbar->setObjectName(QString::fromUtf8("fileToolbar"));
    fileToolbar->setOrientation(Qt::Horizontal);
    MainWindowUi->addToolBar(Qt::TopToolBarArea, fileToolbar);
    minimapDock = new QDockWidget(MainWindowUi);
    minimapDock->setObjectName(QString::fromUtf8("minimapDock"));
    dockWidgetContents_4 = new QWidget(minimapDock);
    dockWidgetContents_4->setObjectName(QString::fromUtf8("dockWidgetContents_4"));
    vboxLayout1 = new QVBoxLayout(dockWidgetContents_4);
    vboxLayout1->setSpacing(6);
    vboxLayout1->setMargin(0);
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
    vboxLayout1->setContentsMargins(0, 0, 0, 0);
    minimapView = new QGraphicsView(dockWidgetContents_4);
    minimapView->setObjectName(QString::fromUtf8("minimapView"));
    minimapView->setDragMode(QGraphicsView::ScrollHandDrag);

    vboxLayout1->addWidget(minimapView);

    minimapZoomSlider = new QSlider(dockWidgetContents_4);
    minimapZoomSlider->setObjectName(QString::fromUtf8("minimapZoomSlider"));
    minimapZoomSlider->setMaximum(50);
    minimapZoomSlider->setValue(25);
    minimapZoomSlider->setOrientation(Qt::Horizontal);

    vboxLayout1->addWidget(minimapZoomSlider);

    minimapDock->setWidget(dockWidgetContents_4);
    MainWindowUi->addDockWidget(static_cast<Qt::DockWidgetArea>(2), minimapDock);
    paletteDock = new QDockWidget(MainWindowUi);
    paletteDock->setObjectName(QString::fromUtf8("paletteDock"));
    dockWidgetContents_5 = new QWidget(paletteDock);
    dockWidgetContents_5->setObjectName(QString::fromUtf8("dockWidgetContents_5"));
    hboxLayout1 = new QHBoxLayout(dockWidgetContents_5);
    hboxLayout1->setSpacing(6);
    hboxLayout1->setMargin(0);
    hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
    hboxLayout1->setContentsMargins(0, 0, 0, 0);
    paletteToolbox = new PaletteToolbox(dockWidgetContents_5);
    paletteToolbox->setObjectName(QString::fromUtf8("paletteToolbox"));

    hboxLayout1->addWidget(paletteToolbox);

    paletteDock->setWidget(dockWidgetContents_5);
    MainWindowUi->addDockWidget(static_cast<Qt::DockWidgetArea>(2), paletteDock);
    viewToolbar = new QToolBar(MainWindowUi);
    viewToolbar->setObjectName(QString::fromUtf8("viewToolbar"));
    MainWindowUi->addToolBar(Qt::TopToolBarArea, viewToolbar);
    propertyDock = new QDockWidget(MainWindowUi);
    propertyDock->setObjectName(QString::fromUtf8("propertyDock"));
    dockWidgetContents = new QWidget(propertyDock);
    dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
    vboxLayout2 = new QVBoxLayout(dockWidgetContents);
    vboxLayout2->setSpacing(6);
    vboxLayout2->setMargin(0);
    vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
    vboxLayout2->setContentsMargins(0, 0, 0, 0);
    propertyEditor = new QTableView(dockWidgetContents);
    propertyEditor->setObjectName(QString::fromUtf8("propertyEditor"));
    propertyEditor->setAlternatingRowColors(true);
    propertyEditor->setShowGrid(false);

    vboxLayout2->addWidget(propertyEditor);

    propertyDock->setWidget(dockWidgetContents);
    MainWindowUi->addDockWidget(static_cast<Qt::DockWidgetArea>(1), propertyDock);

    menubar->addAction(menu_File->menuAction());
    menubar->addAction(menu_View->menuAction());
    menubar->addAction(menuHelp->menuAction());
    menu_File->addAction(actionConnect);
    menu_File->addAction(actionDisconnect);
    menu_File->addSeparator();
    menu_File->addAction(actionBeginTransaction);
    menu_File->addAction(actionCommitTransaction);
    menu_File->addAction(actionRollbackTransaction);
    menu_File->addSeparator();
    menu_File->addAction(actionPrint);
    menu_File->addAction(actionMakeSvg);
    menu_File->addSeparator();
    menu_File->addAction(actionQuit);
    menu_View->addAction(actionZoom_In);
    menu_View->addAction(actionZoom_Out);
    menu_View->addSeparator();
    menu_View->addAction(actionAntialiasing);
    menu_View->addAction(actionOpenGL_Renderer);
    menuHelp->addAction(actionHelp);
    menuHelp->addSeparator();
    menuHelp->addAction(actionAbout);
    menuHelp->addAction(actionAboutQt);
    fileToolbar->addAction(actionConnect);
    fileToolbar->addAction(actionDisconnect);
    fileToolbar->addSeparator();
    fileToolbar->addAction(actionBeginTransaction);
    fileToolbar->addAction(actionCommitTransaction);
    fileToolbar->addAction(actionRollbackTransaction);
    fileToolbar->addSeparator();
    fileToolbar->addAction(actionPrint);
    viewToolbar->addAction(actionZoom_In);
    viewToolbar->addAction(actionZoom_Out);

    retranslateUi(MainWindowUi);

    QMetaObject::connectSlotsByName(MainWindowUi);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowUi)
    {
    MainWindowUi->setWindowTitle(QApplication::translate("MainWindowUi", "unREAL", 0, QApplication::UnicodeUTF8));
    actionQuit->setText(QApplication::translate("MainWindowUi", "&Quit", 0, QApplication::UnicodeUTF8));
    actionConnect->setText(QApplication::translate("MainWindowUi", "&Connect", 0, QApplication::UnicodeUTF8));
    actionZoom_In->setText(QApplication::translate("MainWindowUi", "Zoom In", 0, QApplication::UnicodeUTF8));
    actionZoom_Out->setText(QApplication::translate("MainWindowUi", "Zoom Out", 0, QApplication::UnicodeUTF8));
    actionAntialiasing->setText(QApplication::translate("MainWindowUi", "Antialiasing", 0, QApplication::UnicodeUTF8));
    actionOpenGL_Renderer->setText(QApplication::translate("MainWindowUi", "OpenGL Renderer", 0, QApplication::UnicodeUTF8));
    actionPrint->setText(QApplication::translate("MainWindowUi", "Print", 0, QApplication::UnicodeUTF8));
    actionMakeSvg->setText(QApplication::translate("MainWindowUi", "Export to SVG", 0, QApplication::UnicodeUTF8));
    actionDisconnect->setText(QApplication::translate("MainWindowUi", "&Disconnect", 0, QApplication::UnicodeUTF8));
    actionBeginTransaction->setText(QApplication::translate("MainWindowUi", "Begin Transaction", 0, QApplication::UnicodeUTF8));
    actionCommitTransaction->setText(QApplication::translate("MainWindowUi", "Commit Transaction", 0, QApplication::UnicodeUTF8));
    actionRollbackTransaction->setText(QApplication::translate("MainWindowUi", "Rollback Transaction", 0, QApplication::UnicodeUTF8));
    actionDeleteFromDiagram->setText(QApplication::translate("MainWindowUi", "Delete", 0, QApplication::UnicodeUTF8));
    actionHelp->setText(QApplication::translate("MainWindowUi", "Small Help", 0, QApplication::UnicodeUTF8));
    actionAbout->setText(QApplication::translate("MainWindowUi", "About...", 0, QApplication::UnicodeUTF8));
    actionAboutQt->setText(QApplication::translate("MainWindowUi", "About Qt...", 0, QApplication::UnicodeUTF8));
    menu_File->setTitle(QApplication::translate("MainWindowUi", "&File", 0, QApplication::UnicodeUTF8));
    menu_View->setTitle(QApplication::translate("MainWindowUi", "&View", 0, QApplication::UnicodeUTF8));
    menuHelp->setTitle(QApplication::translate("MainWindowUi", "Help", 0, QApplication::UnicodeUTF8));
    objectDock->setWindowTitle(QApplication::translate("MainWindowUi", "Object Explorer", 0, QApplication::UnicodeUTF8));
    diagramDock->setWindowTitle(QApplication::translate("MainWindowUi", "Diagram Explorer", 0, QApplication::UnicodeUTF8));
    minimapDock->setWindowTitle(QApplication::translate("MainWindowUi", "Mini Map", 0, QApplication::UnicodeUTF8));
    paletteDock->setWindowTitle(QApplication::translate("MainWindowUi", "Palette", 0, QApplication::UnicodeUTF8));
    viewToolbar->setWindowTitle(QApplication::translate("MainWindowUi", "toolBar", 0, QApplication::UnicodeUTF8));
    propertyDock->setWindowTitle(QApplication::translate("MainWindowUi", "Property Editor", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindowUi: public Ui_MainWindowUi {};
} // namespace Ui

#endif // UI_MAINWINDOW_H

/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Fri Jul 1 16:37:15 2011
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
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
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QSlider>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTableView>
#include <QtGui/QToolBar>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "errorlistwidget.h"
#include "paletteToolbox.h"

QT_BEGIN_NAMESPACE

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
    QAction *openNewTab;
    QAction *actionHelp;
    QAction *actionAbout;
    QAction *actionAboutQt;
    QAction *actionPlugins;
    QAction *actionShowSplash;
    QAction *actionClear;
    QAction *actionExport_to_XMI;
    QAction *actionGenerate_to_Java;
    QAction *actionSave;
    QAction *actionGenerate_editor;
    QAction *actionGenerate_to_Hascol;
    QAction *actionSave_as;
    QAction *actionOpen;
    QAction *actionParse_Hascol_sources;
    QAction *actionParse_Java_Libraries;
    QAction *actionShow_grid;
    QAction *actionShape_Edit;
    QAction *actionGenerate_Editor;
    QAction *actionParse_Editor_xml;
    QAction *actionSwitch_on_grid;
    QAction *actionCommit;
    QAction *actionShow;
    QAction *actionPreferences;
    QAction *actionCheckout;
    QAction *actionGenerate_Editor_qrmc;
    QAction *actionSwitch_on_alignment;
    QAction *actionShow_alignment;
    QAction *actionDebug;
    QAction *actionDebug_Single_step;
    QAction *actionGenerate_and_build;
    QAction *actionStart_debugger;
    QAction *actionRun;
    QAction *actionKill;
    QAction *actionClose_all;
    QAction *actionSet_Breakpoints;
    QAction *actionCont;
    QAction *actionNext;
    QAction *actionConfigure;
    QAction *actionBreak_main;
    QAction *actionNew_Diagram;
    QAction *actionStart_debugging;
    QAction *actionFullscreen;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QTabWidget *tabs;
    QMenuBar *menubar;
    QMenu *menu_File;
    QMenu *menu_View;
    QMenu *menuHelp;
    QMenu *menuSettings;
    QMenu *menuMouse_gestures;
    QMenu *menuSvn;
    QMenu *menuTools;
    QMenu *menuDebug_with_debugger;
    QMenu *menuAdvanced_options;
    QStatusBar *statusbar;
    QToolBar *fileToolbar;
    QDockWidget *minimapDock;
    QWidget *minimapDockWidgetContents;
    QVBoxLayout *vboxLayout;
    QGraphicsView *minimapView;
    QSlider *minimapZoomSlider;
    QDockWidget *paletteDock;
    QWidget *paletteDockWidgetContents;
    QHBoxLayout *hboxLayout;
    qReal::gui::PaletteToolbox *paletteToolbox;
    QToolBar *viewToolbar;
    QDockWidget *logicalModelDock;
    QWidget *logicalModelDockWidgetContents;
    QVBoxLayout *verticalLayout;
    QTreeView *logicalModelExplorer;
    QToolBar *generatorsToolbar;
    QToolBar *parsersToolbar;
    QDockWidget *errorDock;
    QWidget *errorDockWidgetContents;
    QHBoxLayout *horizontalLayout_2;
    ErrorListWidget *errorListWidget;
    QDockWidget *graphicalModelDock;
    QWidget *graphicalModelDockWidgetContents;
    QHBoxLayout *horizontalLayout;
    QTreeView *graphicalModelExplorer;
    QDockWidget *propertyDock;
    QWidget *propertyDockWidgetContents;
    QVBoxLayout *vboxLayout1;
    QTableView *propertyEditor;
    QToolBar *debuggerToolBar;

    void setupUi(QMainWindow *MainWindowUi)
    {
        if (MainWindowUi->objectName().isEmpty())
            MainWindowUi->setObjectName(QString::fromUtf8("MainWindowUi"));
        MainWindowUi->resize(1062, 523);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/qreal.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindowUi->setWindowIcon(icon);
        actionQuit = new QAction(MainWindowUi);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        actionQuit->setCheckable(false);
        actionConnect = new QAction(MainWindowUi);
        actionConnect->setObjectName(QString::fromUtf8("actionConnect"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/database_connected.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionConnect->setIcon(icon1);
        actionZoom_In = new QAction(MainWindowUi);
        actionZoom_In->setObjectName(QString::fromUtf8("actionZoom_In"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/zoomin.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionZoom_In->setIcon(icon2);
        actionZoom_Out = new QAction(MainWindowUi);
        actionZoom_Out->setObjectName(QString::fromUtf8("actionZoom_Out"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/zoomout.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionZoom_Out->setIcon(icon3);
        actionAntialiasing = new QAction(MainWindowUi);
        actionAntialiasing->setObjectName(QString::fromUtf8("actionAntialiasing"));
        actionAntialiasing->setCheckable(true);
        actionAntialiasing->setChecked(true);
        actionOpenGL_Renderer = new QAction(MainWindowUi);
        actionOpenGL_Renderer->setObjectName(QString::fromUtf8("actionOpenGL_Renderer"));
        actionOpenGL_Renderer->setCheckable(true);
        actionPrint = new QAction(MainWindowUi);
        actionPrint->setObjectName(QString::fromUtf8("actionPrint"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icons/print.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPrint->setIcon(icon4);
        actionMakeSvg = new QAction(MainWindowUi);
        actionMakeSvg->setObjectName(QString::fromUtf8("actionMakeSvg"));
        actionDisconnect = new QAction(MainWindowUi);
        actionDisconnect->setObjectName(QString::fromUtf8("actionDisconnect"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/icons/database_disconnect.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDisconnect->setIcon(icon5);
        actionBeginTransaction = new QAction(MainWindowUi);
        actionBeginTransaction->setObjectName(QString::fromUtf8("actionBeginTransaction"));
        actionBeginTransaction->setEnabled(false);
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/icons/database_connect.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionBeginTransaction->setIcon(icon6);
        actionCommitTransaction = new QAction(MainWindowUi);
        actionCommitTransaction->setObjectName(QString::fromUtf8("actionCommitTransaction"));
        actionCommitTransaction->setEnabled(false);
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/icons/database_commit.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCommitTransaction->setIcon(icon7);
        actionRollbackTransaction = new QAction(MainWindowUi);
        actionRollbackTransaction->setObjectName(QString::fromUtf8("actionRollbackTransaction"));
        actionRollbackTransaction->setEnabled(false);
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/icons/database_revert.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRollbackTransaction->setIcon(icon8);
        actionDeleteFromDiagram = new QAction(MainWindowUi);
        actionDeleteFromDiagram->setObjectName(QString::fromUtf8("actionDeleteFromDiagram"));
        actionDeleteFromDiagram->setShortcutContext(Qt::WidgetShortcut);
        openNewTab = new QAction(MainWindowUi);
        openNewTab->setObjectName(QString::fromUtf8("openNewTab"));
        actionHelp = new QAction(MainWindowUi);
        actionHelp->setObjectName(QString::fromUtf8("actionHelp"));
        actionAbout = new QAction(MainWindowUi);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionAboutQt = new QAction(MainWindowUi);
        actionAboutQt->setObjectName(QString::fromUtf8("actionAboutQt"));
        actionPlugins = new QAction(MainWindowUi);
        actionPlugins->setObjectName(QString::fromUtf8("actionPlugins"));
        actionPlugins->setCheckable(false);
        actionShowSplash = new QAction(MainWindowUi);
        actionShowSplash->setObjectName(QString::fromUtf8("actionShowSplash"));
        actionShowSplash->setCheckable(true);
        actionShowSplash->setChecked(true);
        actionClear = new QAction(MainWindowUi);
        actionClear->setObjectName(QString::fromUtf8("actionClear"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/icons/clear.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionClear->setIcon(icon9);
        actionExport_to_XMI = new QAction(MainWindowUi);
        actionExport_to_XMI->setObjectName(QString::fromUtf8("actionExport_to_XMI"));
        actionGenerate_to_Java = new QAction(MainWindowUi);
        actionGenerate_to_Java->setObjectName(QString::fromUtf8("actionGenerate_to_Java"));
        actionSave = new QAction(MainWindowUi);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionGenerate_editor = new QAction(MainWindowUi);
        actionGenerate_editor->setObjectName(QString::fromUtf8("actionGenerate_editor"));
        actionGenerate_to_Hascol = new QAction(MainWindowUi);
        actionGenerate_to_Hascol->setObjectName(QString::fromUtf8("actionGenerate_to_Hascol"));
        actionSave_as = new QAction(MainWindowUi);
        actionSave_as->setObjectName(QString::fromUtf8("actionSave_as"));
        actionOpen = new QAction(MainWindowUi);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionParse_Hascol_sources = new QAction(MainWindowUi);
        actionParse_Hascol_sources->setObjectName(QString::fromUtf8("actionParse_Hascol_sources"));
        actionParse_Java_Libraries = new QAction(MainWindowUi);
        actionParse_Java_Libraries->setObjectName(QString::fromUtf8("actionParse_Java_Libraries"));
        actionShow_grid = new QAction(MainWindowUi);
        actionShow_grid->setObjectName(QString::fromUtf8("actionShow_grid"));
        actionShow_grid->setCheckable(true);
        actionShow_grid->setChecked(true);
        actionShape_Edit = new QAction(MainWindowUi);
        actionShape_Edit->setObjectName(QString::fromUtf8("actionShape_Edit"));
        actionGenerate_Editor = new QAction(MainWindowUi);
        actionGenerate_Editor->setObjectName(QString::fromUtf8("actionGenerate_Editor"));
        actionParse_Editor_xml = new QAction(MainWindowUi);
        actionParse_Editor_xml->setObjectName(QString::fromUtf8("actionParse_Editor_xml"));
        actionSwitch_on_grid = new QAction(MainWindowUi);
        actionSwitch_on_grid->setObjectName(QString::fromUtf8("actionSwitch_on_grid"));
        actionSwitch_on_grid->setCheckable(true);
        actionCommit = new QAction(MainWindowUi);
        actionCommit->setObjectName(QString::fromUtf8("actionCommit"));
        actionShow = new QAction(MainWindowUi);
        actionShow->setObjectName(QString::fromUtf8("actionShow"));
        actionPreferences = new QAction(MainWindowUi);
        actionPreferences->setObjectName(QString::fromUtf8("actionPreferences"));
        actionCheckout = new QAction(MainWindowUi);
        actionCheckout->setObjectName(QString::fromUtf8("actionCheckout"));
        actionGenerate_Editor_qrmc = new QAction(MainWindowUi);
        actionGenerate_Editor_qrmc->setObjectName(QString::fromUtf8("actionGenerate_Editor_qrmc"));
        actionSwitch_on_alignment = new QAction(MainWindowUi);
        actionSwitch_on_alignment->setObjectName(QString::fromUtf8("actionSwitch_on_alignment"));
        actionSwitch_on_alignment->setCheckable(true);
        actionSwitch_on_alignment->setChecked(true);
        actionShow_alignment = new QAction(MainWindowUi);
        actionShow_alignment->setObjectName(QString::fromUtf8("actionShow_alignment"));
        actionShow_alignment->setCheckable(true);
        actionShow_alignment->setChecked(true);
        actionDebug = new QAction(MainWindowUi);
        actionDebug->setObjectName(QString::fromUtf8("actionDebug"));
        actionDebug_Single_step = new QAction(MainWindowUi);
        actionDebug_Single_step->setObjectName(QString::fromUtf8("actionDebug_Single_step"));
        actionGenerate_and_build = new QAction(MainWindowUi);
        actionGenerate_and_build->setObjectName(QString::fromUtf8("actionGenerate_and_build"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/icons/build.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionGenerate_and_build->setIcon(icon10);
        actionStart_debugger = new QAction(MainWindowUi);
        actionStart_debugger->setObjectName(QString::fromUtf8("actionStart_debugger"));
        actionRun = new QAction(MainWindowUi);
        actionRun->setObjectName(QString::fromUtf8("actionRun"));
        actionKill = new QAction(MainWindowUi);
        actionKill->setObjectName(QString::fromUtf8("actionKill"));
        actionClose_all = new QAction(MainWindowUi);
        actionClose_all->setObjectName(QString::fromUtf8("actionClose_all"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/icons/stop.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionClose_all->setIcon(icon11);
        actionSet_Breakpoints = new QAction(MainWindowUi);
        actionSet_Breakpoints->setObjectName(QString::fromUtf8("actionSet_Breakpoints"));
        actionCont = new QAction(MainWindowUi);
        actionCont->setObjectName(QString::fromUtf8("actionCont"));
        actionNext = new QAction(MainWindowUi);
        actionNext->setObjectName(QString::fromUtf8("actionNext"));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/icons/next.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNext->setIcon(icon12);
        actionConfigure = new QAction(MainWindowUi);
        actionConfigure->setObjectName(QString::fromUtf8("actionConfigure"));
        actionBreak_main = new QAction(MainWindowUi);
        actionBreak_main->setObjectName(QString::fromUtf8("actionBreak_main"));
        actionNew_Diagram = new QAction(MainWindowUi);
        actionNew_Diagram->setObjectName(QString::fromUtf8("actionNew_Diagram"));
        actionStart_debugging = new QAction(MainWindowUi);
        actionStart_debugging->setObjectName(QString::fromUtf8("actionStart_debugging"));
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/icons/run.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionStart_debugging->setIcon(icon13);
        actionFullscreen = new QAction(MainWindowUi);
        actionFullscreen->setObjectName(QString::fromUtf8("actionFullscreen"));
        centralwidget = new QWidget(MainWindowUi);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tabs = new QTabWidget(centralwidget);
        tabs->setObjectName(QString::fromUtf8("tabs"));

        gridLayout->addWidget(tabs, 0, 0, 1, 1);

        MainWindowUi->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindowUi);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1062, 21));
        menu_File = new QMenu(menubar);
        menu_File->setObjectName(QString::fromUtf8("menu_File"));
        menu_View = new QMenu(menubar);
        menu_View->setObjectName(QString::fromUtf8("menu_View"));
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        menuSettings = new QMenu(menubar);
        menuSettings->setObjectName(QString::fromUtf8("menuSettings"));
        menuMouse_gestures = new QMenu(menubar);
        menuMouse_gestures->setObjectName(QString::fromUtf8("menuMouse_gestures"));
        menuSvn = new QMenu(menubar);
        menuSvn->setObjectName(QString::fromUtf8("menuSvn"));
        menuTools = new QMenu(menubar);
        menuTools->setObjectName(QString::fromUtf8("menuTools"));
        menuDebug_with_debugger = new QMenu(menuTools);
        menuDebug_with_debugger->setObjectName(QString::fromUtf8("menuDebug_with_debugger"));
        menuAdvanced_options = new QMenu(menuDebug_with_debugger);
        menuAdvanced_options->setObjectName(QString::fromUtf8("menuAdvanced_options"));
        MainWindowUi->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindowUi);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindowUi->setStatusBar(statusbar);
        fileToolbar = new QToolBar(MainWindowUi);
        fileToolbar->setObjectName(QString::fromUtf8("fileToolbar"));
        fileToolbar->setOrientation(Qt::Horizontal);
        MainWindowUi->addToolBar(Qt::TopToolBarArea, fileToolbar);
        minimapDock = new QDockWidget(MainWindowUi);
        minimapDock->setObjectName(QString::fromUtf8("minimapDock"));
        minimapDockWidgetContents = new QWidget();
        minimapDockWidgetContents->setObjectName(QString::fromUtf8("minimapDockWidgetContents"));
        vboxLayout = new QVBoxLayout(minimapDockWidgetContents);
        vboxLayout->setSpacing(6);
        vboxLayout->setContentsMargins(0, 0, 0, 0);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        minimapView = new QGraphicsView(minimapDockWidgetContents);
        minimapView->setObjectName(QString::fromUtf8("minimapView"));
        minimapView->setDragMode(QGraphicsView::ScrollHandDrag);

        vboxLayout->addWidget(minimapView);

        minimapZoomSlider = new QSlider(minimapDockWidgetContents);
        minimapZoomSlider->setObjectName(QString::fromUtf8("minimapZoomSlider"));
        minimapZoomSlider->setMaximum(50);
        minimapZoomSlider->setValue(25);
        minimapZoomSlider->setOrientation(Qt::Horizontal);

        vboxLayout->addWidget(minimapZoomSlider);

        minimapDock->setWidget(minimapDockWidgetContents);
        MainWindowUi->addDockWidget(static_cast<Qt::DockWidgetArea>(2), minimapDock);
        paletteDock = new QDockWidget(MainWindowUi);
        paletteDock->setObjectName(QString::fromUtf8("paletteDock"));
        paletteDockWidgetContents = new QWidget();
        paletteDockWidgetContents->setObjectName(QString::fromUtf8("paletteDockWidgetContents"));
        hboxLayout = new QHBoxLayout(paletteDockWidgetContents);
        hboxLayout->setSpacing(6);
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        paletteToolbox = new qReal::gui::PaletteToolbox(paletteDockWidgetContents);
        paletteToolbox->setObjectName(QString::fromUtf8("paletteToolbox"));

        hboxLayout->addWidget(paletteToolbox);

        paletteDock->setWidget(paletteDockWidgetContents);
        MainWindowUi->addDockWidget(static_cast<Qt::DockWidgetArea>(2), paletteDock);
        viewToolbar = new QToolBar(MainWindowUi);
        viewToolbar->setObjectName(QString::fromUtf8("viewToolbar"));
        MainWindowUi->addToolBar(Qt::TopToolBarArea, viewToolbar);
        logicalModelDock = new QDockWidget(MainWindowUi);
        logicalModelDock->setObjectName(QString::fromUtf8("logicalModelDock"));
        logicalModelDock->setFeatures(QDockWidget::AllDockWidgetFeatures);
        logicalModelDockWidgetContents = new QWidget();
        logicalModelDockWidgetContents->setObjectName(QString::fromUtf8("logicalModelDockWidgetContents"));
        verticalLayout = new QVBoxLayout(logicalModelDockWidgetContents);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        logicalModelExplorer = new QTreeView(logicalModelDockWidgetContents);
        logicalModelExplorer->setObjectName(QString::fromUtf8("logicalModelExplorer"));
        logicalModelExplorer->setContextMenuPolicy(Qt::ActionsContextMenu);
        logicalModelExplorer->setAcceptDrops(true);
        logicalModelExplorer->setDragEnabled(false);
        logicalModelExplorer->setDragDropOverwriteMode(false);
        logicalModelExplorer->setDragDropMode(QAbstractItemView::DragDrop);
        logicalModelExplorer->setDefaultDropAction(Qt::IgnoreAction);

        verticalLayout->addWidget(logicalModelExplorer);

        logicalModelDock->setWidget(logicalModelDockWidgetContents);
        MainWindowUi->addDockWidget(static_cast<Qt::DockWidgetArea>(1), logicalModelDock);
        generatorsToolbar = new QToolBar(MainWindowUi);
        generatorsToolbar->setObjectName(QString::fromUtf8("generatorsToolbar"));
        MainWindowUi->addToolBar(Qt::TopToolBarArea, generatorsToolbar);
        parsersToolbar = new QToolBar(MainWindowUi);
        parsersToolbar->setObjectName(QString::fromUtf8("parsersToolbar"));
        MainWindowUi->addToolBar(Qt::TopToolBarArea, parsersToolbar);
        errorDock = new QDockWidget(MainWindowUi);
        errorDock->setObjectName(QString::fromUtf8("errorDock"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(errorDock->sizePolicy().hasHeightForWidth());
        errorDock->setSizePolicy(sizePolicy);
        errorDock->setMinimumSize(QSize(91, 113));
        errorDock->setMaximumSize(QSize(524287, 524287));
        errorDock->setAcceptDrops(false);
        errorDock->setAutoFillBackground(false);
        errorDockWidgetContents = new QWidget();
        errorDockWidgetContents->setObjectName(QString::fromUtf8("errorDockWidgetContents"));
        horizontalLayout_2 = new QHBoxLayout(errorDockWidgetContents);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        errorListWidget = new ErrorListWidget(errorDockWidgetContents);
        errorListWidget->setObjectName(QString::fromUtf8("errorListWidget"));

        horizontalLayout_2->addWidget(errorListWidget);

        errorDock->setWidget(errorDockWidgetContents);
        MainWindowUi->addDockWidget(static_cast<Qt::DockWidgetArea>(8), errorDock);
        graphicalModelDock = new QDockWidget(MainWindowUi);
        graphicalModelDock->setObjectName(QString::fromUtf8("graphicalModelDock"));
        graphicalModelDockWidgetContents = new QWidget();
        graphicalModelDockWidgetContents->setObjectName(QString::fromUtf8("graphicalModelDockWidgetContents"));
        horizontalLayout = new QHBoxLayout(graphicalModelDockWidgetContents);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        graphicalModelExplorer = new QTreeView(graphicalModelDockWidgetContents);
        graphicalModelExplorer->setObjectName(QString::fromUtf8("graphicalModelExplorer"));
        graphicalModelExplorer->setContextMenuPolicy(Qt::ActionsContextMenu);
        graphicalModelExplorer->setAcceptDrops(true);

        horizontalLayout->addWidget(graphicalModelExplorer);

        graphicalModelDock->setWidget(graphicalModelDockWidgetContents);
        MainWindowUi->addDockWidget(static_cast<Qt::DockWidgetArea>(1), graphicalModelDock);
        propertyDock = new QDockWidget(MainWindowUi);
        propertyDock->setObjectName(QString::fromUtf8("propertyDock"));
        propertyDockWidgetContents = new QWidget();
        propertyDockWidgetContents->setObjectName(QString::fromUtf8("propertyDockWidgetContents"));
        vboxLayout1 = new QVBoxLayout(propertyDockWidgetContents);
        vboxLayout1->setSpacing(6);
        vboxLayout1->setContentsMargins(0, 0, 0, 0);
        vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
        propertyEditor = new QTableView(propertyDockWidgetContents);
        propertyEditor->setObjectName(QString::fromUtf8("propertyEditor"));
        propertyEditor->setMaximumSize(QSize(16777214, 16777215));
        propertyEditor->setAlternatingRowColors(true);
        propertyEditor->setShowGrid(false);

        vboxLayout1->addWidget(propertyEditor);

        propertyDock->setWidget(propertyDockWidgetContents);
        MainWindowUi->addDockWidget(static_cast<Qt::DockWidgetArea>(1), propertyDock);
        debuggerToolBar = new QToolBar(MainWindowUi);
        debuggerToolBar->setObjectName(QString::fromUtf8("debuggerToolBar"));
        MainWindowUi->addToolBar(Qt::TopToolBarArea, debuggerToolBar);

        menubar->addAction(menu_File->menuAction());
        menubar->addAction(menu_View->menuAction());
        menubar->addAction(menuTools->menuAction());
        menubar->addAction(menuSvn->menuAction());
        menubar->addAction(menuSettings->menuAction());
        menubar->addAction(menuMouse_gestures->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menu_File->addAction(actionOpen);
        menu_File->addAction(actionSave);
        menu_File->addAction(actionSave_as);
        menu_File->addAction(actionPrint);
        menu_File->addAction(actionMakeSvg);
        menu_File->addSeparator();
        menu_File->addAction(actionQuit);
        menu_View->addAction(actionZoom_In);
        menu_View->addAction(actionZoom_Out);
        menu_View->addSeparator();
        menu_View->addAction(actionAntialiasing);
        menu_View->addAction(actionOpenGL_Renderer);
        menu_View->addAction(actionShowSplash);
        menuHelp->addAction(actionHelp);
        menuHelp->addSeparator();
        menuHelp->addAction(actionAbout);
        menuHelp->addAction(actionAboutQt);
        menuSettings->addAction(actionPreferences);
        menuSettings->addAction(actionPlugins);
        menuSettings->addAction(actionShow_grid);
        menuSettings->addAction(actionShow_alignment);
        menuSettings->addAction(actionSwitch_on_grid);
        menuSettings->addAction(actionSwitch_on_alignment);
        menuMouse_gestures->addAction(actionShow);
        menuSvn->addAction(actionCommit);
        menuSvn->addAction(actionCheckout);
        menuTools->addAction(actionClear);
        menuTools->addAction(actionExport_to_XMI);
        menuTools->addAction(actionGenerate_to_Java);
        menuTools->addAction(actionGenerate_to_Hascol);
        menuTools->addAction(actionDebug);
        menuTools->addAction(actionDebug_Single_step);
        menuTools->addAction(menuDebug_with_debugger->menuAction());
        menuTools->addAction(actionShape_Edit);
        menuTools->addAction(actionGenerate_Editor);
        menuTools->addAction(actionParse_Editor_xml);
        menuTools->addSeparator();
        menuTools->addAction(actionGenerate_Editor_qrmc);
        menuDebug_with_debugger->addAction(actionGenerate_and_build);
        menuDebug_with_debugger->addAction(actionStart_debugging);
        menuDebug_with_debugger->addAction(actionNext);
        menuDebug_with_debugger->addAction(actionClose_all);
        menuDebug_with_debugger->addAction(menuAdvanced_options->menuAction());
        menuAdvanced_options->addAction(actionStart_debugger);
        menuAdvanced_options->addAction(actionConfigure);
        menuAdvanced_options->addAction(actionSet_Breakpoints);
        menuAdvanced_options->addAction(actionBreak_main);
        menuAdvanced_options->addAction(actionRun);
        menuAdvanced_options->addAction(actionCont);
        menuAdvanced_options->addAction(actionKill);
        fileToolbar->addAction(actionPrint);
        fileToolbar->addAction(actionNew_Diagram);
        fileToolbar->addAction(actionSave);
        viewToolbar->addAction(actionZoom_In);
        viewToolbar->addAction(actionZoom_Out);
        viewToolbar->addSeparator();
        viewToolbar->addAction(actionFullscreen);
        generatorsToolbar->addAction(actionGenerate_to_Hascol);
        generatorsToolbar->addAction(actionGenerate_to_Java);
        generatorsToolbar->addAction(actionExport_to_XMI);
        generatorsToolbar->addAction(actionShape_Edit);
        parsersToolbar->addAction(actionParse_Hascol_sources);
        parsersToolbar->addAction(actionParse_Java_Libraries);
        debuggerToolBar->addAction(actionStart_debugging);
        debuggerToolBar->addAction(actionClose_all);
        debuggerToolBar->addAction(actionNext);

        retranslateUi(MainWindowUi);

        QMetaObject::connectSlotsByName(MainWindowUi);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowUi)
    {
        MainWindowUi->setWindowTitle(QApplication::translate("MainWindowUi", "QReal", 0, QApplication::UnicodeUTF8));
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
        actionDeleteFromDiagram->setShortcut(QApplication::translate("MainWindowUi", "Del", 0, QApplication::UnicodeUTF8));
        openNewTab->setText(QApplication::translate("MainWindowUi", "Open in new tab", 0, QApplication::UnicodeUTF8));
        actionHelp->setText(QApplication::translate("MainWindowUi", "Small Help", 0, QApplication::UnicodeUTF8));
        actionAbout->setText(QApplication::translate("MainWindowUi", "About...", 0, QApplication::UnicodeUTF8));
        actionAboutQt->setText(QApplication::translate("MainWindowUi", "About Qt...", 0, QApplication::UnicodeUTF8));
        actionPlugins->setText(QApplication::translate("MainWindowUi", "Plugins...", 0, QApplication::UnicodeUTF8));
        actionShowSplash->setText(QApplication::translate("MainWindowUi", "Show splashscreen", 0, QApplication::UnicodeUTF8));
        actionClear->setText(QApplication::translate("MainWindowUi", "Exterminatus", 0, QApplication::UnicodeUTF8));
        actionExport_to_XMI->setText(QApplication::translate("MainWindowUi", "Export to XMI", 0, QApplication::UnicodeUTF8));
        actionGenerate_to_Java->setText(QApplication::translate("MainWindowUi", "Generate to Java", 0, QApplication::UnicodeUTF8));
        actionSave->setText(QApplication::translate("MainWindowUi", "Save", 0, QApplication::UnicodeUTF8));
        actionGenerate_editor->setText(QApplication::translate("MainWindowUi", "Generate editor", 0, QApplication::UnicodeUTF8));
        actionGenerate_to_Hascol->setText(QApplication::translate("MainWindowUi", "Generate to Hascol", 0, QApplication::UnicodeUTF8));
        actionSave_as->setText(QApplication::translate("MainWindowUi", "Save as...", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("MainWindowUi", "Open...", 0, QApplication::UnicodeUTF8));
        actionParse_Hascol_sources->setText(QApplication::translate("MainWindowUi", "Parse Hascol sources", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionParse_Hascol_sources->setToolTip(QApplication::translate("MainWindowUi", "Build diagrams from existing Hascol sources", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionParse_Java_Libraries->setText(QApplication::translate("MainWindowUi", "Parse Java Libraries", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionParse_Java_Libraries->setToolTip(QApplication::translate("MainWindowUi", "Parse Java Libraries", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionShow_grid->setText(QApplication::translate("MainWindowUi", "Show grid", 0, QApplication::UnicodeUTF8));
        actionShape_Edit->setText(QApplication::translate("MainWindowUi", "Shape Editor", 0, QApplication::UnicodeUTF8));
        actionGenerate_Editor->setText(QApplication::translate("MainWindowUi", "Generate Editor", 0, QApplication::UnicodeUTF8));
        actionParse_Editor_xml->setText(QApplication::translate("MainWindowUi", "Parse Editor xml", 0, QApplication::UnicodeUTF8));
        actionSwitch_on_grid->setText(QApplication::translate("MainWindowUi", "Switch on grid", 0, QApplication::UnicodeUTF8));
        actionCommit->setText(QApplication::translate("MainWindowUi", "Commit", 0, QApplication::UnicodeUTF8));
        actionShow->setText(QApplication::translate("MainWindowUi", "Show", 0, QApplication::UnicodeUTF8));
        actionPreferences->setText(QApplication::translate("MainWindowUi", "Preferences...", 0, QApplication::UnicodeUTF8));
        actionCheckout->setText(QApplication::translate("MainWindowUi", "Checkout...", 0, QApplication::UnicodeUTF8));
        actionGenerate_Editor_qrmc->setText(QApplication::translate("MainWindowUi", "Generate Editor (qrmc)", 0, QApplication::UnicodeUTF8));
        actionSwitch_on_alignment->setText(QApplication::translate("MainWindowUi", "Switch on alignment", 0, QApplication::UnicodeUTF8));
        actionShow_alignment->setText(QApplication::translate("MainWindowUi", "Show alignment", 0, QApplication::UnicodeUTF8));
        actionDebug->setText(QApplication::translate("MainWindowUi", "Debug", 0, QApplication::UnicodeUTF8));
        actionDebug->setShortcut(QApplication::translate("MainWindowUi", "F9", 0, QApplication::UnicodeUTF8));
        actionDebug_Single_step->setText(QApplication::translate("MainWindowUi", "Debug (Single step)", 0, QApplication::UnicodeUTF8));
        actionDebug_Single_step->setShortcut(QApplication::translate("MainWindowUi", "F10", 0, QApplication::UnicodeUTF8));
        actionGenerate_and_build->setText(QApplication::translate("MainWindowUi", "Generate and build", 0, QApplication::UnicodeUTF8));
        actionGenerate_and_build->setShortcut(QApplication::translate("MainWindowUi", "Ctrl+F9", 0, QApplication::UnicodeUTF8));
        actionStart_debugger->setText(QApplication::translate("MainWindowUi", "Start debugger", 0, QApplication::UnicodeUTF8));
        actionStart_debugger->setShortcut(QApplication::translate("MainWindowUi", "Ctrl+F1", 0, QApplication::UnicodeUTF8));
        actionRun->setText(QApplication::translate("MainWindowUi", "Run", 0, QApplication::UnicodeUTF8));
        actionRun->setShortcut(QApplication::translate("MainWindowUi", "Ctrl+F5", 0, QApplication::UnicodeUTF8));
        actionKill->setText(QApplication::translate("MainWindowUi", "Kill", 0, QApplication::UnicodeUTF8));
        actionKill->setShortcut(QApplication::translate("MainWindowUi", "Ctrl+F7", 0, QApplication::UnicodeUTF8));
        actionClose_all->setText(QApplication::translate("MainWindowUi", "Stop debugging", 0, QApplication::UnicodeUTF8));
        actionClose_all->setShortcut(QApplication::translate("MainWindowUi", "Ctrl+F12", 0, QApplication::UnicodeUTF8));
        actionSet_Breakpoints->setText(QApplication::translate("MainWindowUi", "Set breakpoints", 0, QApplication::UnicodeUTF8));
        actionSet_Breakpoints->setIconText(QApplication::translate("MainWindowUi", "Set breakpoints", 0, QApplication::UnicodeUTF8));
        actionSet_Breakpoints->setShortcut(QApplication::translate("MainWindowUi", "Ctrl+F3", 0, QApplication::UnicodeUTF8));
        actionCont->setText(QApplication::translate("MainWindowUi", "Cont", 0, QApplication::UnicodeUTF8));
        actionCont->setShortcut(QApplication::translate("MainWindowUi", "Ctrl+F6", 0, QApplication::UnicodeUTF8));
        actionNext->setText(QApplication::translate("MainWindowUi", "Next", 0, QApplication::UnicodeUTF8));
        actionNext->setShortcut(QApplication::translate("MainWindowUi", "Ctrl+F11", 0, QApplication::UnicodeUTF8));
        actionConfigure->setText(QApplication::translate("MainWindowUi", "Configure", 0, QApplication::UnicodeUTF8));
        actionConfigure->setShortcut(QApplication::translate("MainWindowUi", "Ctrl+F2", 0, QApplication::UnicodeUTF8));
        actionBreak_main->setText(QApplication::translate("MainWindowUi", "Break main", 0, QApplication::UnicodeUTF8));
        actionBreak_main->setShortcut(QApplication::translate("MainWindowUi", "Ctrl+F4", 0, QApplication::UnicodeUTF8));
        actionNew_Diagram->setText(QApplication::translate("MainWindowUi", "New Diagram", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionNew_Diagram->setToolTip(QApplication::translate("MainWindowUi", "Create new diagram in a current model", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionStart_debugging->setText(QApplication::translate("MainWindowUi", "Start debugging", 0, QApplication::UnicodeUTF8));
        actionStart_debugging->setShortcut(QApplication::translate("MainWindowUi", "Ctrl+F10", 0, QApplication::UnicodeUTF8));
        actionFullscreen->setText(QApplication::translate("MainWindowUi", "Fullscreen Mode", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionFullscreen->setToolTip(QApplication::translate("MainWindowUi", "Fullscreen Mode", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionFullscreen->setShortcut(QApplication::translate("MainWindowUi", "Ctrl+Shift+F", 0, QApplication::UnicodeUTF8));
        menu_File->setTitle(QApplication::translate("MainWindowUi", "&File", 0, QApplication::UnicodeUTF8));
        menu_View->setTitle(QApplication::translate("MainWindowUi", "&View", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("MainWindowUi", "&Help", 0, QApplication::UnicodeUTF8));
        menuSettings->setTitle(QApplication::translate("MainWindowUi", "&Settings", 0, QApplication::UnicodeUTF8));
        menuMouse_gestures->setTitle(QApplication::translate("MainWindowUi", "Mouse gestures", 0, QApplication::UnicodeUTF8));
        menuSvn->setTitle(QApplication::translate("MainWindowUi", "Svn", 0, QApplication::UnicodeUTF8));
        menuTools->setTitle(QApplication::translate("MainWindowUi", "Tools", 0, QApplication::UnicodeUTF8));
        menuDebug_with_debugger->setTitle(QApplication::translate("MainWindowUi", "Debug (with debugger)", 0, QApplication::UnicodeUTF8));
        menuAdvanced_options->setTitle(QApplication::translate("MainWindowUi", "Advanced options...", 0, QApplication::UnicodeUTF8));
        fileToolbar->setWindowTitle(QApplication::translate("MainWindowUi", "File Toolbar", 0, QApplication::UnicodeUTF8));
        minimapDock->setWindowTitle(QApplication::translate("MainWindowUi", "Mini Map", 0, QApplication::UnicodeUTF8));
        paletteDock->setWindowTitle(QApplication::translate("MainWindowUi", "Palette", 0, QApplication::UnicodeUTF8));
        viewToolbar->setWindowTitle(QApplication::translate("MainWindowUi", "View Toolbar", 0, QApplication::UnicodeUTF8));
        logicalModelDock->setWindowTitle(QApplication::translate("MainWindowUi", "Logical Model Explorer", 0, QApplication::UnicodeUTF8));
        generatorsToolbar->setWindowTitle(QApplication::translate("MainWindowUi", "toolBar", 0, QApplication::UnicodeUTF8));
        parsersToolbar->setWindowTitle(QApplication::translate("MainWindowUi", "toolBar", 0, QApplication::UnicodeUTF8));
        errorDock->setWindowTitle(QApplication::translate("MainWindowUi", "Error List", 0, QApplication::UnicodeUTF8));
        graphicalModelDock->setWindowTitle(QApplication::translate("MainWindowUi", "Graphical Model Explorer", 0, QApplication::UnicodeUTF8));
        propertyDock->setWindowTitle(QApplication::translate("MainWindowUi", "Property Editor", 0, QApplication::UnicodeUTF8));
        debuggerToolBar->setWindowTitle(QApplication::translate("MainWindowUi", "toolBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindowUi: public Ui_MainWindowUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

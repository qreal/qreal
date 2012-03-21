#pragma once


#include <QApplication>

#include "findDialog.h"

#include <QtCore/QSignalMapper>
#include <QtCore/QTranslator>
#include <QtCore/QDir>
#include <QtGui/QMainWindow>
#include <QtGui/QSplashScreen>
#include <QtGui/QProgressBar>
#include <QtGui/QListWidget>
#include <QtSql/QSqlDatabase>

#include "../pluginManager/editorManager.h"
#include "../pluginManager/toolPluginManager.h"
#include "propertyEditorProxyModel.h"
#include "gesturesPainterInterface.h"
#include "../dialogs/gesturesShow/gesturesWidget.h"
#include "../interpreters/visualDebugger/debuggerConnector.h"
#include "mainWindowInterpretersInterface.h"
#include "../../qrkernel/settingsManager.h"
#include "../../qrgui/dialogs/preferencesDialog.h"
#include "../textEditor/codeEditor.h"
#include "helpBrowser.h"

#include "../models/logicalModelAssistApi.h"

namespace Ui {
class MainWindowUi;
}

namespace qReal {

class EditorView;
class ListenerManager;
class VisualDebugger;

namespace models {
class Models;
}

namespace gui {
class ErrorReporter;
}

class MainWindow : public QMainWindow, public qReal::gui::MainWindowInterpretersInterface
{
	Q_OBJECT

public:
	MainWindow();
	~MainWindow();

	EditorManager* manager();
	EditorView *getCurrentTab();
	ListenerManager *listenerManager();
	GesturesPainterInterface *gesturesPainter();
	QModelIndex rootIndex() const;

	QAction *actionDeleteFromDiagram() const;

	virtual void highlight(Id const &graphicalId, bool exclusive = true);
	virtual void dehighlight(Id const &graphicalId);
	virtual void dehighlight();
	virtual ErrorReporterInterface *errorReporter();
	virtual Id activeDiagram();
	void openShapeEditor(QPersistentModelIndex const &index, int role, QString const &propertyValue);
	virtual void openSettingsDialog(QString const &tab);

	void showErrors(gui::ErrorReporter *reporter);

	/// Tells if we should display trace connections menu or not
	bool showConnectionRelatedMenus() const;

	virtual void showInTextEditor(QString const &title, QString const &text);

	virtual void reinitModels();

	virtual QWidget *windowWidget();

	virtual bool unloadPlugin(QString const &pluginName);
	virtual bool loadPlugin(QString const &fileName, QString const &pluginName);
	virtual bool pluginLoaded(QString const &pluginName);

signals:
	void gesturesShowed();
	void currentIdealGestureChanged();
	void rootDiagramChanged();

public slots:
	void deleteFromScene();
	void editWindowTitle();
	void propertyEditorScrollTo(QModelIndex const &index);

	void activateItemOrDiagram(Id const &id, bool bl = true, bool isSetSel = true);
	void activateItemOrDiagram(QModelIndex const &idx, bool bl = true, bool isSetSel = true);
	virtual void selectItem(Id const &id);

	void selectItemWithError(Id const &id);

	void showErrors(gui::ErrorReporter const * const errorReporter);

private slots:

	/// handler for find dialog 'button find' pressed
	void handleFindDialog(QString const &name);

	void setSceneFont();
	void adjustMinimapZoom(int zoom);
	void toggleShowSplash(bool show);

	void updateTabName(Id const &id);

	void settingsPlugins();

	void showAbout();
	void showHelp();

	void checkoutDialogOk();
	void checkoutDialogCancel();

	void saveAllAndOpen(QString const &dirName);


	/// wrapper for import(QString const &fileName)
	/// uses getWorkingFile(...)
	/// @return true - if all ok, false - if not ok
	bool importProject();

	/// checks parameters for integrity,then importing it
	/// @param fileName - *.qrs file to import
	/// @return true - if all ok, false - if not ok
	bool import(QString const &fileName);
	bool open(QString const &dirName);
	bool checkPluginsAndReopen(QSplashScreen* const splashScreen);
	void saveProjectAs();
	virtual void saveAll();
	void fullscreen();
	void openRecentProjectsMenu();
	bool openNewProject();
	void createProject();

	void saveDiagramAsAPicture();

	void print();
	void makeSvg();
	void showGrid(bool isChecked);

	void finalClose();
	void closeAllTabs();

	void sceneSelectionChanged();

	void exportToXmi();
	void generateToJava();
	void parseJavaLibraries();
	void applySettings();

	void deleteFromScene(QGraphicsItem *target);

	void debug();
	void debugSingleStep();
	void drawDebuggerStdOutput(QString output);
	void drawDebuggerErrOutput(QString output);
	void generateAndBuild();
	void startDebugger();
	void runProgramWithDebugger();
	void killProgramWithDebugger();
	void closeDebuggerProcessAndThread();
	void placeBreakpointsInDebugger();
	void goToNextBreakpoint();
	void goToNextInstruction();
	void configureDebugger();
	void setBreakpointAtStart();
	void startDebugging();
	void checkEditorForDebug(int index);

	void deleteFromDiagram();
	void changeMiniMapSource(int index);
	void closeTab(int index);

	/// Closes the appropriate tab if the specified index corresponds to the diagram on one of the tabs
	/// @return true if one of the tabs was closed
	bool closeTab(QModelIndex const &graphicsIndex);

	void showPreferencesDialog();

	void connectActions();
	void connectDebugActions();

	void centerOn(Id const &id);
	void graphicalModelExplorerClicked(const QModelIndex &index);
	void logicalModelExplorerClicked(const QModelIndex &index);

	void openNewTab(const QModelIndex &index);

	/// Called after current tab was changed somehow --- opened, closed, switched to other
	/// @param newIndex Index of a new active tab, -1 if there is none
	void currentTabChanged(int newIndex);

	void showGestures();
	void showAlignment(bool isChecked);
	void switchGrid(bool isChecked);
	void switchAlignment(bool isChecked);
	void setShape(QString const &data, QPersistentModelIndex const &index, int const &role);

	void openShapeEditor();

	void setDiagramCreateFlag();
	void diagramInCreateListDeselect();
	void diagramInCreateListSelected(int num);

	void on_actionNew_Diagram_triggered();

	void updatePaletteIcons();

	void autosave();
	void setAutoSaveParameters();
	void closeProject();
	void closeProjectAndSave();

private:

	/// @param mFindDialog - Dialod for searching model by name.
	FindDialog *mFindDialog;

	/// @param mCodeTabManager - Map that keeps pairs of opened tabs and their code areas.
	QMap<EditorView*, CodeArea*> *mCodeTabManager;

	/// Initializes a tab if it is a diagram --- sets its logical and graphical
	/// models, connects to various main window actions and so on
	/// @param tab Tab to be initialized
	/// @param rootIndex Index of a graphical model element that will be root of a diagram shown in this tab
	void initCurrentTab(EditorView * const tab, const QModelIndex &rootIndex);

	/// Sets shortcuts for a given tab which don`t have own buttons anywhere
	/// @param tab Tab to be initialized with shortcuts
	void setShortcuts(EditorView * const tab);

	void createDiagram(QString const &idString);

	void loadPlugins();

	QListWidget* createSaveListWidget();
	void suggestToCreateDiagram();

	virtual void closeEvent(QCloseEvent *event);
	void deleteFromExplorer(bool isLogicalModel);
	void keyPressEvent(QKeyEvent *event);
	QString getWorkingFile(QString const &dialogWindowTitle, bool save);

	int getTabIndex(const QModelIndex &index);

	void initGridProperties();
	void disconnectZoom(QGraphicsView* view);
	void connectZoom(QGraphicsView* view);
	void disconnectActionZoomTo(QWidget* widget);
	void connectActionZoomTo(QWidget* widget);
	void setConnectActionZoomTo(QWidget* widget);
	void clickErrorListWidget();
	void connectWindowTitle();
	void disconnectWindowTitle();

	void setShowGrid(bool isChecked);
	void setShowAlignment(bool isChecked);
	void setSwitchGrid(bool isChecked);
	void setSwitchAlignment(bool isChecked);

	void setIndexesOfPropertyEditor(Id const &id);

	/// Check if we need to hide widget in fullscreen mode or not. If we do, hide it
	/// @param dockWidget QDockWidget to hide
	/// @param name Widget's name in internal map
	void hideDockWidget(QDockWidget *dockWidget, QString const &name);

	/// Check if we need to show widget in fullscreen mode or not. If we do, show it
	/// @param dockWidget QDockWidget to show
	/// @param name Widget's name in internal map
	void showDockWidget(QDockWidget *dockWidget, QString const &name);

	QString getNextDirName(QString const &name);

	void initToolPlugins();

	QProgressBar *createProgressBar(QSplashScreen* splash);
	void initMiniMap();
	void initToolManager();
	void initTabs();
	void initDocks();
	void initWindowTitle();
	void initDebugger();
	void initExplorers();
	void initRecentProjectsMenu();

	void saveAs(QString const &saveName);

	void refreshRecentProjectsList(QString const &fileName);
	int openSaveOfferDialog();

	Ui::MainWindowUi *mUi;

	QCloseEvent *mCloseEvent;
	models::Models *mModels;
	EditorManager mEditorManager;
	ToolPluginManager mToolManager;
	ListenerManager *mListenerManager;
	PropertyEditorModel mPropertyModel;
	GesturesWidget *mGesturesWidget;

	QVector<bool> mSaveListChecked;
	bool mDiagramCreateFlag;

	QStringList mDiagramsList;
	QModelIndex mRootIndex;

	DebuggerConnector *mDebuggerConnector;
	VisualDebugger *mVisualDebugger;
	gui::ErrorReporter *mErrorReporter;  // Has ownership

	/// Fullscreen mode flag
	bool mIsFullscreen;

	/// Internal map table to store info what widgets should we hide/show
	QMap<QString, bool> mDocksVisibility;

	QString mSaveFile;
	QString mTempDir;
	PreferencesDialog mPreferencesDialog;

	HelpBrowser *mHelpBrowser;
	bool mIsNewProject;
	bool mUnsavedProjectIndicator;
	QTimer mAutoSaveTimer;

	int mRecentProjectsLimit;
	QSignalMapper *mRecentProjectsMapper;
	QMenu *mRecentProjectsMenu;
};

}

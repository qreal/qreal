#pragma once

#include <QtGui/QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QtCore/QDir>
#include <QSplashScreen>
#include <QtGui>

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
#include "nxtFlashTool.h"
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
class NxtFlashTool;
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

	bool open(QString const &dirName);
	bool checkPluginsAndReopen(QSplashScreen* const splashScreen);
	void saveProjectAs();
	void saveAll();
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

	void doCheckout();
	void doCommit();
	void exportToXmi();
	void generateToJava();
	void parseJavaLibraries();
	void applySettings();

	void deleteFromScene(QGraphicsItem *target);

	void activateSubdiagram(QModelIndex const &idx);

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
	void closeTab(QModelIndex const &graphicsIndex);
	void generateEditor();
	void parseEditorXml();
	void generateToHascol();
	void parseHascol();
	void showPreferencesDialog();

	void generateRobotSourceCode();
	void flashRobot();
	void uploadProgram();

	void connectActions();
	void connectDebugActions();

	void centerOn(Id const &id);
	void graphicalModelExplorerClicked(const QModelIndex &index);
	void logicalModelExplorerClicked(const QModelIndex &index);

	void openNewTab(const QModelIndex &index);
	void initCurrentTab(const QModelIndex &rootIndex);

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

private:
	void createDiagram(const QString &idString);
	void loadNewEditor(QString const &directoryName, QString const &metamodelName,
			QString const &commandFirst, QString const &commandSecond, QString const &extension, QString const &prefix);

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

	/** @brief Check if we need to hide widget in fullscreen mode or not. If we do, hide it
		@param dockWidget QDockWidget to hide
		@param name Widget's name in internal map
	*/
	void hideDockWidget(QDockWidget *dockWidget, QString name);

	/** @brief Check if we need to show widget in fullscreen mode or not. If we do, show it
		@param dockWidget QDockWidget to show
		@param name Widget's name in internal map
	*/
	void showDockWidget(QDockWidget *dockWidget, QString name);

	QString getNextDirName(QString const &name);

	void initToolPlugins();
	void checkNxtTools();

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

	gui::NxtFlashTool *mFlashTool;

	bool mNxtToolsPresent;
	HelpBrowser *mHelpBrowser;
	bool mIsNewProject;
	bool mUnsavedProjectIndicator;
	QTimer mAutoSaveTimer;

	int mRecentProjectsLimit;
	QSignalMapper *mRecentProjectsMapper;
	QMenu *mRecentProjectsMenu;
};

}

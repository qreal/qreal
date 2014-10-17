#pragma once

#include <QtCore/QSignalMapper>
#include <QtCore/QDir>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSplashScreen>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QListWidget>
#include <QtSql/QSqlDatabase>

#include <qrkernel/settingsManager.h>

#include "mainwindow/mainWindowInterpretersInterface.h"
#include "mainwindow/mainWindowDockInterface.h"
#include "mainwindow/propertyEditorProxyModel.h"
#include "mainwindow/findManager.h"
#include "mainwindow/referenceList.h"
#include "mainwindow/projectManager/projectManager.h"
#include "mainwindow/tabWidget.h"
#include "mainwindow/qscintillaTextEdit.h"
#include "mainwindow/filterObject.h"
#include "mainwindow/startWidget/startWidget.h"
#include "mainwindow/propertyEditorProxyModel.h"

#include "pluginManager/editorManagerInterface.h"
#include "pluginManager/editorManager.h"
#include "pluginManager/interpreterEditorManager.h"
#include "pluginManager/proxyEditorManager.h"
#include "pluginManager/toolPluginManager.h"
#include "pluginManager/exploser.h"

#include "models/logicalModelAssistApi.h"
#include "view/propertyEditorView.h"
#include "controller/controller.h"
#include "toolPluginInterface/systemEvents.h"

#include "gestures/gesturesPainterInterface.h"
#include "gestures/gesturesWidget.h"

#include "dialogs/preferencesDialog.h"
#include "dialogs/findReplaceDialog.h"
#include "dialogs/suggestToCreateDiagramDialog.h"

#include "textEditor/codeEditor.h"
#include "textEditor/textManager.h"

#include "scriptAPI/scriptAPI.h"

namespace Ui {
class MainWindowUi;
}

namespace qReal {

class EditorView;
class ListenerManager;
class SceneCustomizer;

namespace models {
class Models;
}

namespace gui {
class ErrorReporter;
class PaletteTree;
}

class MainWindow : public QMainWindow
		, public qReal::gui::MainWindowInterpretersInterface
		, public qReal::gui::MainWindowDockInterface
{
	Q_OBJECT

public:
	MainWindow(QString const &fileToOpen = QString());
	~MainWindow();

	EditorManagerInterface &editorManager();
	EditorView *getCurrentTab() const;
	bool isCurrentTabShapeEdit() const;
	ListenerManager *listenerManager() const;
	models::Models *models() const;
	Exploser &exploser();
	Controller *controller() const;
	PropertyEditorView *propertyEditor() const;
	QTreeView *graphicalModelExplorer() const;
	QTreeView *logicalModelExplorer() const;
	PropertyEditorModel &propertyModel();
	ToolPluginManager &toolManager();

	gestures::GesturesPainterInterface *gesturesPainter() const;
	QModelIndex rootIndex() const;

	QAction *actionDeleteFromDiagram() const;

	virtual void highlight(Id const &graphicalId, bool exclusive = true, QColor const &color = Qt::red);
	virtual void dehighlight(Id const &graphicalId);
	virtual void dehighlight();
	virtual ErrorReporterInterface *errorReporter();
	virtual Id activeDiagram() const;
	void openShapeEditor(QPersistentModelIndex const &index, int role, QString const &propertyValue
		, bool useTypedPorts);
	void openQscintillaTextEditor(QPersistentModelIndex const &index, int const role, QString const &propertyValue);
	void openShapeEditor(Id const &id, QString const &propertyValue, EditorManagerInterface *editorManagerProxy
		, bool useTypedPorts);
	void showAndEditPropertyInTextEditor(QString const &title, QString const &text, QPersistentModelIndex const &index
			, int const &role);
	void openReferenceList(QPersistentModelIndex const &index, QString const &referenceType, QString const &propertyValue
			, int role);
	virtual void openSettingsDialog(QString const &tab);

	void showErrors(gui::ErrorReporter *reporter);

	/// Tells if we should display trace connections menu or not
	//virtual void showInTextEditor(QFileInfo const &fileInfo);
	virtual void reinitModels();

	virtual QWidget *windowWidget();

	virtual bool unloadPlugin(QString const &pluginName);
	virtual bool loadPlugin(QString const &fileName, QString const &pluginName);
	virtual bool pluginLoaded(QString const &pluginName);

	virtual void saveDiagramAsAPictureToFile(QString const &fileName);
	virtual void arrangeElementsByDotRunner(QString const &algorithm, QString const &absolutePathToDotFiles);
	virtual IdList selectedElementsOnActiveDiagram();
	virtual void updateActiveDiagram();
	virtual void deleteElementFromDiagram(Id const &id);

	virtual void reportOperation(invocation::LongOperation *operation);
	virtual QWidget *currentTab();
	virtual void openTab(QWidget *tab, QString const &title);
	virtual void closeTab(QWidget *tab);

	/// Closes tab having given id as root id. If there is no such tab, does nothing.
	/// @param id Id of a diagram (root element) that we want to close.
	void closeDiagramTab(Id const &id);

	/// Returns editor manager proxy, which allows to change editor manager implementation.
	ProxyEditorManager &editorManagerProxy();

	/// Loads (or reloads) available editor plugins and reinits palette.
	void loadPlugins();

	/// Clears selection on all opened tabs.
	void clearSelectionOnTabs();

	/// Adds all elements from given diagram in a given editor to a palette.
	/// @param editor Id of an editor we need to add elements from.
	/// @param diagram Id of a diagram we need to add elements from.
	void addEditorElementsToPalette(const Id &editor, const Id &diagram);

	virtual QDockWidget *logicalModelDock() const;
	virtual QDockWidget *graphicalModelDock() const;
	virtual QDockWidget *propertyEditorDock() const;
	virtual QDockWidget *errorReporterDock() const;
	virtual QDockWidget *paletteDock() const;

	virtual void tabifyDockWidget(QDockWidget *first, QDockWidget *second);
	virtual void addDockWidget(Qt::DockWidgetArea area, QDockWidget *dockWidget);

	QListIterator<EditorView *> openedEditorViews() const;

	void setTabText(QWidget *tab, QString const &text);

	void beginPaletteModification() override;
	void setElementInPaletteVisible(Id const &metatype, bool visible) override;
	void setVisibleForAllElementsInPalette(bool visible) override;
	void setElementInPaletteEnabled(Id const &metatype, bool enabled) override;
	void setEnabledForAllElementsInPalette(bool enabled) override;
	void endPaletteModification() override;

signals:
	void gesturesShowed();
	void currentIdealGestureChanged();
	void rootDiagramChanged();

public slots:
	void deleteFromScene();
	void propertyEditorScrollTo(QModelIndex const &index);

	virtual void activateItemOrDiagram(Id const &id, bool setSelected = true);
	void activateItemOrDiagram(QModelIndex const &idx, bool setSelected = true);
	virtual void selectItem(Id const &id);
	virtual void selectItemOrDiagram(Id const &graphicalId);

	void selectItemWithError(Id const &id);
	void showErrors(gui::ErrorReporter const * const errorReporter);

	void changePaletteRepresentation();
	void closeStartTab();
	void closeAllTabs();
	void refreshRecentProjectsList(QString const &fileName);
	void createDiagram(QString const &idString);
	/// Creates project with specified root diagram
	bool createProject(QString const &diagramIdString);

	void openFirstDiagram();
	void closeTabsWithRemovedRootElements();
	void changeWindowTitle();

private slots:
	/// Suggests user to select a root diagram for the new project
	/// if more than one diagram loaded or creates project with the only diagram
	/// as root otherwise
	void createProject();

	/// Diagram opening must happen after plugins initialization
	void initPluginsAndStartWidget();
	void initToolPlugins();

	/// handler for menu 'button find' pressed
	void showFindDialog();

	void setSceneFont();
	void adjustMinimapZoom(int zoom);
	void toggleShowSplash(bool show);

	void updateTabName(Id const &id);

	void showAbout();
	void showHelp();

	void fullscreen();
	void openRecentProjectsMenu();

	void saveDiagramAsAPicture();

	void print();
	void makeSvg();
	void showGrid(bool isChecked);

	void sceneSelectionChanged();

	void applySettings();
	void resetToolbarSize(int size);

	commands::AbstractCommand *logicalDeleteCommand(QGraphicsItem *target);
	commands::AbstractCommand *graphicalDeleteCommand(QGraphicsItem *target);
	commands::AbstractCommand *logicalDeleteCommand(QModelIndex const &index);
	commands::AbstractCommand *graphicalDeleteCommand(QModelIndex const &index);
	commands::AbstractCommand *logicalDeleteCommand(Id const &index);
	commands::AbstractCommand *graphicalDeleteCommand(Id const &index);
	void appendExplosionsCommands(commands::AbstractCommand *parentCommand, Id const &logicalId);

	void deleteFromDiagram();

	void cropSceneToItems();

	void closeCurrentTab();
	void closeTab(int index);

	/// Closes the appropriate tab if the specified index corresponds to the diagram on one of the tabs
	/// @return true if one of the tabs was closed
	bool closeTab(QModelIndex const &graphicsIndex);

	void showPreferencesDialog();

	void initSettingsManager();
	void connectActions();
	void initActionsFromSettings();
	void connectActionsForUXInfo();

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

	void setData(QString const &data, QPersistentModelIndex const &index, int const &role);
	void setReference(QStringList const &data, QPersistentModelIndex const &index, int const &role);
	void openShapeEditor();

	void updatePaletteIcons();
	void setTextChanged(bool changed);

	void setUsabilityMode(bool mode);
	void startUsabilityTest();
	void finishUsabilityTest();

private:
	void checkForUpdates();
	void showUpdatesDialog();

	/// Initializes a tab if it is a diagram --- sets its logical and graphical
	/// models, connects to various main window actions and so on
	/// @param tab Tab to be initialized
	/// @param rootIndex Index of a graphical model element that will be root of a diagram shown in this tab
	void initCurrentTab(EditorView * const tab, const QModelIndex &rootIndex);

	/// Sets shortcuts for a given tab which don`t have own buttons anywhere
	/// @param tab Tab to be initialized with shortcuts
	void setShortcuts(EditorView * const tab);

	void setDefaultShortcuts();

	void registerMetaTypes();

	QListWidget* createSaveListWidget();

	virtual void closeEvent(QCloseEvent *event);

	void deleteFromExplorer(bool isLogicalModel);
	void deleteItems(IdList &itemsToDelete, bool global = false);

	QString getSaveFileName(QString const &dialogWindowTitle);
	QString getOpenFileName(QString const &dialogWindowTitle);
	QString getWorkingFile(QString const &dialogWindowTitle, bool save);

	void selectItemInLogicalModel(Id const &id);
	void switchToTab(int index);
	int getTabIndex(const QModelIndex &index);

	void initGridProperties();
	void disconnectZoom(QGraphicsView* view);
	void connectZoom(QGraphicsView* view);
	void disconnectActionZoomTo(QWidget* widget);
	void connectActionZoomTo(QWidget* widget);
	void setConnectActionZoomTo(QWidget* widget);
	void clickErrorListWidget();

	void setShowGrid(bool isChecked);
	void setShowAlignment(bool isChecked);
	void setSwitchGrid(bool isChecked);
	void setSwitchAlignment(bool isChecked);

	void addActionOrSubmenu(QMenu *target, ActionInfo const &actionOrMenu);

	void setIndexesOfPropertyEditor(Id const &id);

	void setBackReference(QPersistentModelIndex const &index, QString const &data);
	void removeOldBackReference(QPersistentModelIndex const &index, int const role);

	void removeReferences(Id const &id);

	/// Check if we need to hide widget in fullscreen mode or not. If we do, hide it
	/// @param dockWidget QDockWidget to hide
	/// @param name Widget's name in internal map
	void hideDockWidget(QDockWidget *dockWidget, QString const &name);

	/// Check if we need to show widget in fullscreen mode or not. If we do, show it
	/// @param dockWidget QDockWidget to show
	/// @param name Widget's name in internal map
	void showDockWidget(QDockWidget *dockWidget, QString const &name);

	/// Find edges that connect items from itemsToDelete and should be deleted with them
	/// @param itemsToDelete selected items that should be deleted
	void addEdgesToBeDeleted(IdList &itemsToDelete);

	QString getNextDirName(QString const &name);

	void initMiniMap();
	void initToolManager();
	void initTabs();
	void initDocks();
	void initExplorers();
	void initRecentProjectsMenu();
	void openStartTab();
	void initScriptAPI();
	void initActionWidgetsNames();

	void setVersion(QString const &version);

	Ui::MainWindowUi *mUi;

	/// elements & theirs ids
	QMap<QString, Id> mElementsNamesAndIds;

	/// mFindDialog - Dialog for searching elements.
	FindReplaceDialog *mFindReplaceDialog;

	/// mCodeTabManager - Map that keeps pairs of opened tabs and their code areas.
	QMap<EditorView*, gui::QScintillaTextEdit*> *mCodeTabManager;

	models::Models *mModels;
	Controller *mController;
	ProxyEditorManager mEditorManagerProxy;
	ToolPluginManager mToolManager;
	ListenerManager *mListenerManager;
	PropertyEditorModel mPropertyModel;
	gestures::GesturesWidget *mGesturesWidget;
	SystemEvents *mSystemEvents;
	TextManager *mTextManager;
	QScopedPointer<Exploser> mExploser;

	QVector<bool> mSaveListChecked;

	QStringList mDiagramsList;
	QModelIndex mRootIndex;

	gui::ErrorReporter *mErrorReporter;  // Has ownership

	/// Fullscreen mode flag
	bool mIsFullscreen;

	/// Internal map table to store info what widgets should we hide/show
	QMap<QString, bool> mDocksVisibility;

	QString mTempDir;
	PreferencesDialog mPreferencesDialog;

	int mRecentProjectsLimit;
	QSignalMapper *mRecentProjectsMapper;
	QMenu *mRecentProjectsMenu;

	FindManager *mFindHelper;
	ProjectManager *mProjectManager;
	StartWidget *mStartWidget;

	FilterObject *mFilterObject; // Has ownership
	SceneCustomizer *mSceneCustomizer;
	QList<QDockWidget *> mAdditionalDocks;

	/// A field for storing file name passed as console argument
	QString mInitialFileToOpen;

	QToolBar *mUsabilityTestingToolbar; // Has ownership
	QAction *mStartTest; // Has ownership
	QAction *mFinishTest; // Has ownership

	gui::ScriptAPI mScriptAPI;
};

}

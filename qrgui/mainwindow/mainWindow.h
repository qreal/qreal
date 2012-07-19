#pragma once

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
#include "../view/propertyEditorView.h"
#include "../dialogs/gesturesShow/gesturesWidget.h"
#include "mainWindowInterpretersInterface.h"
#include "../../qrkernel/settingsManager.h"
#include "../../qrgui/dialogs/preferencesDialog.h"

#include "../textEditor/codeEditor.h"
#include "helpBrowser.h"

#include "../models/logicalModelAssistApi.h"

#include "../../qrgui/dialogs/findReplaceDialog.h"
#include "findManager.h"

#include  "paletteTree.h"

#include "../dialogs/startDialog/startDialog.h"
#include "projectManager/projectManager.h"

namespace Ui {
class MainWindowUi;
}

namespace qReal {

class EditorView;
class ListenerManager;

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

	EditorManager *manager();
	EditorView *getCurrentTab();
	ListenerManager *listenerManager();
	models::Models *models();
	PropertyEditorView *propertyEditor();
	QTreeView *graphicalModelExplorer();
	QTreeView *logicalModelExplorer();
	PropertyEditorModel &propertyModel();
	ToolPluginManager &toolManager();

	GesturesPainterInterface *gesturesPainter();
	QModelIndex rootIndex() const;

	QAction *actionDeleteFromDiagram() const;
	QAction *actionCopyElementsOnDiagram() const;
	QAction *actionPasteOnDiagram() const;
	QAction *actionPasteCopyOfLogical() const;

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

	virtual void saveDiagramAsAPictureToFile(const QString &fileName);
	virtual void arrangeElementsByDotRunner(const QString &algorithm, const QString &absolutePathToDotFiles);
	virtual IdList selectedElementsOnActiveDiagram();
	virtual void updateActiveDiagram();
	virtual void deleteElementFromDiagram(Id const &id);

signals:
	void gesturesShowed();
	void currentIdealGestureChanged();
	void rootDiagramChanged();

public slots:
	void deleteFromScene();
	void modelsAreChanged();
	void propertyEditorScrollTo(QModelIndex const &index);

	virtual void activateItemOrDiagram(Id const &id, bool bl = true, bool isSetSel = true);
	void activateItemOrDiagram(QModelIndex const &idx, bool bl = true, bool isSetSel = true);
	virtual void selectItem(Id const &id);
	virtual void selectItemOrDiagram(Id const &graphicalId);

	void selectItemWithError(Id const &id);
	void showErrors(gui::ErrorReporter const * const errorReporter);

	void changePaletteRepresentation();
	void closeAllTabs();
	void refreshRecentProjectsList(QString const &fileName);
	void connectWindowTitle();
	void disconnectWindowTitle();
	void createDiagram(QString const &idString);

private slots:

	/// handler for menu 'button find' pressed
	void showFindDialog();

	void setSceneFont();
	void adjustMinimapZoom(int zoom);
	void toggleShowSplash(bool show);

	void updateTabName(Id const &id);

	void settingsPlugins();

	void showAbout();
	void showHelp();

	/// checks parameters for integrity,then importing it
	/// @param fileName - *.qrs file to import
	/// @return true - if all ok, false - if not ok
	void fullscreen();
	void openRecentProjectsMenu();

	void saveDiagramAsAPicture();

	void print();
	void makeSvg();
	void showGrid(bool isChecked);

	void sceneSelectionChanged();

	void exportToXmi();
	void generateToJava();
	void parseJavaLibraries();
	void applySettings();

	void deleteFromScene(QGraphicsItem *target);

	void deleteFromDiagram();
	void copyElementsOnDiagram();
	void pasteOnDiagram();
	void pasteCopyOfLogical();

	void cropSceneToItems();

	void closeTab(int index);

	/// Closes the appropriate tab if the specified index corresponds to the diagram on one of the tabs
	/// @return true if one of the tabs was closed
	bool closeTab(QModelIndex const &graphicsIndex);

	void showPreferencesDialog();

	void initSettingsManager();
	void connectActions();
	void initActionsFromSettings();

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

	void updatePaletteIcons();

private:
	void deleteElementFromScene(QPersistentModelIndex const &index);
	/// Initializes a tab if it is a diagram --- sets its logical and graphical
	/// models, connects to various main window actions and so on
	/// @param tab Tab to be initialized
	/// @param rootIndex Index of a graphical model element that will be root of a diagram shown in this tab
	void initCurrentTab(EditorView * const tab, const QModelIndex &rootIndex);

	/// Sets shortcuts for a given tab which don`t have own buttons anywhere
	/// @param tab Tab to be initialized with shortcuts
	void setShortcuts(EditorView * const tab);


	void loadPlugins();

	void registerMetaTypes();

	QListWidget* createSaveListWidget();

	virtual void closeEvent(QCloseEvent *event);
	void deleteFromExplorer(bool isLogicalModel);
	void keyPressEvent(QKeyEvent *event);

	QString getSaveFileName(const QString &dialogWindowTitle);
	QString getOpenFileName(const QString &dialogWindowTitle);
	QString getWorkingFile(QString const &dialogWindowTitle, bool save);

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

	void initMiniMap();
	void initToolManager();
	void initTabs();
	void initDocks();
	void initExplorers();
	void initRecentProjectsMenu();

	Ui::MainWindowUi *mUi;

	/// elements & theirs ids
	QMap<QString, Id> mElementsNamesAndIds;

	/// mFindDialog - Dialog for searching elements.
	FindReplaceDialog *mFindReplaceDialog;

	/// mCodeTabManager - Map that keeps pairs of opened tabs and their code areas.
	QMap<EditorView*, CodeArea*> *mCodeTabManager;

	models::Models *mModels;
	EditorManager mEditorManager;
	ToolPluginManager mToolManager;
	ListenerManager *mListenerManager;
	PropertyEditorModel mPropertyModel;
	GesturesWidget *mGesturesWidget;

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

	HelpBrowser *mHelpBrowser;
	int mRecentProjectsLimit;
	QSignalMapper *mRecentProjectsMapper;
	QMenu *mRecentProjectsMenu;
	qReal::gui::PaletteTree *mPaletteTree;

	FindManager *mFindHelper;
	ProjectManager *mProjectManager;
	StartDialog *mStartDialog;
};

}

/* Copyright 2007-2015 QReal Research Group, Dmitry Mordvinov
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#pragma once

#include <QtWidgets/QMainWindow>

#include "scriptAPI/scriptAPI.h"

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/mainWindowInterpretersInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/mainWindowDockInterface.h>

#include <qrgui/preferencesDialog/preferencesDialog.h>

class QGraphicsView;
class QSignalMapper;
class QListWidget;
class QTreeView;

class PropertyEditorModel;
class FindManager;
class FindReplaceDialog;

namespace Ui {
class MainWindowUi;
}

namespace qReal {
class Controller;
class EditorManagerInterface;
class ProjectManagerWrapper;
class SplashScreen;
class StartWidget;
class SystemFacade;
class ToolPluginManager;
class ActionInfo;

namespace gui {
class ErrorReporter;
class PaletteTree;

namespace editor {
class SceneCustomizer;
class EditorView;
class PropertyEditorView;
}
}

namespace models {
class Models;
}

namespace text {
class TextManager;
}

class MainWindow : public QMainWindow
		, public qReal::gui::MainWindowInterpretersInterface
		, public qReal::gui::MainWindowDockInterface
{
	Q_OBJECT

public:
	explicit MainWindow(const QString &fileToOpen = QString());
	~MainWindow();

	EditorManagerInterface &editorManager();
	gui::editor::EditorView *getCurrentTab() const;
	bool isCurrentTabShapeEdit() const;
	models::Models &models();
	Controller *controller() const;
	gui::editor::PropertyEditorView *propertyEditor() const;
	QTreeView *graphicalModelExplorer() const;
	QTreeView *logicalModelExplorer() const;
	PropertyEditorModel &propertyModel();
	ToolPluginManager &toolManager();

	QModelIndex rootIndex() const;

	void highlight(const Id &graphicalId, bool exclusive = true, const QColor &color = Qt::red) override;
	void dehighlight(const Id &graphicalId) override;
	void dehighlight() override;
	ErrorReporterInterface *errorReporter() override;
	Id activeDiagram() const override;
	void openShapeEditor(const QPersistentModelIndex &index, int role, const QString &propertyValue
		, bool useTypedPorts);
	void openQscintillaTextEditor(const QPersistentModelIndex &index, const int role, const QString &propertyValue);
	void openShapeEditor(const Id &id
			, const QString &propertyValue
			/// @todo: whan passing it by reference the build on travis fails
			, const EditorManagerInterface *editorManager
			, bool useTypedPorts);
	void showAndEditPropertyInTextEditor(const QString &title, const QString &text, const QPersistentModelIndex &index
			, const int &role);
	void openReferenceList(const QPersistentModelIndex &index, const QString &referenceType
			, const QString &propertyValue, int role);

	void openSettingsDialog(const QString &tab) override;

	void showErrors(gui::ErrorReporter *reporter);

	/// Tells if we should display trace connections menu or not
	//virtual void showInTextEditor(const QFileInfo &fileInfo);
	void reinitModels() override;

	QWidget *windowWidget() override;

	bool unloadPlugin(const QString &pluginName) override;
	bool loadPlugin(const QString &fileName, const QString &pluginName) override;
	bool pluginLoaded(const QString &pluginName) override;

	void saveDiagramAsAPictureToFile(const QString &fileName) override;
	void arrangeElementsByDotRunner(const QString &algorithm, const QString &absolutePathToDotFiles) override;
	IdList selectedElementsOnActiveDiagram() override;
	void updateActiveDiagram() override;
	void deleteElementFromDiagram(const Id &id) override;

	void reportOperation(const QFuture<void> &operation, const QString &description = QString()) override;
	QWidget *currentTab() override;
	void openTab(QWidget *tab, const QString &title) override;
	void closeTab(QWidget *tab) override;

	QMap<QString, gui::PreferencesPage *> preferencesPages() const override;

	/// Closes tab having given id as root id. If there is no such tab, does nothing.
	/// @param id Id of a diagram (root element) that we want to close.
	void closeDiagramTab(const Id &id);

	/// Loads (or reloads) available editor plugins and reinits palette.
	void loadEditorPlugins();

	/// Clears selection on all opened tabs.
	void clearSelectionOnTabs();

	QDockWidget *logicalModelDock() const override;
	QDockWidget *graphicalModelDock() const override;
	QDockWidget *propertyEditorDock() const override;
	QDockWidget *errorReporterDock() const override;
	QDockWidget *paletteDock() const override;
	QStatusBar *statusBar() const override;
	QList<QToolBar *> toolBars() const override;

	void tabifyDockWidget(QDockWidget *first, QDockWidget *second) override;
	void addDockWidget(Qt::DockWidgetArea area, QDockWidget *dockWidget) override;
	void addToolBar(Qt::ToolBarArea area, QToolBar * const toolbar) override;

	QByteArray saveState(int version = 0) const override;
	bool restoreState(const QByteArray &state, int version = 0) override;

	void setCorner(Qt::Corner corner, Qt::DockWidgetArea area) override;

	void setTabText(QWidget *tab, const QString &text) override;

	void beginPaletteModification() override;
	void setElementInPaletteVisible(const Id &metatype, bool visible) override;
	void setVisibleForAllElementsInPalette(const Id &diagram, bool visible) override;
	void setElementInPaletteEnabled(const Id &metatype, bool enabled) override;
	void setEnabledForAllElementsInPalette(const Id &diagram, bool enabled) override;
	void endPaletteModification() override;

	/// Additional actions for interpreter palette.
	QList<QAction *> optionalMenuActionsForInterpretedPlugins();

signals:
	void rootDiagramChanged();

public slots:
	void propertyEditorScrollTo(const QModelIndex &index);

	void activateItemOrDiagram(const Id &id, bool setSelected = true) override;
	void activateItemOrDiagram(const QModelIndex &idx, bool setSelected = true);
	void selectItem(const Id &id) override;
	void selectItemOrDiagram(const Id &graphicalId) override;

	void selectItemWithError(const Id &id);
	void showErrors(const gui::ErrorReporter * const errorReporter);

	void changePaletteRepresentation();
	void closeStartTab();
	void closeAllTabs();
	void refreshRecentProjectsList(const QString &fileName);
	void createDiagram(const QString &idString);
	/// Creates project with specified root diagram
	bool createProject(const QString &diagramIdString);

	void openFirstDiagram();
	void changeWindowTitle();

	void registerEditor(EditorInterface &editor) override;

private slots:
	/// Suggests user to select a root diagram for the new project
	/// if more than one diagram loaded or creates project with the only diagram
	/// as root otherwise
	void createProject();

	/// Diagram opening must happen after plugins initialization
	void initPluginsAndStartWidget();
	void initToolPlugins();
	void customizeActionsVisibility();

	/// handler for menu 'button find' pressed
	void showFindDialog();

	void adjustMinimapZoom(int zoom);
	void toggleShowSplash(bool show);

	void updateTabName(const Id &id);

	void showAbout();
	void showHelp();

	void fullscreen();
	void hideBottomDocks();

	void openRecentProjectsMenu();

	void saveDiagramAsAPicture();
	void print();
	void makeSvg();
	void showGrid(bool isChecked);

	void sceneSelectionChanged();

	void applySettings();
	void resetToolbarSize(int size);

	void closeCurrentTab();
	void closeTab(int index);

	/// Closes the appropriate tab if the specified index corresponds to the diagram on one of the tabs
	/// @return true if one of the tabs was closed
	bool closeTab(const QModelIndex &graphicsIndex);

	void showPreferencesDialog();

	void connectActions();
	void connectSystemEvents();
	void initActionsFromSettings();

	void centerOn(const Id &id);
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

	void setData(const QString &data, const QPersistentModelIndex &index, const int &role);
	void setReference(const QStringList &data, const QPersistentModelIndex &index, const int &role);
	void openShapeEditor();

	void updatePaletteIcons();
	void setTextChanged(bool changed);

private:
	/// Initializes a tab if it is a diagram --- sets its logical and graphical
	/// models, connects to various main window actions and so on
	/// @param tab Tab to be initialized
	/// @param rootIndex Index of a graphical model element that will be root of a diagram shown in this tab
	void initCurrentTab(qReal::gui::editor::EditorView * const tab, const QModelIndex &rootIndex);

	/// Sets shortcuts for a given tab which don`t have own buttons anywhere
	/// @param tab Tab to be initialized with shortcuts
	void setShortcuts(qReal::gui::editor::EditorView * const tab);

	void setDefaultShortcuts();

	void registerMetaTypes();

	QListWidget* createSaveListWidget();

	void closeEvent(QCloseEvent *event) override;
	void deleteFromLogicalExplorer();
	void deleteFromGraphicalExplorer();

	QString getSaveFileName(const QString &dialogWindowTitle);
	QString getOpenFileName(const QString &dialogWindowTitle);
	QString getWorkingFile(const QString &dialogWindowTitle, bool save);

	void selectItemInLogicalModel(const Id &id);
	void switchToTab(int index);
	int getTabIndex(const QModelIndex &index);

	void initGridProperties();
	void clickErrorListWidget();

	void setShowGrid(bool isChecked);
	void setShowAlignment(bool isChecked);
	void setSwitchGrid(bool isChecked);
	void setSwitchAlignment(bool isChecked);

	void addActionOrSubmenu(QMenu *target, const ActionInfo &actionOrMenu);

	/// Traverses list of actions and adds buttons to toolbar.
	/// @param actions - list of actions to traverse
	void traverseListOfActions(const QList<ActionInfo> &actions);
	void addExternalToolActions();

	void setIndexesOfPropertyEditor(const Id &id);

	void setBackReference(const QPersistentModelIndex &index, const QString &data);
	void removeOldBackReference(const QPersistentModelIndex &index, const int role);

	void removeReferences(const Id &id);

	/// Check if we need to hide widget in fullscreen mode or not. If we do, hide it
	/// @param dockWidget QDockWidget to hide
	/// @param name Widget's name in internal map
	void hideDockWidget(QDockWidget *dockWidget, const QString &name);

	/// Check if we need to show widget in fullscreen mode or not. If we do, show it
	/// @param dockWidget QDockWidget to show
	/// @param name Widget's name in internal map
	void showDockWidget(QDockWidget *dockWidget, const QString &name);

	/// Find edges that connect items from itemsToDelete and should be deleted with them
	/// @param itemsToDelete selected items that should be deleted
	void addEdgesToBeDeleted(IdList &itemsToDelete);

	QString getNextDirName(const QString &name);

	void initMiniMap();
	void initToolManager();
	void initTabs();
	void initDocks();
	void initExplorers();
	void initRecentProjectsMenu();
	void initScriptAPI();
	void initActionWidgetsNames();
	void openStartTab() override;

	void setVersion(const QString &version);

	void highlightCode(Id const &graphicalId, bool highlight);

	Ui::MainWindowUi *mUi;
	QScopedPointer<SystemFacade> mFacade;

	QScopedPointer<SplashScreen> mSplashScreen;

	/// elements & theirs ids
	QMap<QString, Id> mElementsNamesAndIds;

	/// mFindDialog - Dialog for searching elements.
	FindReplaceDialog *mFindReplaceDialog;

	Controller *mController;
	QScopedPointer<ToolPluginManager> mToolManager;
	QScopedPointer<PropertyEditorModel> mPropertyModel;
	text::TextManager *mTextManager;
	EditorInterface *mCurrentEditor;

	QVector<bool> mSaveListChecked;

	QStringList mDiagramsList;
	QModelIndex mRootIndex;

	gui::ErrorReporter *mErrorReporter;  // Has ownership

	/// Fullscreen mode flag
	bool mIsFullscreen;

	/// Internal map table to store info what widgets should we hide/show
	QMap<QString, bool> mDocksVisibility;

	QString mTempDir;
	qReal::gui::PreferencesDialog mPreferencesDialog;

	int mRecentProjectsLimit;
	QSignalMapper *mRecentProjectsMapper;
	QMenu *mRecentProjectsMenu;

	FindManager *mFindHelper;
	ProjectManagerWrapper *mProjectManager;
	StartWidget *mStartWidget;

	qReal::gui::editor::SceneCustomizer *mSceneCustomizer;
	QList<QDockWidget *> mAdditionalDocks;
	QMap<QWidget *, int> mLastTabBarIndexes;

	QList<QAction *> mListOfAdditionalActions; // doesn't have ownership

	/// A field for storing file name passed as console argument
	QString mInitialFileToOpen;

	gui::ScriptAPI mScriptAPI;
};

}

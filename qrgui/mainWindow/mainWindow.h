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

#include <QtCore/QSignalMapper>
#include <QtCore/QDir>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSplashScreen>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QListWidget>
#include <QtSql/QSqlDatabase>

#include <qrkernel/settingsManager.h>

#include "findManager.h"
#include "referenceList.h"
#include "projectManager/projectManagerWrapper.h"
#include "tabWidget.h"
#include "startWidget/startWidget.h"

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/mainWindowInterpretersInterface.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/mainWindowDockInterface.h>
#include <qrgui/plugins/pluginManager/editorManagerInterface.h>
#include <qrgui/plugins/pluginManager/editorManager.h>
#include <qrgui/plugins/pluginManager/interpreterEditorManager.h>
#include <qrgui/plugins/pluginManager/proxyEditorManager.h>
#include <qrgui/plugins/pluginManager/toolPluginManager.h>
#include <qrgui/plugins/pluginManager/interpretedPluginsLoader.h>

#include <qrgui/systemFacade/systemFacade.h>
#include <qrgui/editor/propertyEditorView.h>
#include <qrgui/models/propertyEditorModel.h>
#include <qrgui/controller/controller.h>

#include <qrgui/preferencesDialog/preferencesDialog.h>
#include <qrgui/dialogs/findReplaceDialog.h>

class QGraphicsView;

namespace Ui {
class MainWindowUi;
}

namespace qReal {

class EditorView;
class SceneCustomizer;

namespace models {
class Models;
}

namespace gui {
class ErrorReporter;
class PaletteTree;
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
	MainWindow(const QString &fileToOpen = QString());
	~MainWindow();

	EditorManagerInterface &editorManager();
	EditorView *getCurrentTab() const;
	bool isCurrentTabShapeEdit() const;
	models::Models &models();
	Controller *controller() const;
	PropertyEditorView *propertyEditor() const;
	QTreeView *graphicalModelExplorer() const;
	QTreeView *logicalModelExplorer() const;
	PropertyEditorModel &propertyModel();
	ToolPluginManager &toolManager();

	QModelIndex rootIndex() const;

	virtual void highlight(const Id &graphicalId, bool exclusive = true, const QColor &color = Qt::red);
	virtual void dehighlight(const Id &graphicalId);
	virtual void dehighlight();
	virtual ErrorReporterInterface *errorReporter();
	virtual Id activeDiagram() const;
	void openShapeEditor(const QPersistentModelIndex &index, int role, const QString &propertyValue
		, bool useTypedPorts);
	void openQscintillaTextEditor(const QPersistentModelIndex &index, const int role, const QString &propertyValue);
	void openShapeEditor(const Id &id
			, const QString &propertyValue
			/// @todo: whan passing it by reference the build on travis fails
			, const EditorManagerInterface *editorManagerProxy
			, bool useTypedPorts);
	void showAndEditPropertyInTextEditor(const QString &title, const QString &text, const QPersistentModelIndex &index
			, const int &role);
	void openReferenceList(const QPersistentModelIndex &index, const QString &referenceType
			, const QString &propertyValue, int role);

	virtual void openSettingsDialog(const QString &tab);

	void showErrors(gui::ErrorReporter *reporter);

	/// Tells if we should display trace connections menu or not
	//virtual void showInTextEditor(const QFileInfo &fileInfo);
	virtual void reinitModels();

	virtual QWidget *windowWidget();

	virtual bool unloadPlugin(const QString &pluginName);
	virtual bool loadPlugin(const QString &fileName, const QString &pluginName);
	virtual bool pluginLoaded(const QString &pluginName);

	virtual void saveDiagramAsAPictureToFile(const QString &fileName);
	virtual void arrangeElementsByDotRunner(const QString &algorithm, const QString &absolutePathToDotFiles);
	virtual IdList selectedElementsOnActiveDiagram();
	virtual void updateActiveDiagram();
	virtual void deleteElementFromDiagram(const Id &id);

	virtual void reportOperation(invocation::LongOperation *operation);
	virtual QWidget *currentTab();
	virtual void openTab(QWidget *tab, const QString &title);
	virtual void closeTab(QWidget *tab);

	QMap<QString, gui::PreferencesPage *> preferencesPages() const override;

	/// Closes tab having given id as root id. If there is no such tab, does nothing.
	/// @param id Id of a diagram (root element) that we want to close.
	void closeDiagramTab(const Id &id);

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

	QDockWidget *logicalModelDock() const override;
	QDockWidget *graphicalModelDock() const override;
	QDockWidget *propertyEditorDock() const override;
	QDockWidget *errorReporterDock() const override;
	QDockWidget *paletteDock() const override;
	QStatusBar *statusBar() const override;
	QList<QToolBar *> toolBars() const override;

	void tabifyDockWidget(QDockWidget *first, QDockWidget *second) override;
	void addDockWidget(Qt::DockWidgetArea area, QDockWidget *dockWidget) override;

	QByteArray saveState(int version = 0) const override;
	bool restoreState(const QByteArray &state, int version = 0) override;

	void setCorner(Qt::Corner corner, Qt::DockWidgetArea area) override;

	void setTabText(QWidget *tab, const QString &text) override;

	void beginPaletteModification() override;
	void setElementInPaletteVisible(const Id &metatype, bool visible) override;
	void setVisibleForAllElementsInPalette(bool visible) override;
	void setElementInPaletteEnabled(const Id &metatype, bool enabled) override;
	void setEnabledForAllElementsInPalette(bool enabled) override;
	void endPaletteModification() override;

	/// Additional actions for interpreter palette.
	QList<QAction *> optionalMenuActionsForInterpretedPlugins();

signals:
	void rootDiagramChanged();

public slots:
	void propertyEditorScrollTo(const QModelIndex &index);

	virtual void activateItemOrDiagram(const Id &id, bool setSelected = true);
	void activateItemOrDiagram(const QModelIndex &idx, bool setSelected = true);
	virtual void selectItem(const Id &id);
	virtual void selectItemOrDiagram(const Id &graphicalId);

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

	/// Inits interpreted plugins and adds their actions to the toolbar.
	void initInterpretedPlugins();

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

	void sceneSelectionChanged(const QList<Element *> &elements);

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
	void initCurrentTab(EditorView * const tab, const QModelIndex &rootIndex);

	/// Sets shortcuts for a given tab which don`t have own buttons anywhere
	/// @param tab Tab to be initialized with shortcuts
	void setShortcuts(EditorView * const tab);

	void setDefaultShortcuts();

	void registerMetaTypes();

	QListWidget* createSaveListWidget();

	virtual void closeEvent(QCloseEvent *event);
	void deleteFromLogicalExplorer();
	void deleteFromGraphicalExplorer();

	QString getSaveFileName(const QString &dialogWindowTitle);
	QString getOpenFileName(const QString &dialogWindowTitle);
	QString getWorkingFile(const QString &dialogWindowTitle, bool save);

	void selectItemInLogicalModel(const Id &id);
	void switchToTab(int index);
	int getTabIndex(const QModelIndex &index);

	void initGridProperties();
	void disconnectZoom(QGraphicsView *view);
	void connectZoom(QGraphicsView *view);
	void disconnectActionZoomTo(QWidget* widget);
	void connectActionZoomTo(QWidget* widget);
	void setConnectActionZoomTo(QWidget* widget);
	void clickErrorListWidget();

	void setShowGrid(bool isChecked);
	void setShowAlignment(bool isChecked);
	void setSwitchGrid(bool isChecked);
	void setSwitchAlignment(bool isChecked);

	void addActionOrSubmenu(QMenu *target, const ActionInfo &actionOrMenu);

	/// Traverses list of actions and adds buttons to toolbar.
	/// @param actions - list of actions to traverse
	void traverseListOfActions(QList<ActionInfo> const &actions);

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
	void openStartTab();

	void setVersion(const QString &version);

	Ui::MainWindowUi *mUi;
	SystemFacade mFacade;

	/// elements & theirs ids
	QMap<QString, Id> mElementsNamesAndIds;

	/// mFindDialog - Dialog for searching elements.
	FindReplaceDialog *mFindReplaceDialog;

	Controller *mController;
	ToolPluginManager mToolManager;
	InterpretedPluginsLoader mInterpretedPluginLoader;
	PropertyEditorModel mPropertyModel;
	text::TextManager *mTextManager;

	QVector<bool> mSaveListChecked;

	QStringList mDiagramsList;
	QModelIndex mRootIndex;

	gui::ErrorReporter *mErrorReporter;  // Has ownership

	/// Fullscreen mode flag
	bool mIsFullscreen;

	/// Internal map table to store info what widgets should we hide/show
	QMap<QString, bool> mDocksVisibility;

	QString mTempDir;
	gui::PreferencesDialog mPreferencesDialog;

	int mRecentProjectsLimit;
	QSignalMapper *mRecentProjectsMapper;
	QMenu *mRecentProjectsMenu;

	FindManager *mFindHelper;
	ProjectManagerWrapper *mProjectManager;
	StartWidget *mStartWidget;

	SceneCustomizer *mSceneCustomizer;
	QList<QDockWidget *> mAdditionalDocks;
	QMap<QWidget *, int> mLastTabBarIndexes;

	QList<QAction *> mListOfAdditionalActions; // doesn't have ownership

	/// A field for storing file name passed as console argument
	QString mInitialFileToOpen;
};

}

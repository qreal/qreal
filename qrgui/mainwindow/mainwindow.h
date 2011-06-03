#pragma once

#include <QtGui/QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QtCore/QDir>
#include <QSplashScreen>
#include <QtGui>

#include "../editorManager/editorManager.h"
#include "propertyeditorproxymodel.h"
#include "propertyeditordelegate.h"
#include "igesturespainter.h"
#include "gesturesShow/gestureswidget.h"
#include "mainWindowInterpretersInterface.h"

#include "../interpreters/robots/bluetoothRobotCommunication.h"
#include "../interpreters/robots/details/d2RobotModel/d2RobotModel.h"

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

namespace interpreters {
namespace robots {
class Interpreter;
}
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
	IGesturesPainter *gesturesPainter();
	QModelIndex rootIndex() const;

	QAction *actionDeleteFromDiagram() const;

	virtual void highlight(Id const &graphicalId, bool exclusive = true);
	virtual void dehighlight(Id const &graphicalId);
	virtual gui::ErrorReporter *errorReporter();

signals:
	void gesturesShowed();
	void currentIdealGestureChanged();
	void rootDiagramChanged();

public slots:
	void deleteFromScene();

	void propertyEditorScrollTo(QModelIndex const &index);

	void activateItemOrDiagram(Id const &id, bool bl = true, bool isSetSel = true);
	void activateItemOrDiagram(QModelIndex const &idx, bool bl = true, bool isSetSel = true);
	virtual void selectItem(Id const &id);

private slots:
	void adjustMinimapZoom(int zoom);
	void toggleShowSplash(bool show);

	void updateTabName(Id const &id);

	void settingsPlugins();

	void showAbout();
	void showHelp();

	bool open();
	bool checkPluginsAndReopen();
	void saveAs();
	void saveAll();

	void print();
	void makeSvg();
	void showGrid(bool isChecked);

	void finalClose();
	void closeAllTabs();

	void sceneSelectionChanged();

	void deleteFromScene(QGraphicsItem *target);

	void activateSubdiagram(QModelIndex const &idx);

	void deleteFromDiagram();
	void changeMiniMapSource(int index);
	void closeTab(int index);
	void closeTab(QModelIndex const &index);
	void exterminate();
	void generateEditor();
	void parseEditorXml();
	void showPreferencesDialog();

	void connectActions();

	void centerOn(Id const &id);
	void graphicalModelExplorerClicked(const QModelIndex &index);
	void logicalModelExplorerClicked(const QModelIndex &index);

	void openShapeEditor();
	void openNewTab(const QModelIndex &index);
	void initCurrentTab(const QModelIndex &rootIndex);

	void showGestures();
	void showAlignment(bool isChecked);
	void switchGrid(bool isChecked);
	void switchAlignment(bool isChecked);
	void setShape(QString const &data, QPersistentModelIndex const &index, int const &role);

	void setDiagramCreateFlag();
	void diagramInCreateListDeselect();
	void diagramInCreateListSelected(int num);

	void run();
	void stop();
	void stopRobot();
	void showRobotSettingsDialog();

	void on_actionNew_Diagram_triggered();

	void updatePaletteIcons();

private:
	Ui::MainWindowUi *mUi;

	QCloseEvent *mCloseEvent;
	models::Models *mModels;
	EditorManager mEditorManager;
	ListenerManager *mListenerManager;
	PropertyEditorModel mPropertyModel;
	PropertyEditorDelegate mDelegate;
	GesturesWidget *mGesturesWidget;

	QVector<bool> mSaveListChecked;
	bool mDiagramCreateFlag;

	QStringList mDiagramsList;
	QModelIndex mRootIndex;

	VisualDebugger *mVisualDebugger;
	interpreters::robots::Interpreter *mRobotInterpreter;  // Has ownership
	interpreters::robots::BluetoothRobotCommunication *mBluetoothCommunication;  // Does not have ownership
	interpreters::robots::details::d2Model::D2RobotModel *mD2Model;// Does not have ownership
	gui::ErrorReporter *mErrorReporter;  // Has ownership

	void createDiagram(const QString &idString);
	void loadNewEditor(QString const &directoryName, QString const &metamodelName,
			QString const &commandFirst, QString const &commandSecond, QString const &extension, QString const &prefix);

	void loadPlugins();

	QListWidget* createSaveListWidget();
	void suggestToSave();
	void suggestToCreateDiagram();

	virtual void closeEvent(QCloseEvent *event);
	void deleteFromExplorer(bool isLogicalModel);
	void keyPressEvent(QKeyEvent *event);
	QString getWorkingDir(QString const &dialogWindowTitle);

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
};
}

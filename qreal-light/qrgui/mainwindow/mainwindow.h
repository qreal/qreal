#pragma once

#include <QtGui/QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QtCore/QDir>
#include <QSplashScreen>
#include <QtGui>

#include "ui_mainwindow.h"

#include "../editorManager/editorManager.h"
#include "propertyeditorproxymodel.h"
#include "propertyeditordelegate.h"

namespace qReal {

	class EditorView;
	class ListenerManager;

	namespace model {
		class Model;
	}

	class MainWindow : public QMainWindow
	{
		Q_OBJECT

	public:
		MainWindow();
		~MainWindow();

		EditorManager* manager();
		EditorView *getCurrentTab();
		ListenerManager *listenerManager();
		Ui::MainWindowUi ui;

	signals:
		void gesturesShowed();
		void currentIdealGestureChanged();
		void rootDiagramChanged();

	public slots:
		void adjustMinimapZoom(int zoom);
		void toggleShowSplash(bool show);

		void updateTab(QModelIndex const &index);

		void settingsPlugins();

		void showAbout();
		void showHelp();

		void open();
		void save();
		void saveAs();
		void saveAll();
		void saveIds(QList<Id> const &toSave, QList<Id> const & toRemove);

		void print();
		void makeSvg();
		void showGrid(bool show);

		void finalClose();

		void sceneSelectionChanged();

		void deleteFromScene();
		void deleteFromScene(QGraphicsItem *target);

		void activateSubdiagram(QModelIndex const &idx);
		void activateItemOrDiagram(Id const &id, bool bl = true, bool isSetSel = true);
		void activateItemOrDiagram(QModelIndex const &idx, bool bl = true, bool isSetSel = true);
		void propertyEditorScrollTo(QModelIndex const &index);

	private slots:
		void deleteFromDiagram();
		void changeMiniMapSource(int index);
		void closeTab(int index);
		void closeTab(QModelIndex const &index);
		void exterminate();
		void showPreferencesDialog();

		void centerOn(const QModelIndex &rootIndex);
		void diagramExplorerClicked(const QModelIndex &rootIndex);

		void openNewTab(const QModelIndex &index);
		void initCurrentTab(const QModelIndex &rootIndex);

		void switchGrid(bool isChecked);
		void switchAlignment(bool isChecked);

		void saveListClosed();

		void setDiagramCreateFlag();
		void diagramInCreateListDeselect();
		void diagramInCreateListSelected(int num);

		void diagramInSaveListChanged(QListWidgetItem* diagram);

	private:
		void createDiagram(const QString &idString);

		void loadPlugins();

		QListWidget* createSaveListWidget();
		void suggestToSave();
		void suggestToCreateDiagram();

		virtual void closeEvent(QCloseEvent *event);
		void deleteFromExplorer();
		void keyPressEvent(QKeyEvent *event);
		QString getWorkingDir(QString const &dialogWindowTitle);

		int getTabIndex(const QModelIndex &index);

		void initGridProperties();
		void disconnectZoom(QGraphicsView* view);
		void connectZoom(QGraphicsView* view);
		void disconnectActionZoomTo(QWidget* widget);
		void connectActionZoomTo(QWidget* widget);
		void setConnectActionZoomTo(QWidget* widget);

		QCloseEvent *mCloseEvent;
		model::Model *mModel;
		EditorManager mEditorManager;
		ListenerManager *mListenerManager;
		PropertyEditorModel mPropertyModel;
		PropertyEditorDelegate mDelegate;

		bool *mSaveListChecked;  // TODO: It's actually dynamically allocated plain C array. Change this to QVector.
		bool mDiagramCreateFlag;

		QStringList mDiagramsList;

	};
}

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
#include "igesturespainter.h"
#include "gesturesShow/gestureswidget.h"

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
		IGesturesPainter *gesturesPainter();
		Ui::MainWindowUi ui;

	public slots:
		void adjustMinimapZoom(int zoom);
		void toggleShowSplash(bool show);

		void updateTab(QModelIndex const &index);

		void settingsPlugins();

		void showAbout();
		void showHelp();

		void checkoutDialogOk();
		void checkoutDialogCancel();
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

		void doCheckout();
		void doCommit();
		void exportToXmi();
		void generateToJava();
		void parseJavaLibraries();
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
		void generateEditor();
		void generateEditorWithQRMC();
		void parseEditorXml();
		void generateToHascol();
		void parseHascol();
		void showPreferencesDialog();

		void centerOn(const QModelIndex &rootIndex);
		void diagramExplorerClicked(const QModelIndex &rootIndex);

		void openNewEmptyTab();
		void openNewTab(const QModelIndex &index);
		void initCurrentTab(const QModelIndex &rootIndex);

		void showGestures();
		void switchGrid(bool isChecked);
		void setShape( QString const &data, QPersistentModelIndex const &index, int const &role);

		void saveListClosed();

		void setDiagramCreateFlag();
		void diagramInCreateListDeselect();
		void diagramInCreateListSelected(int num);

		void diagramInSaveListChanged(QListWidgetItem* diagram);

	private:
		QCloseEvent *mCloseEvent;
		model::Model *mModel;
		EditorManager mEditorManager;
		ListenerManager *mListenerManager;
		PropertyEditorModel mPropertyModel;
		PropertyEditorDelegate mDelegate;
		GesturesWidget *mGesturesWidget;

		bool *mSaveListChecked;  // TODO: It's actually dynamically allocated plain C array. Change this to QVector.
		bool mDiagramCreateFlag;

		QStringList mDiagramsList;
		void createDiagram(const QString &idString);
		void loadNewEditor(QString const &directoryName, QString const &metamodelName,
				QString const &commandFirst, QString const &commandSecond, QString const &extension, QString const &prefix);

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

	signals:
		void gesturesShowed();
		void currentIdealGestureChanged();
		void rootDiagramChanged();
	};
}

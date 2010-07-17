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

		EditorManager *manager() {
			return &mEditorManager;
		}

		ListenerManager *listenerManager();

				IGesturesPainter *gesturesPainter();

		Ui::MainWindowUi ui;

		void suggestToSave();
	public slots:
		//		void connectRepo();
		//		void closeRepo();

		void adjustMinimapZoom(int zoom);
		void toggleShowSplash(bool show);

		void updateTab(QModelIndex const &index);

		void settingsPlugins();

		void showAbout();
		void showHelp();

		void open();
		void save();
		void saveAs();
		void print();
		void makeSvg();
		void showGrid();

		void finalClose();

		void sceneSelectionChanged();

		void doCommit();
		void exportToXmi();
		void generateToJava();
		void parseJavaLibraries();
		void deleteFromScene();
		void deleteFromScene(QGraphicsItem *target);

		void activateSubdiagram(QModelIndex const &idx);

		void activateItemOrDiagram(Id const &id);

		EditorView *getCurrentTab();
	private slots:
		void activateItemOrDiagram(QModelIndex const &idx);
		void deleteFromDiagram();
		void changeMiniMapSource(int index);
		void closeTab(int index);
		void closeTab(QModelIndex const &index);
		void exterminate();
		void newGenerateEditor();
		void parseEditorXml();
		void generateEditor();
		void generateToHascol();
		void parseHascol();
		void showPreferencesDialog();
		void initCurrentTab(const QModelIndex &rootIndex);
		void openNewTab(const QModelIndex &index);
		void openNewEmptyTab();
		void switchGrid(bool isChecked);
		void showGestures();
		void setShape( QString const &data, QPersistentModelIndex const &index, int const &role);

		void diagramInListDeselect();
		void diagramInListSelected(int num);
	private:
		QCloseEvent* clEvent;
		model::Model *mModel;
		EditorManager mEditorManager;
		ListenerManager *mListenerManager;

		PropertyEditorModel mPropertyModel;

		PropertyEditorDelegate mDelegate;

		GesturesWidget * mGesturesWidget;

		void loadPlugins();
		void chooseDiagram();
		QStringList diagramsList;
		void createDiagram(const QString &idString);

		virtual void closeEvent(QCloseEvent *event);
		void deleteFromExplorer();
		void keyPressEvent(QKeyEvent *event);
		QString getWorkingDir(QString const &dialogWindowTitle);

			signals:
				void gesturesShowed();
				void currentIdealGestureChanged();
				void tabOpened();
			};

}

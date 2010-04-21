#pragma once

#include <QtGui/QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QtCore/QDir>
#include <QSplashScreen>

#include "ui_mainwindow.h"

#include "editormanager.h"
#include "propertyeditorproxymodel.h"
#include "propertyeditordelegate.h"

namespace qReal {

	class EditorView;

	namespace model  {
		class Model;
	}

	class MainWindow : public QMainWindow
	{
		Q_OBJECT

	public:
		MainWindow();
		~MainWindow();

		EditorManager *manager() {
			return &mgr;
		}

		/** @brief Интерфейс главного окна */
		Ui::MainWindowUi ui;
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

		/** @brief Реагирует на изменение выделения на сцене, синхронизируя его с диаграм
		 * эксплорером. */
		void sceneSelectionChanged();

		void exportToXmi();
		void generateToJava();
		void deleteFromScene();
		void deleteFromScene(QGraphicsItem *target);

		/** @brief Активирует ближайшую к текущей поддиаграмму, содержащую указанный
		 * элемент, если в ней есть элементы. */
		void activateSubdiagram(QModelIndex const &idx);

		/** @brief Ищет в модели нужный элемент и открывает соответствующую ему
		 * диаграмму. */
		void activateItemOrDiagram(Id const &id);

		EditorView *getCurrentTab();
	private slots:
		void activateItemOrDiagram(QModelIndex const &idx);
		void deleteFromDiagram();
		void changeMiniMapSource(int index);
		void closeTab(int index);
		void closeTab(QModelIndex const &index);
		void exterminate();
		void generateEditor();
		void generateToHascol();
		void parseHascol();
		void initCurrentTab(const QModelIndex &rootIndex);
		void openNewTab(const QModelIndex &index);

	private:
		model::Model *mModel;
		EditorManager mgr;

		/** @brief Модель редактора свойств */
		PropertyEditorModel mPropertyModel;

		/** @brief Делегат */
		PropertyEditorDelegate mDelegate;

		void loadPlugins();
		virtual void closeEvent(QCloseEvent *event);
		void deleteFromExplorer();
		void keyPressEvent(QKeyEvent *event);
		QString getWorkingDir(QString const &dialogWindowTitle);
	};

}

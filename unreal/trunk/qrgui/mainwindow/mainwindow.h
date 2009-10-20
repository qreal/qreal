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

		void settingsPlugins();

		void showAbout();
		void showHelp();

		void save();
		void print();
		void makeSvg();

		/** @brief Реагирует на изменение выделения на сцене, синхронизируя его с диаграм
		 * эксплорером. */
		void sceneSelectionChanged();

		void exportToXmi();
		void generateToJava();
		void deleteFromScene();

		/** @brief Активирует ближайшую к текущей поддиаграмму, содержащую указанный
		 * элемент, если в ней есть элементы. */
		void activateSubdiagram(QModelIndex const &idx);

		EditorView *getCurrentTab();
	private slots:
		void activateItemOrDiagram(QModelIndex const &idx);
		void deleteFromDiagram();
		void openNewTab();
		void changeMiniMapSource(int index);
		void closeTab(int index);
		void exterminate();
		void generateEditor();
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
		void initCurrentTab(const QModelIndex &rootIndex);
	};

}

#pragma once

#include <QtGui/QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QtCore/QDir>

#include "ui_mainwindow.h"

#include "editormanager.h"
#include "propertyeditorproxymodel.h"
#include "propertyeditordelegate.h"

class EditorView;

namespace qReal {

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

		void settingsPlugins();

		void showAbout();
		void showHelp();

		void print();
		void makeSvg();

		/** @brief Реагирует на изменение выделения на сцене, синхронизируя его с диаграм
		 * эксплорером. */
		void sceneSelectionChanged();
	private:
		model::Model *mModel;
		/** @brief Модель редактора свойств */
		PropertyEditorModel mPropertyModel;

		/** @brief Делегат */
		PropertyEditorDelegate mDelegate;
		EditorManager mgr;

		void loadPlugins();
		virtual void closeEvent(QCloseEvent *event);
	};

}

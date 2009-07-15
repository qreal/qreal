#pragma once

#include <QtGui/QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QtCore/QDir>

#include "ui_mainwindow.h"

#include "editormanager.h"

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
		Ui::MainWindowUi ui;

		EditorManager *manager() {
			return &mgr;
		};

	public slots:
		//		void connectRepo();
		//		void closeRepo();
		void adjustMinimapZoom(int zoom);

		void settingsPlugins();

		void showAbout();
		void showHelp();

		void print();
		void makeSvg();

	private:
		model::Model *mModel;

		void loadPlugins();
		EditorManager mgr;
	};

}

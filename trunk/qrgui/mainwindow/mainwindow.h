#pragma once

#include <QtGui/QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QtCore/QDir>

#include "ui_mainwindow.h"

#include "editormanager.h"

namespace qReal {

	class RealRepoModel;
	class EditorView;

	class MainWindow : public QMainWindow
	{
		Q_OBJECT

	public:
		MainWindow();
		~MainWindow();

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
		Ui::MainWindowUi ui;

		void loadPlugins();
		EditorManager mgr;
	};

}

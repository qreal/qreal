#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtSql/QSqlDatabase>

#include "ui_mainwindow.h"

class RealRepoModel;
class EditorView;

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
	MainWindow();
	~MainWindow();

public slots:
/*	void connectRepo();
	void closeRepo();			*/
	void adjustMinimapZoom(int zoom);

/*	void beginTransaction();
	void commitTransaction();
	void rollbackTransaction();
	void deleteFromDiagram();		*/

	void showAbout();
	void showHelp();

	void print();
	void makeSvg();

private:
	
	Ui::MainWindowUi ui;
};

#endif

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtSql/QSqlDatabase>

#include "ui_mainwindow.h"

class RealRepoModel;
class QSortFilterProxyModel;
class EditorView;

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
	MainWindow();
	~MainWindow();

public slots:
	void connectRepo();
	void adjustMinimapZoom(int zoom);

private:
	RealRepoModel *model;
	QSortFilterProxyModel *filterModel;
//	EditorView *view;
	QSqlDatabase db;
	
	Ui::MainWindowUi ui;
};

#endif

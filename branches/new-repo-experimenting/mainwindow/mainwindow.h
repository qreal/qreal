#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QModelIndex>
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
	void connectRepo();

private:
	RealRepoModel *model;
	EditorView *view;
	QSqlDatabase db;
	
	Ui::MainWindowUi ui;
};

#endif

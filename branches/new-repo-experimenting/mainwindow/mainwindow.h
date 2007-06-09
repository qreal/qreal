#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtSql/QSqlDatabase>

#include "ui_mainwindow.h"

#include "propertyeditorproxymodel.h"

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
	void closeRepo();
	void adjustMinimapZoom(int zoom);

	void beginTransaction();
	void commitTransaction();
	void rollbackTransaction();

	void print();
	void makeSvg();

private:
	RealRepoModel *model;
	PropertyEditorModel propertyModel;
	QSqlDatabase db;
	
	Ui::MainWindowUi ui;
};

#endif

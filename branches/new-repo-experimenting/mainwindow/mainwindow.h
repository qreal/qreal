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
	void adjustMinimapZoom(int zoom);

	void print();
	void makeSvg();

private:
	RealRepoModel *model;
	PropertyEditorModel propertyModel;
	EditorView *view;
	QSqlDatabase db;
	
	Ui::MainWindowUi ui;
};

#endif

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtSql/QSqlDatabase>

class QAction;
class QActionGroup;
class QLabel;
class QMenu;
class QTreeView;
class RealRepoModel;

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
	MainWindow();

public slots:
	void connectRepo();

private:
	void createActions();
	void createMenus();

	QMenu *fileMenu;
	QMenu *editMenu;
	QMenu *formatMenu;
	QMenu *helpMenu;

	QAction *connectAct;
	QAction *exitAct;

	QTreeView *diagramExplorer;
	QTreeView *objectExplorer;

	RealRepoModel *model;

	QSqlDatabase db;
};

#endif

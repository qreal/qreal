#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QModelIndex>
#include <QtGui/QMainWindow>
#include <QtSql/QSqlDatabase>

class QAction;
class QActionGroup;
class QLabel;
class QMenu;
class QTreeView;
class RealRepoModel;
class EditorView;

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
	MainWindow();

public slots:
	void connectRepo();
	void selectDiagram(const QModelIndex &index);

private:
	void createActions();
	void createMenus();
    bool createDatabase();

	QMenu *fileMenu;
	QMenu *editMenu;
	QMenu *formatMenu;
	QMenu *helpMenu;

	QAction *connectAct;
	QAction *exitAct;

	QTreeView *diagramExplorer;
	QTreeView *objectExplorer;

	RealRepoModel *model;
	EditorView *view;

	QSqlDatabase db;
};

#endif

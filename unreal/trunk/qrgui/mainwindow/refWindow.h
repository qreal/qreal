#ifndef REFWINDOW_H
#define REFWINDOW_H

#include <QtGui>
#include <QWidget>
#include "../../qrrepo/repoApi.h"
#include "mainwindow.h"

namespace Ui {
	class RefWindow;
}

class RefWindow : public QWidget
{
	Q_OBJECT

public:
	explicit RefWindow(const qrRepo::RepoApi *mApi, QString name,
					   QAbstractItemModel* tModel, int r, const QModelIndex &ind,
					   qReal::MainWindow *mWindow, QWidget *parent = 0);
	~RefWindow();

public slots:
	void setName();
	void getId(QListWidgetItem * item, bool bl = true);
	void noSelectClose();

private:
	void keyPressEvent(QKeyEvent *event);

	Ui::RefWindow *ui;
	const qrRepo::RepoApi *api;
	QString typeName;
	QAbstractItemModel* model;
	int role;
	const QModelIndex &index;
	qReal::MainWindow *mainWindow;
	QListWidgetItem *mItem;
};

#endif // REFWINDOW_H

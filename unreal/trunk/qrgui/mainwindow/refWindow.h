#ifndef REFWINDOW_H
#define REFWINDOW_H

#include <QtGui>
#include <QWidget>
#include "../../qrrepo/repoApi.h"

namespace Ui {
	class RefWindow;
}

class RefWindow : public QWidget
{
	Q_OBJECT

public:
	explicit RefWindow(const qrRepo::RepoApi *mApi, QString name,
					   QAbstractItemModel* tModel, int r, const QModelIndex &ind,
					   QWidget *parent = 0);
	~RefWindow();

public slots:
	void setName();

private:
	void keyPressEvent(QKeyEvent *event);

	Ui::RefWindow *ui;
	const qrRepo::RepoApi *api;
	QString typeName;
	QAbstractItemModel* model;
	int role;
	const QModelIndex &index;
};

#endif // REFWINDOW_H

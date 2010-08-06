#pragma once

#include <QPushButton>
#include "../../qrrepo/repoApi.h"
#include "refWindow.h"
#include "mainwindow.h"

class ButtonRefWindow : public QPushButton
{
	Q_OBJECT
public:
	ButtonRefWindow(QWidget *parent, QString typeName,const qrRepo::RepoApi *api,
					QAbstractItemModel* tModel, int r, const QModelIndex &ind,
					qReal::MainWindow *mWindow);

private:
	QString name;
	const qrRepo::RepoApi *mApi;
	QAbstractItemModel* targetModel;
	int role;
	const QModelIndex &index;
	qReal::MainWindow *mainWindow;

private slots:
	void makeWindow();
};

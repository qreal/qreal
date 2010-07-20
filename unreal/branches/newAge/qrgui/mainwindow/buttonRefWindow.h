#pragma once

#include <QPushButton>
#include "../../qrrepo/repoApi.h"
#include "refWindow.h"

class ButtonRefWindow : public QPushButton
{
	Q_OBJECT
public:
	ButtonRefWindow(QWidget *parent, QString typeName,const qrRepo::RepoApi *api,
					QAbstractItemModel* tModel, int r, const QModelIndex &ind);

private:
	QString name;
	const qrRepo::RepoApi *mApi;
	QAbstractItemModel* targetModel;
	int role;
	const QModelIndex &index;

private slots:
	void makeWindow();
};

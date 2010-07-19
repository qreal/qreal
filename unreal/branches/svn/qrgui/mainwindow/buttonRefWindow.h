#pragma once

#include <QPushButton>
#include "../../qrrepo/repoApi.h"
#include "refWindow.h"

class ButtonRefWindow : public QPushButton
{
	Q_OBJECT
public:
	ButtonRefWindow(QWidget *parent, QString typeName,const qrRepo::RepoApi *api);
private:
	QString name;
	const qrRepo::RepoApi *mApi;
private slots:
	void makeWindow();
};

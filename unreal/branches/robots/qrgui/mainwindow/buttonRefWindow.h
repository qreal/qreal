#pragma once

#include <QPushButton>
#include "../../qrrepo/repoApi.h"
#include "refWindow.h"
#include "mainwindow.h"

class ButtonRefWindow : public QPushButton
{
	Q_OBJECT

public:
	ButtonRefWindow(QWidget *parent, QString const &typeName
			, qReal::models::LogicalModelAssistApi const &logicalModelAssistApi
			, int role, const QModelIndex &index, qReal::MainWindow *mainWindow);

private:
	QString mName;
	qrRepo::LogicalRepoApi const &mApi;
	int mRole;
	QPersistentModelIndex const mIndex;
	qReal::MainWindow * const mMainWindow;

private slots:
	void makeWindow();
};

#include "buttonRefWindow.h"

ButtonRefWindow::ButtonRefWindow(QWidget *parent, QString const &typeName
		, qReal::models::LogicalModelAssistApi const &logicalModelAssistApi
		, int role, const QModelIndex &index
		, qReal::MainWindow *mainWindow)
	: QPushButton(parent)
	, mName(typeName)
	, mApi(logicalModelAssistApi.logicalRepoApi())
	, mRole(role)
	, mIndex(index)
	, mMainWindow(mainWindow)
{
	setText("Reference button");
	connect(this, SIGNAL(clicked()), this, SLOT(makeWindow()));
	show();
}

void ButtonRefWindow::makeWindow()
{
	RefWindow *window = new RefWindow(mApi, mName, mRole, mIndex, *mMainWindow);
	window->setWindowModality(Qt::ApplicationModal);
	window->show();
}

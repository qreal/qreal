#include "buttonRefWindow.h"

ButtonRefWindow::ButtonRefWindow(QWidget *parent, QString typeName,
		const qrRepo::RepoApi *api,
		QAbstractItemModel* tModel, int r, const QModelIndex &ind,
		qReal::MainWindow *mWindow)	: QPushButton(parent),
		name(typeName), mApi(api), targetModel(tModel), role(r), index(ind),
		mainWindow(mWindow)
{
	setText("Reference button");
	connect(this, SIGNAL(clicked()), this, SLOT(makeWindow()));
	show();
}

void ButtonRefWindow::makeWindow()
{
	RefWindow *window = new RefWindow(mApi, name, targetModel, role, index, mainWindow);
	window->setWindowModality(Qt::ApplicationModal);
	window->show();
}

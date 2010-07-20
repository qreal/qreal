#include "buttonRefWindow.h"

ButtonRefWindow::ButtonRefWindow(QWidget *parent, QString typeName,
		const qrRepo::RepoApi *api,
		QAbstractItemModel* tModel, int r, const QModelIndex &ind)	: QPushButton(parent),
		name(typeName), mApi(api), targetModel(tModel), role(r), index(ind)
{
	this->setText("Reference button");
	connect(this, SIGNAL(clicked()), this, SLOT(makeWindow()));
	this->show();
}

void ButtonRefWindow::makeWindow()
{
	RefWindow *window = new RefWindow(mApi, name, targetModel, role, index);
	window->setWindowModality(Qt::ApplicationModal);
	window->show();
}

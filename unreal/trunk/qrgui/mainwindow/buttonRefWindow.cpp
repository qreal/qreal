#include "buttonRefWindow.h"

ButtonRefWindow::ButtonRefWindow(QWidget *parent, QString typeName,
		const qrRepo::RepoApi *api)	: QPushButton(parent),
		name(typeName), mApi(api)
{
	this->setText("Reference button");
	connect(this, SIGNAL(clicked()), this, SLOT(makeWindow()));
	this->show();
}

void ButtonRefWindow::makeWindow()
{
	RefWindow *window = new RefWindow(mApi, name);
	window->setWindowModality(Qt::ApplicationModal);
	window->show();
}

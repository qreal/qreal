#include "refWindow.h"
#include "ui_refWindow.h"

RefWindow::RefWindow(const qrRepo::RepoApi *mApi, QString name, QWidget *parent):
	QWidget(parent),
	ui(new Ui::RefWindow),
	api(mApi),
	typeName(name)
{
	ui->setupUi(this);
	qReal::IdList mIdList = api->elementsByType(typeName);
	int size = mIdList.size();
	QString elementName;
	for (int i = 0; i < size; ++i)
	{
		elementName = api->name(mIdList[i]);
		ui->listWidget->addItem(elementName);
	}
}

RefWindow::~RefWindow()
{
	delete ui;
}

#include "refWindow.h"
#include "ui_refWindow.h"
#include "../kernel/ids.h"

RefWindow::RefWindow(const qrRepo::RepoApi *mApi, QString name,
					 QAbstractItemModel* tModel, int r, const QModelIndex &ind,
					 QWidget *parent):
	QWidget(parent),
	ui(new Ui::RefWindow),
	api(mApi),
	typeName(name),
	model(tModel),
	role(r),
	index(ind)
{
	ui->setupUi(this);
	qReal::IdList idList = api->elementsByType(typeName);
	int size = idList.size();
	QString elementName;
	for (int i = 0; i < size; ++i)
	{
		elementName = api->name(idList[i]);
		qReal::IdList parentsIdList = api->parents(idList[i]);
		int parentsListSize = parentsIdList.size();
		QString parentName;
		for (int j = 0; j < parentsListSize; ++j)
		{
			parentName = api->name(parentsIdList[j]);
			if (parentName.contains("Diagram"))
			{
				QListWidgetItem *item = new QListWidgetItem;
				QString text = parentName + ":" + elementName;
				item->setText(text);
				qReal::Id id = idList[i];
				QVariant val = id.toString();
				qDebug() << val;
				item->setData(Qt::ToolTipRole, val);
				ui->listWidget->addItem(item);
			}
		}
	}
}

RefWindow::~RefWindow()
{
	delete ui;
}

void RefWindow::keyPressEvent(QKeyEvent *event)
{
	if ((event->key()) == (Qt::Key_Escape))
		close();
	if ((event->key()) == (Qt::Key_Return))
		setName();
}

void RefWindow::setName()
{
	QListWidgetItem* item = ui->listWidget->currentItem();
	if (item)
		model->setData(index, item->data(Qt::ToolTipRole), role);
	close();
}

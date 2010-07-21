#include "refWindow.h"
#include "ui_refWindow.h"
#include "../kernel/ids.h"

RefWindow::RefWindow(const qrRepo::RepoApi *mApi, QString name,
					 QAbstractItemModel* tModel, int r, const QModelIndex &ind,
					 qReal::MainWindow *mWindow, QWidget *parent):
	QWidget(parent),
	ui(new Ui::RefWindow),
	api(mApi),
	typeName(name),
	model(tModel),
	role(r),
	index(ind),
	mainWindow(mWindow),
	mItem(NULL)
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
				item->setData(Qt::ToolTipRole, val);
				ui->listWidget->addItem(item);
				if (val == model->data(index, role))
					item->setSelected(true);
			}
		}
	}
	ui->mButtonOk->setEnabled(false);
	connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this,
			SLOT(getId(QListWidgetItem*)));
	connect(ui->mButtonCancel, SIGNAL(clicked()), this, SLOT(noSelectClose()));
	connect(ui->listWidget, SIGNAL(itemSelectionChanged()), this,
			SLOT(setEnabledButton()));
	connect(ui->mButtonOk, SIGNAL(clicked()), this, SLOT(setElementId()));
}

RefWindow::~RefWindow()
{
	delete ui;
}

void RefWindow::keyPressEvent(QKeyEvent *event)
{
	if ((event->key()) == (Qt::Key_Escape))
	{
		if (mItem)
			getId(mItem, false);
		close();
	}
	if (((event->key()) == (Qt::Key_Return)) && (ui->mButtonOk->isEnabled()))
	{
		setName();
		setElementId();
	}
}

void RefWindow::setName()
{
	QListWidgetItem* item = ui->listWidget->currentItem();
	if (item)
		model->setData(index, item->data(Qt::ToolTipRole), role);
	getId(mItem, false);
	close();
}

void RefWindow::getId(QListWidgetItem *item, bool bl)
{
	mItem = item;
	qReal::Id const id = qReal::Id::loadFromString(item->data(Qt::ToolTipRole).toString());
	mainWindow->activateItemOrDiagram(id, bl, false);
}

void RefWindow::noSelectClose()
{
	if (mItem)
		getId(mItem, false);
	QVariant data = model->data(index, role);
	model->setData(index, data, role);
	close();
}

void RefWindow::setEnabledButton()
{
	ui->mButtonOk->setEnabled(true);
}

void RefWindow::setElementId()
{
	mainWindow->activateItemOrDiagram(index, false);
}

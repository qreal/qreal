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
	for (int i = 0; i < size; ++i)
	{
		qReal::IdList parentsIdList = api->parents(idList[i]);
		int parentsListSize = parentsIdList.size();
		for (int j = 0; j < parentsListSize; ++j)
		{
			QString parentName = api->name(parentsIdList[j]);
			if (parentName.contains("Diagram"))
			{
				QListWidgetItem *item = new QListWidgetItem();
				QString text = parentName + "::" + api->name(idList[i]);
				item->setText(text);
				QVariant val = idList[i].toString();
				item->setData(Qt::ToolTipRole, val);
				ui->listWidget->addItem(item);
				if (val == model->data(index, role))
					item->setSelected(true);
			}
		}
	}
	ui->mButtonOk->setEnabled(false);
	connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this,
			SLOT(highlightElement(QListWidgetItem*)));
	connect(ui->mButtonCancel, SIGNAL(clicked()), this, SLOT(cancel()));
	connect(ui->listWidget, SIGNAL(itemSelectionChanged()), this,
			SLOT(enableOkButton()));
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
			highlightElement(mItem, false);
		close();
	}
	if (((event->key()) == (Qt::Key_Return)) && (ui->mButtonOk->isEnabled()))
	{
		setPropertyValue();
		setElementId();
	}
}

void RefWindow::setPropertyValue()
{
	QListWidgetItem* item = ui->listWidget->currentItem();
	if (item)
		model->setData(index, item->data(Qt::ToolTipRole), role);
	highlightElement(mItem, false);
	close();
}

void RefWindow::highlightElement(QListWidgetItem *item, bool bl)
{
	mItem = item;
	qReal::Id const id = qReal::Id::loadFromString(item->data(Qt::ToolTipRole).toString());
	mainWindow->activateItemOrDiagram(id, bl, false);
}

void RefWindow::cancel()
{
	if (mItem)
		highlightElement(mItem, false);
	close();
}

void RefWindow::enableOkButton()
{
	ui->mButtonOk->setEnabled(true);
}

void RefWindow::setElementId()
{
	mainWindow->activateItemOrDiagram(index, false);
}

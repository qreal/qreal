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
        qReal::TypeList idList = api->elementsByType(typeName);
	int size = idList.size();
	ui->listWidget->setSelectionMode(QAbstractItemView::MultiSelection);
	QString str = model->data(index, role).toString();
	int sizeStr = str.count("$$");
	for (int i = 0; i < size; ++i)
	{
                qReal::TypeList parentsIdList = api->parents(idList[i]);
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
				for (int k = 0; k < sizeStr; ++k)
				{
					if (val == str.section("$$", k, k))
						item->setSelected(true);
				}
			}
		}
	}
	ui->mButtonOk->setEnabled(false);
	connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this,
			SLOT(highlightElement(QListWidgetItem*)));
	connect(ui->mButtonCancel, SIGNAL(clicked()), this, SLOT(cancel()));
	connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this,
			SLOT(enableOkButton(QListWidgetItem*)));
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
	QList<QListWidgetItem *> itemList = ui->listWidget->selectedItems();
	if (!itemList.isEmpty())
	{
		QString idStr;
		for (int i = 0; i < itemList.size(); ++i)
			idStr.append(itemList[i]->data(Qt::ToolTipRole).toString() + "$$");
		model->setData(index, idStr, role);
	}
	close();
}

void RefWindow::highlightElement(QListWidgetItem *item, bool bl)
{
	Q_UNUSED(bl)
	mItem = item;
        qReal::NewType const id = qReal::NewType::loadFromString(item->data(Qt::ToolTipRole).toString());
	if (item->isSelected())
	{
		mainWindow->activateItemOrDiagram(id, true, false);
		elementList << item;
	}
	else
	{
		mainWindow->activateItemOrDiagram(id, false, false);
	}
}

void RefWindow::cancel()
{
	QVariant data = model->data(index, role);
	model->setData(index, data, role);
	setElementId();
	close();
}

void RefWindow::enableOkButton(QListWidgetItem* item)
{
	Q_UNUSED(item);
	ui->mButtonOk->setEnabled(true);
}

void RefWindow::setElementId()
{
	for (int i = 0; i < elementList.size(); ++i)
	{
                qReal::NewType const id = qReal::NewType::loadFromString(elementList[i]->data(Qt::ToolTipRole).toString());
		mainWindow->activateItemOrDiagram(id, false, false);
	}
	mainWindow->activateItemOrDiagram(index, false);
}

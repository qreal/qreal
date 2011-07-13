#include "refWindow.h"
#include "ui_refWindow.h"
#include "../kernel/ids.h"

RefWindow::RefWindow(qrRepo::LogicalRepoApi const &logicalRepoApi, QString const &name,
		int role, QModelIndex const &index,
		qReal::MainWindow &mainWindow)
	: QWidget(NULL)
	, mUi(new Ui::RefWindow)
	, mApi(logicalRepoApi)
	, mTypeName(name)
	, mRole(role)
	, mIndex(index)
	, mMainWindow(mainWindow)
	, mItem(NULL)
{
	mUi->setupUi(this);
	qReal::IdList idList = mApi.elementsByType(mTypeName);
	int size = idList.size();
	mUi->listWidget->setSelectionMode(QAbstractItemView::MultiSelection);
	QString const str = mIndex.data(mRole).toString();
	int sizeStr = str.count("$$");
	for (int i = 0; i < size; ++i)
	{
		qReal::Id parentId = mApi.parent(idList[i]);
		QString parentName = mApi.name(parentId);
		if (parentName.contains("Diagram"))
		{
			QListWidgetItem *item = new QListWidgetItem();
			QString text = parentName + "::" + mApi.name(parentId);
			item->setText(text);
			QVariant val = idList[i].toString();
			item->setData(Qt::ToolTipRole, val);
			mUi->listWidget->addItem(item);
			for (int k = 0; k < sizeStr; ++k)
			{
				if (val == str.section("$$", k, k))
					item->setSelected(true);
			}
		}
	}
	mUi->mButtonOk->setEnabled(false);
	connect(mUi->listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this,
			SLOT(highlightElement(QListWidgetItem*)));
	connect(mUi->mButtonCancel, SIGNAL(clicked()), this, SLOT(cancel()));
	connect(mUi->listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this,
			SLOT(enableOkButton(QListWidgetItem*)));
	connect(mUi->mButtonOk, SIGNAL(clicked()), this, SLOT(setElementId()));
}

RefWindow::~RefWindow()
{
	delete mUi;
}

void RefWindow::keyPressEvent(QKeyEvent *event)
{
	if ((event->key()) == (Qt::Key_Escape))
	{
		if (mItem)
			highlightElement(mItem, false);
		close();
	}
	if (((event->key()) == (Qt::Key_Return)) && (mUi->mButtonOk->isEnabled()))
	{
		setPropertyValue();
		setElementId();
	}
}

void RefWindow::setPropertyValue()
{
	QList<QListWidgetItem *> itemList = mUi->listWidget->selectedItems();
	if (!itemList.isEmpty())
	{
		QString idStr;
		for (int i = 0; i < itemList.size(); ++i)
			idStr.append(itemList[i]->data(Qt::ToolTipRole).toString() + "$$");
		QAbstractItemModel *model = const_cast<QAbstractItemModel *>(mIndex.model());
		model->setData(mIndex, idStr, mRole);
	}
	close();
}

void RefWindow::highlightElement(QListWidgetItem *item, bool bl)
{
	Q_UNUSED(bl)
	mItem = item;
	qReal::Id const id = qReal::Id::loadFromString(item->data(Qt::ToolTipRole).toString());
	if (item->isSelected()) {
		mMainWindow.activateItemOrDiagram(id, true, false);
		mElementList << item;
	}
	else {
		mMainWindow.activateItemOrDiagram(id, false, false);
	}
}

void RefWindow::cancel()
{
	QVariant data = mIndex.data(mRole);
	QAbstractItemModel *model = const_cast<QAbstractItemModel *>(mIndex.model());
	model->setData(mIndex, data, mRole);
	setElementId();
	close();
}

void RefWindow::enableOkButton(QListWidgetItem* item)
{
	Q_UNUSED(item);
	mUi->mButtonOk->setEnabled(true);
}

void RefWindow::setElementId()
{
	for (int i = 0; i < mElementList.size(); ++i)
	{
		qReal::Id const id = qReal::Id::loadFromString(mElementList[i]->data(Qt::ToolTipRole).toString());
		mMainWindow.activateItemOrDiagram(id, false, false);
	}
	mMainWindow.activateItemOrDiagram(mIndex, false);
}
